#ifndef NVS_LIB_H
#define NVS_LIB_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "nvs_flash.h"
#include "string.h"

/**
 * \brief   Read a variable/array from the Non-Volatile Storage.
 * Function for reading from the Non-Volatile Storage using a key identifier to locate the data,
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string. (e.g. "MAC", "JWT", "ID")
 * @param[out]  uint8_t array to store the retrieved variable/array.
 * @param[in]   size_t Expected variable/array size in bytes.
 * @return  Returns **TRUE** if successful.
 */ 
bool NVS_readValue(nvs_handle handle, const char* key, uint8_t* data, size_t expected_length, bool silent);

/**
 * \brief   Write a variable/array from the Non-Volatiale Storage.
 * Function for writing a variable or array into the Non-Volatile Storage using a key identifier.
 * This function avoids re-writing redundant data by reading the contents of the memory chunk
 * pointed by the key identifier.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string. (e.g. "MAC", "JWT", "ID")
 * @param[in]   uint8_t array to store the retrieved variable/array.
 * @param[in]   size_t Variable/array size in bytes.
 * @return  Returns **TRUE** if successful.
 */
bool NVS_writeValue(nvs_handle handle, const char* key, const uint8_t* data, size_t len);

/**
 * \brief   Check the required bytes to store the data key-pair in NVS.
 * Function for estimating the number of bytes required to store the data labeled using a key
 * identifier.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string. (e.g. "MAC", "JWT", "ID")
 * @return      Returns an `uint32_t` containing the number of bytes required to store the data.
 */
uint32_t NVS_requiredBytes(nvs_handle handle, const char *key);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // NVS_LIB_H