#include "nvs_lib.h"
#include "nvs_flash.h"
#include "esp32-hal-log.h"
#include "esp_system.h"

#define NVS_LIB_DEBUG_TAG "NVS";

bool NVS_readValue(nvs_handle handle, const char *key, uint8_t *data, size_t expected_length)
{
    size_t size = expected_length;
    esp_err_t res = nvs_get_blob(handle, key, data, &size);
    if (res == ESP_OK && size == expected_length)
    {
        return true;
    }

    if (res == ESP_OK && size != expected_length)
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "Invalid size of NVS data for %s", key);
        return false;
    }

    if (res == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "No NVS data found for %s", key);
        return false;
    }

    ESP_ERROR_CHECK(res);
    return false;
}

bool NVS_writeValue(nvs_handle handle, const char *key, const uint8_t *data, size_t len)
{
    uint8_t buf[NVS_requiredBytes(handle, key)];
    if (NVS_readValue(handle, key, buf, len) && memcmp(buf, data, len) == 0)
    {
        ESP_LOGV(NVS_LIB_DEBUG_TAG, "Input data equals stored data");
        return true; // Input data equals stored data
    }
    esp_err_t res = nvs_set_blob(handle, key, data, len);
    ESP_ERROR_CHECK(res);

    if (res == ESP_OK)
    {
        ESP_LOGV(NVS_LIB_DEBUG_TAG, "NVS write successful");
    }
    else
    {
        ESP_LOGV(NVS_LIB_DEBUG_TAG, "NVS write failed");
    }
    
    return res == ESP_OK;
}

uint32_t NVS_requiredBytes(nvs_handle handle, const char *key)
{
    size_t requiredBytes;
    esp_err_t res = nvs_get_blob(handle, key, NULL, &requiredBytes);
    if (res == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "No NVS data found for %s", key);
        return 0;
    }
    else if (res != ESP_OK)
    {
        ESP_ERROR_CHECK(res);
        return 0;
    }

    return requiredBytes;
}

bool NVS_confirmedWrite(nvs_handle handle, const char *key, const uint8_t *data, size_t len)
{
    if (!NVS_writeValue(handle, key, data, len))
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "Error while writing data into NVS");
        return false;
    }

    if (NVS_requiredBytes(handle, key) != len)
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "Retrieved data has a different length than written data");
        NVS_eraseKey(handle, key);
        return false;
    }

    uint8_t buff[len];

    if (!NVS_readValue(handle, key, buff, len))
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "The data was not read correctly");
        NVS_eraseKey(handle, key);
        return false;
    }

    if (!memcmp(buff, data, len))
    {
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "Data content cannot be confirmed");
        NVS_eraseKey(handle, key);
        return false;
    }

    nvs_commit(handle);
    return true;
}

bool NVS_resetFlash(nvs_handle handle)
{
    esp_err_t res;
    res = nvs_erase_all(handle);
    ESP_ERROR_CHECK(res);

    res = nvs_commit(handle);
    ESP_ERROR_CHECK(res);

    if (res != ESP_OK){
        ESP_LOGW(NVS_LIB_DEBUG_TAG, "NVS could not be erased");
    }

    return res == ESP_OK;
}

bool NVS_eraseKey(nvs_handle handle, const char* key)
{
    esp_err_t res = nvs_erase_key(handle, key);

    if (res == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGV(NVS_LIB_DEBUG_TAG, "NVS key not found");
    }

    ESP_ERROR_CHECK(res);

    return res == ESP_OK;
}