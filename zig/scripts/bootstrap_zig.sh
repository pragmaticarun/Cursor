#!/usr/bin/env bash
set -euo pipefail

# Install a local Zig toolchain into .tools/zig
# Usage: scripts/bootstrap_zig.sh [version]
# Defaults to 0.14.0

VERSION="${1:-0.14.0}"
OS="$(uname -s)"
ARCH="$(uname -m)"

case "$OS" in
  Linux) os=linux ;;
  Darwin) os=macos ;;
  *) echo "Unsupported OS: $OS" >&2; exit 1 ;;
esac

case "$ARCH" in
  x86_64|amd64) arch=x86_64 ;;
  aarch64|arm64) arch=aarch64 ;;
  *) echo "Unsupported arch: $ARCH" >&2; exit 1 ;;
esac

BASE_URL="https://ziglang.org/download/${VERSION}"
FILENAME="zig-${os}-${arch}-${VERSION}.tar.xz"
URL="${BASE_URL}/${FILENAME}"

DEST_DIR="$(dirname "$0")/../.tools/zig"
mkdir -p "$DEST_DIR"

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

echo "Downloading $URL" >&2
curl -fsSL "$URL" -o "$TMP_DIR/$FILENAME"

echo "Extracting..." >&2
tar -C "$TMP_DIR" -xf "$TMP_DIR/$FILENAME"

EXTRACTED_DIR="$TMP_DIR/zig-${os}-${arch}-${VERSION}"
if [[ ! -d "$EXTRACTED_DIR" ]]; then
  echo "Extracted directory not found: $EXTRACTED_DIR" >&2
  exit 1
fi

rm -rf "$DEST_DIR"/*
cp -R "$EXTRACTED_DIR"/* "$DEST_DIR"/

echo "Installed Zig to $DEST_DIR" >&2
"$DEST_DIR/zig" version

