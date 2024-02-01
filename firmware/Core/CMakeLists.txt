cmake_minimum_required(VERSION 3.12)

project(STM32F446VETx-core C CXX ASM )

set(CORE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src )
set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Inc )

set(CORE_SOURCE_FILES 
    ${CORE_SOURCE_DIR}/main.c
    ${CORE_SOURCE_DIR}/stm32f4xx_hal_msp.c
    ${CORE_SOURCE_DIR}/stm32f4xx_it.c
    ${CORE_SOURCE_DIR}/system_stm32f4xx.c
    )

add_library(${PROJECT_NAME} INTERFACE )
target_sources(${PROJECT_NAME} INTERFACE ${CORE_SOURCE_FILES} )
target_include_directories(${PROJECT_NAME} INTERFACE ${CORE_INCLUDE_DIR} )
