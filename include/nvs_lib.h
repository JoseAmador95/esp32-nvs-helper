#ifndef NVS_LIB_H
#define NVS_LIB_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "nvs_flash.h"
#include "string.h"

/**
 * @brief       Read from the Non-Volatile Storage.
 * @details     Function for reading from the Non-Volatile Storage using a key identifier to locate the data.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string. 
 * @param[out]  uint8_t Pointer to the array to store the retrieved data.
 * @param[in]   size_t Expected data size in bytes.
 * @return      Returns `true` if successful.
 */ 
bool NVS_readValue(nvs_handle handle, const char* key, uint8_t* data, size_t expected_length);

/**
 * @brief       Write to the Non-Volatiale Storage.
 * @details     Function for writing data into the Non-Volatile Storage using a key identifier.
 *              This function avoids re-writing redundant data by reading the contents of the key-pair segment.
 *              pointed by the key identifier.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string. 
 * @param[in]   uint8_t* Pointer to the data to be stored.
 * @param[in]   size_t Size of the data to store in bytes.
 * @return      Returns `true` if successful.
 */
bool NVS_writeValue(nvs_handle handle, const char* key, const uint8_t* data, size_t len);

/**
 * @brief       Check the required bytes to store the data key-pair in NVS.
 * @details     Function for estimating the number of bytes required to store the data labeled using a key
 *              identifier.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string. 
 * @return      Returns an `uint32_t` containing the number of bytes required to store the data.
 */
uint32_t NVS_requiredBytes(nvs_handle handle, const char *key);

/**
 * @brief       Writes data into the NVS and confirms the content.
 * @details     This function calls `NVS_writeValue` and then confirms the integrity of the data using
 *              `NVS_readValue`. If the data at any stage cannot be confirmed, the function tries to
 *              delete the key-data-pair.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* NVS key identifier string.
 * @param[in]   uint8_t* Pointer to the data to be stored.
 * @param[in]   size_t Size of the data to store in bytes.
 * @return      Returns `true` if the data was successfuly written and confirmed.
 * */
bool NVS_confirmedWrite(nvs_handle handle, const char* key, const uint8_t* data, size_t len);

/**
 * @brief       Erases all the keys in a namespace.
 * @details     This function deletes all the contents of the NVS namespace.
 * @param[in]   nvs_handle NVS handle.
 * @return      Returns `true` if the NVS was sucessfully erased.
 * */
bool NVS_resetFlash(nvs_handle handle);

/**
 * @brief       Erases a key in a NVS namespace.
 * @details     Erases a key in a NVS namespace. The namespace must be previously opened using the
 *              `nvs_open` function from the IDF.
 * @param[in]   nvs_handle NVS handle.
 * @param[in]   char* Key in namespace to erase.
 * @return      Returns `true` if the key was sucessfully erased.
 * */
bool NVS_eraseKey(nvs_handle handle, const char* key);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // NVS_LIB_H