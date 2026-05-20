#!/bin/bash

set -e

PROTOC="/opt/conan2/.conan2/p/b/proto5cdcdc3c3fdbe/p/bin/protoc"

PROTO_DIR="./test"
OUT_DIR="./test"

if [ ! -x "$PROTOC" ]; then
    echo "protoc not found or not executable: $PROTOC"
    exit 1
fi

echo "Using protoc:"
"$PROTOC" --version

"$PROTOC" \
    -I "$PROTO_DIR" \
    --cpp_out="$OUT_DIR" \
    "$PROTO_DIR/login.proto"

echo "Generate protobuf files success."