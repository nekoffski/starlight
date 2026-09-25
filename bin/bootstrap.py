#!/usr/bin/env python3
"""Select a supported compiler and save a local Conan profile; do not build."""

import os
import re
import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROFILE = ROOT / ".starlight" / "bootstrap.profile"


def fail(message):
    raise SystemExit(f"bootstrap: {message}")


def find_executable(name):
    if not name:
        fail("compiler executable is empty")
    found = shutil.which(name)
    if found is None:
        fail(f"{name!r} was not found in PATH")
    return Path(found).absolute()


def compiler_info(executable):
    result = subprocess.run(
        [str(executable), "--version"], capture_output=True, text=True, check=True
    )
    first_line = result.stdout.splitlines()[0]
    if match := re.search(r"Apple clang version (\d+)", first_line):
        return "Apple Clang", int(match.group(1)), first_line
    if match := re.search(r"\bclang version (\d+)", first_line):
        return "Clang", int(match.group(1)), first_line
    if re.search(r"(?:gcc|g\+\+|c\+\+|GNU)", first_line, re.I):
        result = subprocess.run(
            [str(executable), "-dumpfullversion", "-dumpversion"],
            capture_output=True,
            text=True,
            check=True,
        )
        match = re.match(r"(\d+)", result.stdout)
        if match:
            return "GCC", int(match.group(1)), first_line
    fail(f"unsupported compiler: {first_line}")


def matching_c_compiler(cxx):
    if "CC" in os.environ:
        return find_executable(os.environ["CC"])
    name = cxx.name
    if name.startswith("g++"):
        name = "gcc" + name[3:]
    elif name.startswith("clang++"):
        name = "clang" + name[7:]
    elif name == "c++":
        name = "cc"
    else:
        fail(f"cannot infer the C compiler for {cxx}; set CC explicitly")
    candidate = cxx.with_name(name)
    if not candidate.is_file():
        fail(f"{candidate} was not found; set CC explicitly")
    return candidate


def detect_profile(cc, cxx, family, major):
    conan = shutil.which("conan")
    if conan is None:
        fail("Conan 2 was not found in PATH")
    with tempfile.TemporaryDirectory(prefix="starlight-bootstrap-") as temp:
        env = {**os.environ, "CONAN_HOME": temp, "CC": str(cc), "CXX": str(cxx)}
        result = subprocess.run(
            [conan, "profile", "detect", "--force"],
            env=env,
            capture_output=True,
            text=True,
        )
        if result.returncode:
            fail(f"Conan profile detection failed:\n{result.stderr or result.stdout}")
        detected = (Path(temp) / "profiles" / "default").read_text()

    conan_family = {"GCC": "gcc", "Clang": "clang", "Apple Clang": "apple-clang"}[family]
    if (
        not re.search(rf"^compiler={re.escape(conan_family)}$", detected, re.M)
        or not re.search(rf"^compiler\.version={major}$", detected, re.M)
    ):
        fail("Conan detected a different compiler than the selected CC/CXX pair")
    detected, count = re.subn(
        r"^compiler\.cppstd=.*$", "compiler.cppstd=23", detected, flags=re.M
    )
    if count != 1:
        fail("Conan did not detect a C++ standard")
    return (
        detected.rstrip()
        + "\n\n[conf]\n"
        + (
            "tools.build:compiler_executables="
            + repr({"c": str(cc), "cpp": str(cxx)})
            + "\n"
        )
    )


def main():
    cxx = find_executable(os.environ.get("CXX", "c++"))
    family, major, version = compiler_info(cxx)
    minimum = {"GCC": 14, "Clang": 18, "Apple Clang": 17}[family]
    if major < minimum:
        hint = "Xcode 16.3+" if family == "Apple Clang" else f"{family} {minimum}+"
        fail(f"{version} is too old; use {hint} and rerun make bootstrap")

    cc = matching_c_compiler(cxx)
    c_family, c_major, _ = compiler_info(cc)
    if (c_family, c_major) != (family, major):
        fail(f"CC ({cc}) must match CXX ({cxx}) in compiler family and major version")

    profile = detect_profile(cc, cxx, family, major)
    PROFILE.parent.mkdir(exist_ok=True)
    PROFILE.write_text(profile)

    print(f"C++ compiler: {cxx} ({version})")
    print(f"C compiler:   {cc}")
    print(f"Saved Conan profile: {PROFILE}")
    print("No dependencies installed and no project build run.")
    for cache in (ROOT / "build" / name / "CMakeCache.txt" for name in ("Debug", "Release")):
        if not cache.is_file():
            continue
        match = re.search(
            r"^CMAKE_CXX_COMPILER:FILEPATH=(.*)$", cache.read_text(), re.M
        )
        if match and Path(match.group(1)).resolve() != cxx.resolve():
            print(f"Existing CMake cache uses another compiler: {cache}")
            print("Run make clean before the next build.")


if __name__ == "__main__":
    try:
        main()
    except (OSError, subprocess.CalledProcessError) as error:
        fail(str(error))
