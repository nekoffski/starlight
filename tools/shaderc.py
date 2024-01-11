#!/usr/bin/python3

import typing
import os
import argparse
import subprocess

parser = argparse.ArgumentParser(
    prog='shaderc',
    description='Shader compiler wrapper')

parser.add_argument('-i', '--input')
parser.add_argument('-o', '--output-dir')

args = parser.parse_args()

extensions = ['.frag', '.vert']

is_directory = os.path.isdir(args.input)


def is_shader_file(path: str) -> bool:
    for ext in extensions:
        if path.endswith(ext):
            return True
    return False


def determine_file(path: str) -> typing.Optional[str]:
    if is_shader_file(path):
        return path if os.path.isfile(path) else None
    for extension in extensions:
        full_path = path + extension
        if os.path.exists(full_path):
            return full_path
    return None


def compile_shader(path: str, output: str):
    filename = os.path.basename(path)
    out_file = output + "/" + filename + '.spv'
    stage = path.split('.')[-1]
    print(f"$ - Compiling '{stage}' shader {path}")

    command = f'glslc -fshader-stage={stage} {path} -o {out_file}'
    p = subprocess.run(command.split(' '), stderr=subprocess.PIPE)
    error = p.stderr.decode('utf-8')

    if len(error) > 0:
        print(f"\t ERROR: {error}")
    else:
        print(f"\tSUCCESS: output: {out_file}")


print(f"$ - Compiling shaders: {args.input}")
if is_directory:
    print("$ - Given input is a directory, will compile all shaders stored there")
print(f"$ - Looking for extensions: {extensions}")


if is_directory:
    for f in os.listdir(args.input):
        full_path: str = args.input + "/" + f
        if os.path.isfile(full_path) and is_shader_file(full_path):
            compile_shader(full_path, args.output_dir)

else:
    path = determine_file(args.input)
    if not path:
        print(f"$ - ERROR: Could not find or determine file: {args.input}")
    else:
        compile_shader(path, args.output_dir)
