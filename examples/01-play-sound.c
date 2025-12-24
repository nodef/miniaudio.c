#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <stdio.h>


int main() {
  ma_result result;
  ma_engine engine;

  // Initialize the audio engine
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize audio engine.\n");
    return -1;
  }

  // Play a sound file
  ma_engine_play_sound(&engine, "examples/hello.wav", NULL);

  printf("Press Enter to quit...\n");
  getchar();

  // Uninitialize the engine
  ma_engine_uninit(&engine);

  return 0;
}
