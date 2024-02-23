#include "fatfs_if.h"
#include <printf.h>

FRESULT ffif_res;                           /* FatFs function common result code */
char ffif_path[4];                          /* SD logical drive path */
FATFS ffif_fatFS;                           /* File system object for SD logical drive */
FIL ffif_file;                              /* File object for SD */
FILINFO ffif_fno;

static volatile bool driverInit = false;
static volatile bool sdCardMounted = false;
static uint32_t byteswritten, bytesread;    /* File write/read counts */
uint8_t ffif_writeBuffer[_MAX_SS];              /* File write buffer */
uint8_t ffif_readBuffer[_MAX_SS];              /* File read buffer */

static void clearBuffer(uint8_t* buf)
{
    memset(buf, 0, sizeof(buf));
}

static uint32_t sizeOfBuffer(uint8_t* buf)
{
    uint32_t size = 0;
    while (buf[size] != '\0') size++;
    return size;
}

bool ffif_isDriverInit(void)
{
    return driverInit;
}

bool ffif_isSdMounted(void)
{
    return sdCardMounted;
}

HAL_StatusTypeDef ffif_init(void)
{
    ffif_res = FATFS_LinkDriver(&SD_Driver, ffif_path);
    if (ffif_res == FR_OK)
    {
        driverInit = true;
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

HAL_StatusTypeDef ffif_deinit(void)
{
    ffif_res = FATFS_UnLinkDriver(ffif_path);
    if (ffif_res == FR_OK)
    {
        driverInit = false;
        return HAL_OK;
    }

    return HAL_ERROR;
}

HAL_StatusTypeDef ffif_mountSd(void)
{
    ffif_res = f_mount(&ffif_fatFS, (TCHAR const*) ffif_path, 0);
    if(ffif_res == FR_OK) {
        sdCardMounted = true;
        return HAL_OK;
    } 
    else if (ffif_res == FR_NO_FILESYSTEM)
    {
        printf("Filesystem not FAT based, formatting\r\n");
        if (ffif_formatSd() == HAL_ERROR) return HAL_ERROR;

        ffif_res = f_mount(&ffif_fatFS, (TCHAR const*) ffif_path, 0);
        if(ffif_res == FR_OK) {
           sdCardMounted = true;
            return HAL_OK;
        }
        
        printf("Error code %d, Failed to mount after formatting.", ffif_res);
    }

    return HAL_ERROR;
}

HAL_StatusTypeDef ffif_unmountSd(void)
{
    ffif_res = f_mount(NULL, (TCHAR const*) ffif_path, 0);
    if(ffif_res == FR_OK) {
        sdCardMounted = false;
        return HAL_OK;
    }

    return HAL_ERROR;
}

HAL_StatusTypeDef ffif_formatSd(void)
{
    ffif_res = f_mkfs(ffif_path, FM_ANY, 0, ffif_readBuffer, sizeof(ffif_readBuffer));
    if (ffif_res != FR_OK)
    {
        printf("Error code %d during formatting.\r\n", ffif_res);
        return HAL_ERROR;
    }

    return HAL_OK;

}

HAL_StatusTypeDef ffif_readFile(char* name, uint8_t* data, uint32_t* dataReadBytes)
{
    ffif_res = f_stat(name, &ffif_fno);
    if (ffif_res != FR_OK)
    {
        printf("%s does not exists\n\r", name);
        return HAL_ERROR;
    }

    ffif_res = f_open(&ffif_file, name, FA_READ);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when opening file %s\n\r", ffif_res, name);
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status = HAL_OK;

    ffif_res = f_read(&ffif_file, data, f_size(&ffif_file), dataReadBytes);
    if (ffif_res != FR_OK) 
    {
        printf("Error code %d when reading file %s\n\r", ffif_res, name);
        status = HAL_ERROR;
    }
    
    ffif_res = f_close(&ffif_file);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when closing file %s\n\r", ffif_res, name);
        status = HAL_ERROR;
    }

    return status;
}

HAL_StatusTypeDef ffif_writeFile(char* name, uint8_t* data)
{
    ffif_res = f_stat(name, &ffif_fno);
    if (ffif_res != FR_OK)
    {
        printf("%s does not exist\n\r", name);
        return HAL_ERROR;
    }

    ffif_res = f_open(&ffif_file, name, FA_WRITE);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when opening file %s\n\r", ffif_res, name);
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status = HAL_OK;
    ffif_res = f_write(&ffif_file, data, sizeOfBuffer(data), &byteswritten);
    if (ffif_res != FR_OK) 
    {
        printf("Error code %d when writing file %s\n\r", ffif_res, name);
        status = HAL_ERROR;
    }

    ffif_res = f_close(&ffif_file);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when closing file %s\n\r", ffif_res, name);
        status = HAL_ERROR;
    }

    return status;
}

HAL_StatusTypeDef ffif_createFile(char* name) 
{
    ffif_res = f_stat(name, &ffif_fno);
    if (ffif_res == FR_OK)
    {
        printf("%s already exists\n\r", name);
        return HAL_ERROR;
    }

    ffif_res = f_open(&ffif_file, name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when creating file %s\n\r", ffif_res, name);
        return HAL_ERROR;
    }

    ffif_res = f_close(&ffif_file);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when closing file %s\n\r", ffif_res, name);
    }

    return HAL_OK;
}

HAL_StatusTypeDef ffif_removeFile(char* name)
{
    ffif_res = f_stat(name, &ffif_fno);
    if (ffif_res != FR_OK)
    {
        printf("%s does not exist\n\r", name);
        return HAL_ERROR;
    }

    ffif_res = f_unlink(name);
    if (ffif_res != FR_OK)
    {
        printf("Error code %d when removing %s file", ffif_res, name);
        return HAL_ERROR;
    }
    
    return HAL_OK;
}
