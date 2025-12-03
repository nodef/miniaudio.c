#!/usr/bin/env bash
URL="https://github.com/mackron/miniaudio/archive/refs/tags/0.11.23.zip"
ZIP="${URL##*/}"
DIR="miniaudio-${ZIP%.zip}"
mkdir -p .build
cd .build

# Download the release
if [ ! -f "$ZIP" ]; then
  echo "Downloading $ZIP from $URL ..."
  curl -L "$URL" -o "$ZIP"
  echo ""
fi

# Unzip the release
if [ ! -d "$DIR" ]; then
  echo "Unzipping $ZIP to .build/$DIR ..."
  cp "$ZIP" "$ZIP.bak"
  unzip -q "$ZIP"
  rm "$ZIP"
  mv "$ZIP.bak" "$ZIP"
fi

# Copy the libs to the package directory
echo "Copying libs to miniaudio/ ..."
rm -rf ../miniaudio
mkdir -p ../miniaudio
cp -f "$DIR/miniaudio.h" ../miniaudio/
cp -f "$DIR/miniaudio.c" ../miniaudio/
