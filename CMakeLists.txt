cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME 008_Queues_Timers)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    
    #RTOS sources
    common/ThirdParty/FreeRTOS/croutine.c
    common/ThirdParty/FreeRTOS/event_groups.c
    common/ThirdParty/FreeRTOS/list.c
    common/ThirdParty/FreeRTOS/queue.c
    common/ThirdParty/FreeRTOS/stream_buffer.c
    common/ThirdParty/FreeRTOS/tasks.c
    common/ThirdParty/FreeRTOS/timers.c

    common/ThirdParty/FreeRTOS/portable/port.c
    common/ThirdParty/FreeRTOS/portable/MemMang/heap_4.c

    #SEGGER sources
    common/ThirdParty/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c
    common/ThirdParty/SEGGER/Core/SEGGER_RTT_printf.c
    common/ThirdParty/SEGGER/Core/SEGGER_RTT.c
    common/ThirdParty/SEGGER/Core/SEGGER_SYSVIEW.c
    common/ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c
    common/ThirdParty/SEGGER/Port/segger_uart.c
    
    # Plik asemblera z optymalizacjami dla ARMv7M
    common/ThirdParty/SEGGER/Core/SEGGER_RTT_ASM_ARMv7M.S
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE

    # RTOS include paths
    common/ThirdParty/FreeRTOS/include
    common/ThirdParty/FreeRTOS/portable

    #SEGGER include paths
    common/ThirdParty/SEGGER/Config
    common/ThirdParty/SEGGER/Core
    common/ThirdParty/SEGGER/OS
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
