#!/bin/bash

BINARY_FILE="$1"
TARGET_FILE="$2"

if [[ ! -f "$BINARY_FILE" ]]; then
    echo "Error: binary file not found: $BINARY_FILE"
    exit 1
fi

if [[ ! -f "$TARGET_FILE" ]]; then
    echo "Error: target file not found: $TARGET_FILE"
    exit 1
fi


HASH=$(sha256sum "$BINARY_FILE" | awk '{print $1}')

echo "Computed hash: $HASH"


if grep -q "$HASH" "$TARGET_FILE"; then
    echo "✅ Hash found in $TARGET_FILE"
    exit 0
else
    echo "❌ Hash NOT found in $TARGET_FILE"
    exit 2
fi
