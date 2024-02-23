#ifndef INC_FATFS_IF_H
#define INC_FATFS_IF_H

#include "main.h"
#include <stdbool.h>
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"          /* defines SD_Driver as external */

extern FRESULT ffif_res;        /* FatFs function common result code */
extern char ffif_path[4];       /* SD logical drive path */
extern FATFS ffif_fatFS;        /* File system object for SD logical drive */
extern FIL ffif_file;           /* File object for SD */
extern FILINFO ffif_fno;        

bool ffif_isDriverInit(void);
bool ffif_isSdMounted(void);
HAL_StatusTypeDef ffif_init(void);
HAL_StatusTypeDef ffif_deinit(void);
HAL_StatusTypeDef ffif_mountSd(void);
HAL_StatusTypeDef ffif_unmountSd(void);
HAL_StatusTypeDef ffif_formatSd(void);
HAL_StatusTypeDef ffif_readFile(char* name, uint8_t* data, uint32_t* dataReadBytes);
HAL_StatusTypeDef ffif_writeFile(char* name, uint8_t* data);
HAL_StatusTypeDef ffif_createFile(char* name) ;
HAL_StatusTypeDef ffif_removeFile(char* name);

#endif