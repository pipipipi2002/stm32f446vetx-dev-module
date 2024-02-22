#include <printf.h>
#include <lfs.h>
#include "main.h"
#include "littlefs_if.h"

#define UNUSED(x)               (void)(x)
#define SD_BLOCK_SIZE           (512)
#define LFS_IF_READ_SIZE        (512)
#define LFS_IF_PROG_SIZE        (512)
#define LFS_IF_CACHE_SIZE       (512)
#define LFS_IF_LOOKAHEAD_CACHE_SIZE (512)

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int lfs_erase(const struct lfs_config *c, lfs_block_t block);
int lfs_sync(const struct lfs_config *c);


extern SD_HandleTypeDef hsd;

uint8_t blockData[SD_BLOCK_SIZE];
uint32_t readBuffer[LFS_IF_CACHE_SIZE/sizeof(uint32_t)];
uint32_t programBuffer[LFS_IF_CACHE_SIZE/sizeof(uint32_t)];
uint32_t lookaheadBuffer[LFS_IF_LOOKAHEAD_CACHE_SIZE/sizeof(uint32_t)];

struct lfs_config lfs_cfg =
{
    .read = lfs_read,
    .prog = lfs_prog,
    .erase = lfs_erase,
    .sync = lfs_sync,
    .read_size = LFS_IF_READ_SIZE,
    .prog_size = LFS_IF_PROG_SIZE,
    .block_size = SD_BLOCK_SIZE,
    // .block_count = hsd.SdCard.LogBlockNbr,
    .cache_size = LFS_IF_CACHE_SIZE,
    .lookahead_size = LFS_IF_LOOKAHEAD_CACHE_SIZE,
    .block_cycles = 500,

    .read_buffer = &readBuffer,
    .prog_buffer = &programBuffer,
    .lookahead_buffer = &lookaheadBuffer
};

int lfs_init(lfs_t* lfs) 
{
    if (!lfs_cfg.read_buffer || !lfs_cfg.prog_buffer)
    {
        printf("Buffer error");
        return HAL_ERROR;
    }

    lfs_cfg.block_count = hsd.SdCard.LogBlockNbr;

    int err = lfs_mount(lfs, &lfs_cfg);

    if (err)
    {
        printf("%s, lfs_mount() error, reformatting.\n\r", __func__);
        err = lfs_format(lfs, &lfs_cfg);
        printf("lfs_format status: %d\n\r", err);
        err = lfs_mount(lfs, &lfs_cfg);
        printf("lfs_mount status: %d\n\r", err);
    }
    return err;
}

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    UNUSED(c);
    if (!buffer || !size)
    {
        printf("%s Invalid argument\n\r", __func__);
        return LFS_ERR_INVAL;
    }

    if (off >= SD_BLOCK_SIZE || off < 0)
    {
        printf("Read offset exceed range: %d\n\r", off);
        return LFS_ERR_INVAL;
    }

    if (off + size > SD_BLOCK_SIZE) 
    {
        printf("Requested size exceeds block: %d %d\n\r", off, size);
        return LFS_ERR_INVAL;
    }
    
    // if (HAL_SD_ReadBlocks(&hsd, blockData, block, 1, 5000) != HAL_OK)
    if (HAL_SD_ReadBlocks_DMA(&hsd, blockData, block, 1) != HAL_OK)
    {
        printf("Read SD Card Error\n\r");
        return LFS_ERR_IO;
    }
    
    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);

    memset(blockData, 0, sizeof(blockData));
    memcpy(buffer, blockData + off, size);

    return LFS_ERR_OK;
}

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    UNUSED(c);
    if (off >= SD_BLOCK_SIZE || off < 0)
    {
        printf("Read offset exceed range: %d\n\r", off);
        return LFS_ERR_INVAL;
    }

    if (off + size > SD_BLOCK_SIZE) 
    {
        printf("Requested size exceeds block: %d %d\n\r", off, size);
        return LFS_ERR_INVAL;
    }
    memset(blockData, 0, sizeof(blockData));
    memcpy(blockData + off, buffer, size);

    // volatile int ret_val = HAL_SD_WriteBlocks(&hsd, blockData, block, 1, 5000);
    volatile int ret_val = HAL_SD_WriteBlocks_DMA(&hsd, blockData, block, 1);
    if (ret_val != HAL_OK) 
    {
        printf("Write SD Card Error\n\r");
        return LFS_ERR_IO;
    }

    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);
    
    return LFS_ERR_OK;
}

int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    UNUSED(c);

    if (HAL_SD_Erase(&hsd, block, block) != HAL_OK)
    {
        printf("Erase SD Card Error.\n\r");
        return LFS_ERR_IO;
    }

    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);

    return LFS_ERR_OK;
}

int lfs_sync(const struct lfs_config *c)
{
    UNUSED(c);
    return LFS_ERR_OK;
}