cmake_minimum_required(VERSION 3.25)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# project settings
project(TempMonitor VERSION 1.0 LANGUAGES C CXX ASM)

add_subdirectory(grainTempBoard)

message(STATUS "")
message(STATUS "########################################")
message(STATUS "### PROJECT:    " ${PROJECT_NAME})
message(STATUS "### MCU MODEL:  " ${MCU_MODEL})
message(STATUS "### CPU MODEL:  " ${CPU_CORE})
message(STATUS "### VERSION:    " ${PROJECT_VERSION})
message(STATUS "########################################")
message(STATUS "")

# message(STATUS "Przed flagi")
# message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
# message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

include(${HAL_DIR_PATH}/core/${MANUFACTURER}/${MCU_MODEL}/cfg.cmake)
include(${HAL_DIR_PATH}/cmake/toolchain_arm.cmake)
include(${HAL_DIR_PATH}/cmake/compileFlags.cmake)

# message(STATUS "Po flagi")
# message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_C_COMPILER}")

add_definitions(-DDEBUG -D${MCU_MODEL})

#file(GLOB_RECURSE SOURCES
set(SOURCES
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/delay.cpp
        )

#set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/core/ST/${MCU_MODEL}/linker.ld)


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -Wl,-gc-sections \
    -Wl,--print-memory-usage \
    -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map \
    -mcpu=${CPU_CORE} \
    -mthumb \
    -mthumb-interwork \
    -specs=nosys.specs \
    -T ${LINKER_SCRIPT}")

#add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
#add_link_options(-mcpu=${CPU_CORE} -mthumb -mthumb-interwork -specs=nosys.specs)
#add_link_options(-T ${LINKER_SCRIPT})

set(TARGET_NAME ${PROJECT_NAME}.elf)
add_executable(${TARGET_NAME} ${SOURCES} ${LINKER_SCRIPT})

target_link_libraries(${TARGET_NAME} PRIVATE BOARD_IMPLEMENTATION)

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_SOURCE_DIR}/grainTempBoard
        )

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
                 Building ${BIN_FILE}")
