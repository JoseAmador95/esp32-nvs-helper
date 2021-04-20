#include <Arduino.h>
#include "nvs_lib.h"
#include "esp_system.h"
#include "unity.h"

#define NVS_NAMESPACE "p1"
#define KEY1 "array"
#define KEY2 "string"

#define DEBUG_TAG "main"

void setup()
{
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  esp_err_t res = 0;

  nvs_flash_erase();
  ESP_ERROR_CHECK(res);

  res = nvs_flash_init();
  if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    res = nvs_flash_init();
  }
  ESP_ERROR_CHECK(res);

  static const uint8_t blob1[4] = {1, 2, 3, 4};
  static const char *blob2 = "This is a string!";

  static nvs_handle handle;
  ESP_LOGV(DEBUG_TAG, "NVS handle created");

  res = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
  ESP_ERROR_CHECK(res);
  ESP_LOGV(DEBUG_TAG, "NVS namespace opened");

  ESP_LOGV(DEBUG_TAG, "Starting to write blob1");
  NVS_writeValue(handle, KEY1, blob1, sizeof(blob1));
  ESP_LOGV(DEBUG_TAG, "blob1 written");

  ESP_LOGV(DEBUG_TAG, "Starting to write blob2");
  NVS_confirmedWrite(handle, KEY2, (uint8_t *)blob2, 18);
  ESP_LOGV(DEBUG_TAG, "blob2 written");

  uint8_t buff1[4], buff2[18];
  ESP_LOGV(DEBUG_TAG, "Reading from key1");
  res = NVS_readValue(handle, KEY1, buff1, 4);
  ESP_LOGV(DEBUG_TAG, "Reading status for key1: %d", res);

  ESP_LOGV(DEBUG_TAG, "Reading from key1");
  NVS_readValue(handle, KEY2, buff2, 18);
  ESP_LOGV(DEBUG_TAG, "Reading status for key1: %d", res);

  ESP_LOGV(DEBUG_TAG, "Erasing %s", KEY1);
  NVS_eraseKey(handle, KEY1);

  ESP_LOGV(DEBUG_TAG, "Reading from key1");
  res = NVS_readValue(handle, KEY1, buff1, 4);
  ESP_LOGV(DEBUG_TAG, "Reading status for key1: %d", res);

  ESP_LOGV(DEBUG_TAG, "Erasing NVS");
  NVS_resetFlash(handle);

  ESP_LOGV(DEBUG_TAG, "Reading from key1");
  NVS_readValue(handle, KEY2, buff2, 18);
  ESP_LOGV(DEBUG_TAG, "Reading status for key1: %d", res);
  

  if (memcmp(blob1, buff1, 4) == 0)
  {
    ESP_LOGI(DEBUG_TAG, "blob1 equals buff1");
    ESP_LOG_BUFFER_HEX_LEVEL(DEBUG_TAG, blob1, 4, ESP_LOG_VERBOSE);
  }
  else
  {
    ESP_LOGV(DEBUG_TAG, "blob1 not equals buff1");
  }

  if (memcmp(blob2, buff2, 18) == 0)
  {
    ESP_LOGV(DEBUG_TAG, "blob2 equals buff2");
    ESP_LOG_BUFFER_CHAR(DEBUG_TAG, blob2, 18);
  }

  else
  {
    ESP_LOGI(DEBUG_TAG, "blob2 not equals buff2");
  }
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(10000));
}