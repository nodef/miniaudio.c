#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WAV_HEADER_SIZE 44


void write_wav_header(FILE *pFile, ma_format format, ma_uint32 channels,
                      ma_uint32 sampleRate, ma_uint32 dataSize) {
  ma_uint32 byteRate = sampleRate * channels * ma_get_bytes_per_sample(format);
  ma_uint16 blockAlign =
      (ma_uint16)(channels * ma_get_bytes_per_sample(format));

  fseek(pFile, 0, SEEK_SET);
  fwrite("RIFF", 1, 4, pFile);
  ma_uint32 chunkSize = 36 + dataSize;
  fwrite(&chunkSize, 4, 1, pFile);
  fwrite("WAVE", 1, 4, pFile);

  fwrite("fmt ", 1, 4, pFile);
  ma_uint32 subChunk1Size = 16;
  fwrite(&subChunk1Size, 4, 1, pFile);
  ma_uint16 audioFormat = 1;
  fwrite(&audioFormat, 2, 1, pFile);
  fwrite(&channels, 2, 1, pFile);
  fwrite(&sampleRate, 4, 1, pFile);
  fwrite(&byteRate, 4, 1, pFile);
  fwrite(&blockAlign, 2, 1, pFile);
  ma_uint16 bitsPerSample = (ma_uint16)(ma_get_bytes_per_sample(format) * 8);
  fwrite(&bitsPerSample, 2, 1, pFile);

  fwrite("data", 1, 4, pFile);
  fwrite(&dataSize, 4, 1, pFile);
}


void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  FILE *pFile = (FILE *)pDevice->pUserData;
  if (pFile) {
    fwrite(pInput,
           ma_get_bytes_per_frame(pDevice->capture.format,
                                  pDevice->capture.channels),
           frameCount, pFile);
  }
}


int main() {
  ma_result result;
  ma_device_config deviceConfig;
  ma_device device;
  FILE *pFile = fopen("examples/recorded_audio.wav", "wb");

  if (!pFile) {
    printf("Failed to open file for writing.\n");
    return -1;
  }

  // Write a placeholder WAV header
  write_wav_header(pFile, ma_format_s16, 1, 44100, 0);

  // Configure the device for audio capture
  deviceConfig = ma_device_config_init(ma_device_type_capture);
  deviceConfig.capture.format = ma_format_s16;
  deviceConfig.capture.channels = 1;
  deviceConfig.sampleRate = 44100;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = pFile;

  // Initialize the device
  result = ma_device_init(NULL, &deviceConfig, &device);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize capture device.\n");
    fclose(pFile);
    return -2;
  }

  // Start the device
  result = ma_device_start(&device);
  if (result != MA_SUCCESS) {
    printf("Failed to start capture device.\n");
    ma_device_uninit(&device);
    fclose(pFile);
    return -3;
  }

  printf("Recording... Press Enter to stop.\n");
  getchar();

  // Stop and uninitialize the device
  ma_device_uninit(&device);

  // Finalize the WAV header with the correct data size
  ma_uint32 dataSize = ftell(pFile) - WAV_HEADER_SIZE;
  write_wav_header(pFile, ma_format_s16, 1, 44100, dataSize);

  fclose(pFile);

  printf("Recording saved to examples/recorded_audio.wav\n");
  return 0;
}
