#!/usr/bin/env bash
# Fetch the latest version of the library
fetch() {
if [ -d "miniaudio" ]; then return; fi
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
cd ..

# Copy the libs to the package directory
echo "Copying libs to miniaudio/ ..."
rm -rf miniaudio
mkdir -p miniaudio
cp -f ".build/$DIR/miniaudio.h" miniaudio/
cp -f ".build/$DIR/miniaudio.c" miniaudio/
}


# Test the project
test() {
echo "Running 01-play-sound.c ..."
clang -I. -o 01.exe examples/01-play-sound.c     && ./01.exe && echo -e "\n"
echo "Running 02-record-audio.c ..."
clang -I. -o 02.exe examples/02-record-audio.c   && ./02.exe && echo -e "\n"
echo "Running 03-audio-effects.c ..."
clang -I. -o 03.exe examples/03-audio-effects.c  && ./03.exe && echo -e "\n"
echo "Running 04-custom-backend.c ..."
clang -I. -o 04.exe examples/04-custom-backend.c && ./04.exe && echo -e "\n"
}


# Main script
if [[ "$1" == "test" ]]; then test
elif [[ "$1" == "fetch" ]]; then fetch
else echo "Usage: $0 {fetch|test}"; fi
