#!/bin/bash

set -e

ACTION="$1"
MODE="$2"

case "$MODE" in
    d|debug|Debug)
        BUILD_TYPE="Debug"
        ;;
    r|release|Release)
        BUILD_TYPE="Release"
        ;;
    "")
        BUILD_TYPE="Debug"
        ;;
    *)
        echo "Invalid build type: $MODE"
        echo "Usage:"
        echo "  ./autobuild.sh install [d|r]"
        echo "  ./autobuild.sh build   [d|r]"
        exit 1
        ;;
esac

CONAN_DIR="conan"
BUILD_DIR="build"

case "$ACTION" in
    install)
        conan install . --output-folder="build/$CONAN_DIR" --build=missing -s build_type="$BUILD_TYPE"
        ;;

    build)
        cmake -S . -B "$BUILD_DIR" \
            -DCMAKE_TOOLCHAIN_FILE="$CONAN_DIR/build/$BUILD_TYPE/generators/conan_toolchain.cmake" \
            -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

        cmake --build "$BUILD_DIR" -j
        ;;

    all)
        conan install . \
            --output-folder="$CONAN_DIR" \
            --build=missing \
            -s build_type="$BUILD_TYPE"

        cmake -S . -B "$BUILD_DIR" \
            -DCMAKE_TOOLCHAIN_FILE="$CONAN_DIR/build/$BUILD_TYPE/generators/conan_toolchain.cmake" \
            -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

        cmake --build "$BUILD_DIR" -j
        ;;

    *)
        echo "Usage:"
        echo "  ./autobuild.sh install [d|r]"
        echo "  ./autobuild.sh build   [d|r]"
        echo "  ./autobuild.sh all     [d|r]"
        exit 1
        ;;
esac