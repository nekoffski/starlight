#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 1 || ! -d $1 ]]; then
    echo "Usage: $0 <shader-directory>" >&2
    exit 1
fi

command -v slangc >/dev/null || {
    echo "slangc was not found in PATH" >&2
    exit 1
}

case "$(uname -s)" in
    Darwin)
        if ! xcrun -sdk macosx --find metal >/dev/null 2>&1; then
            echo "Apple Metal compiler not found; install/select Xcode and its Metal Toolchain" >&2
            exit 1
        fi
        extension=metallib
        target_args=(-target metallib)
        ;;
    Linux | MINGW* | MSYS* | CYGWIN*)
        extension=spv
        target_args=(-target spirv -profile spirv_1_5)
        ;;
    *)
        echo "Unsupported platform: $(uname -s)" >&2
        exit 1
        ;;
esac

count=0
while IFS= read -r -d '' shader; do
    output="${shader%.slang}.${extension}"
    reflection="${output}.json"
    echo "Compiling $shader -> $output, $reflection"
    slangc "$shader" "${target_args[@]}" -o "$output" \
        -reflection-json "$reflection"
    ((count += 1))
done < <(find "$1" -type f -name '*.slang' -print0)

if [[ $count -eq 0 ]]; then
    echo "No .slang shaders found in $1" >&2
    exit 1
fi

echo "Compiled $count shader(s)"
