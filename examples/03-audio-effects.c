#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <stdio.h>

float g_volume = 0.5f; // Volume level (0.0 = mute, 1.0 = full volume)


void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  ma_uint32 sampleCount = frameCount * pDevice->playback.channels;
  float *pOutputF32 = (float *)pOutput;
  const float *pInputF32 = (const float *)pInput;

  for (ma_uint32 i = 0; i < sampleCount; i++) {
    pOutputF32[i] = pInputF32[i] * g_volume; // Apply volume adjustment
  }
}


int main() {
  ma_result result;
  ma_device_config deviceConfig;
  ma_device device;

  // Configure the device for playback
  deviceConfig = ma_device_config_init(ma_device_type_duplex);
  deviceConfig.playback.format = ma_format_f32;
  deviceConfig.playback.channels = 2;
  deviceConfig.capture.format = ma_format_f32;
  deviceConfig.capture.channels = 2;
  deviceConfig.sampleRate = 44100;
  deviceConfig.dataCallback = data_callback;

  // Initialize the device
  result = ma_device_init(NULL, &deviceConfig, &device);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize audio device.\n");
    return -1;
  }

  // Start the device
  result = ma_device_start(&device);
  if (result != MA_SUCCESS) {
    printf("Failed to start audio device.\n");
    ma_device_uninit(&device);
    return -2;
  }

  printf("Playing with volume adjustment... Press Enter to quit.\n");
  getchar();

  // Cleanup
  ma_device_uninit(&device);

  return 0;
}
