cmake_minimum_required(VERSION 3.12)

project(stm32f4_hal C ASM)

set(STM32F4_HAL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_HAL_Driver/Src)
set(STM32F4_HAL_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_HAL_Driver/Legacy )

set(STM32F4_CMSIS_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32F4xx/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include )

message(STATUS "STM32F4 HAL source file path: ${STM32F4_HAL_SOURCE_DIR}")
message(STATUS "STM32F4 HAL include file path: ${STM32F4_HAL_INCLUDE_DIR}")


set(STM32F4_HAL_SOURCE_FILES 
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_cortex.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_dma_ex.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_dma.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_exti.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_flash_ex.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_flash_ramfunc.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_flash.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_gpio.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_pwr_ex.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_pwr.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_rcc_ex.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_rcc.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_sd.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_tim_ex.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_tim.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal_uart.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_hal.c
    ${STM32F4_HAL_SOURCE_DIR}/stm32f4xx_ll_sdmmc.c )

# FATFS
set(FATFS_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/App/fatfs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/Target/bsp_driver_sd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/Target/fatfs_platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/Target/ffconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/Target/sd_diskio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../Middlewares/Third_party/FatFs/src/diskio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../Middlewares/Third_party/FatFs/src/ff_gen_drv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../Middlewares/Third_party/FatFs/src/ff.c
    )

set(FATFS_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/App/
    ${CMAKE_CURRENT_SOURCE_DIR}/../FATFS/Target/
    ${CMAKE_CURRENT_SOURCE_DIR}/../Middlewares/Third_party/FatFs/src/
)

add_library(${PROJECT_NAME} INTERFACE )
target_sources(${PROJECT_NAME} INTERFACE ${STM32F4_HAL_SOURCE_FILES} ${FATFS_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE 
    ${STM32F4_HAL_INCLUDE_DIR}
    ${STM32F4_CMSIS_INCLUDE_DIR} 
    ${FATFS_INCLUDE_DIR})