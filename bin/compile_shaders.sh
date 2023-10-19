#!/bin/bash

DIR=$1
COMPILER=glslc
OUTPUT_EXTENSION='.spv'

function help {
    echo "$ - Usage: compile_shaders.sh <path_to_directory>"
}

if [ $# -le 0 ]; then
    help
    exit -1
fi

if ! [ -d $DIR ]; then
    echo "$ - Path $DIR doesn't exist"
    exit -2
fi

if [ `ls $DIR | wc -l` -le 0 ]; then
    echo "$ - Given directory is empty, exitting"
    exit 0
fi

echo "$ - Compiler version"
echo ""
$COMPILER --version
echo ""
echo "$ - Compiling shaders from $DIR"

rm -rf $DIR/*.spv

for FILE in $DIR/*.vert $DIR/*.frag; do
    if ! [ -f $FILE ]; then
        echo "$ - $FILE is a directory, skipping"
        continue
    fi 

    STAGE=`echo $FILE | rev | cut -d '.'  -f1 | rev`
    OUTPUT_PATH=$FILE$OUTPUT_EXTENSION

    echo "$ - Compiling $FILE -> $OUTPUT_PATH" 
    $COMPILER -fshader-stage=$STAGE $FILE -o $OUTPUT_PATH 
    echo "$ - Done"
done


echo "$ - All files compiled"
echo "$"