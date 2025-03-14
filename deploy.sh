#!/bin/bash

{

# Exit on error
set -o errexit
set -o errtrace

stub=$(ls arduino | xargs)
port=$(arduino-cli board list | grep 'usbserial' | awk '{print $1}')

input_path="$PWD/arduino/${stub}"
build_dir="$PWD/build/$stub"

function help() {
    echo "\
CLI wrapper around Arduino

Usage:
./run.sh -h                 Show help and exit

./run.sh compile            Compile
./run.sh upload             Upload
./run.sh                    Compile and upload

"
}

function compile() {
    echo "COMPILING"
    echo

    mkdir -pv "${build_dir}" >/dev/null
    arduino-cli compile \
        --fqbn "arduino:avr:uno" \
        --build-path="${build_dir}" \
        --verbose \
        "${input_path}"

    echo
}

function upload() {
    echo "UPLOADING"
    echo

    arduino-cli upload \
        --fqbn "arduino:avr:uno" \
        --input-dir "${build_dir}" \
        --port "${port}" \
        --verbose

    echo
}

if [ "$1" == '-h' ]; then
    help
    exit
fi

if [ "$1" == 'compile' ]; then
    compile
    exit
fi

if [ "$1" == 'upload' ]; then
    upload
    exit
fi

compile
upload

}