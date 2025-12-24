#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <stdio.h>


void print_device_info(const ma_device_info *pDeviceInfo,
                       ma_uint32 deviceCount) {
  for (ma_uint32 i = 0; i < deviceCount; i++) {
    printf("Device %u: %s\n", i + 1, pDeviceInfo[i].name);
  }
}


int main() {
  ma_result result;
  ma_context_config contextConfig = ma_context_config_init();
  ma_context context;
  ma_device_info *pPlaybackDevices;
  ma_device_info *pCaptureDevices;
  ma_uint32 playbackDeviceCount;
  ma_uint32 captureDeviceCount;

  // Specify multiple backends to try (e.g., WASAPI, DirectSound, and ALSA)
  ma_backend backends[] = {ma_backend_wasapi, ma_backend_dsound,
                           ma_backend_alsa};

  // Initialize the context with the specified backends
  result = ma_context_init(backends, sizeof(backends) / sizeof(backends[0]),
                           &contextConfig, &context);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize context with specified backends.\n");
    return -1;
  }

  printf("Context initialized successfully.\n");

  // Enumerate playback devices
  result =
      ma_context_get_devices(&context, &pPlaybackDevices, &playbackDeviceCount,
                             &pCaptureDevices, &captureDeviceCount);
  if (result != MA_SUCCESS) {
    printf("Failed to enumerate devices.\n");
    ma_context_uninit(&context);
    return -2;
  }

  printf("\nPlayback Devices:\n");
  print_device_info(pPlaybackDevices, playbackDeviceCount);

  printf("\nCapture Devices:\n");
  print_device_info(pCaptureDevices, captureDeviceCount);

  // Uninitialize the context
  ma_context_uninit(&context);

  printf("\nContext uninitialized.\n");
  return 0;
}
