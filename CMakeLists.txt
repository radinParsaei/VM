cmake_minimum_required(VERSION 3.5)

project(VM LANGUAGES CXX C)

option(USE_GMP_LIB "" ON)
option(USE_READLINE "" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)

add_definitions(-DSTATIC_BUILD_MODE)
add_definitions(-DTARGET=${CMAKE_SYSTEM_NAME})
include_directories(Value)
include_directories(libs)
include_directories(.)

if (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    add_link_options(-dead_strip -dead_strip_dylibs -Os)
endif ()
if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    add_link_options(-Wl,--gc-sections)
    add_compile_options(-fdata-sections -ffunction-sections)
endif ()

foreach(X IN LISTS EXTS)
    include("exts/${X}/CMakeLists.txt")
endforeach()

foreach(X IN LISTS LIBS)
    include("libs/${X}/CMakeLists.txt")
endforeach()

message(STATUS ${libsources})

if (USE_READLINE)
    find_package(Readline)
    INCLUDE_DIRECTORIES("${Readline_INCLUDE_DIR}")
    add_definitions(-DUSE_READLINE)
    link_libraries("${Readline_LIBRARY}")
endif()
if (USE_GMP_LIB)
    find_package(GMPXX REQUIRED)
    find_package(GMP REQUIRED)
    INCLUDE_DIRECTORIES("${GMPXX_INCLUDE_DIR}")
    add_definitions(-DUSE_GMP_LIB)
    link_libraries(${GMPXX_LIBRARIES})
    link_libraries(${GMP_LIBRARIES})
    add_executable(VM ${libsources} main.cpp VM.cpp)
    add_executable(repl ${libsources} repl.cpp VM.cpp)
    add_executable(assembler ${libsources} assembler.cpp VM.cpp)
    add_executable(disassembler ${libsources} disassembler.cpp VM.cpp)
    add_executable(mkcc ${libsources} mkcc.cpp VM.cpp)
    add_executable(gen_vm_static_dlcall ${libsources} gen_vm_static_dlcall.cpp VM.cpp)
else()
    include_directories(BigNumber/src/BigNumber)
    add_definitions(-DSTD_INCLUDED)
    add_executable(VM ${libsources} BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/BigNumber.cpp VM.cpp main.cpp)
    add_executable(repl ${libsources} BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/BigNumber.cpp VM.cpp repl.cpp)
    add_executable(assembler ${libsources} BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/BigNumber.cpp VM.cpp assembler.cpp)
    add_executable(disassembler ${libsources} BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/BigNumber.cpp VM.cpp disassembler.cpp)
    add_executable(mkcc ${libsources} BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/BigNumber.cpp VM.cpp mkcc.cpp)
    add_executable(gen_vm_static_dlcall ${libsources} BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/BigNumber.cpp VM.cpp gen_vm_static_dlcall.cpp)
endif()

target_link_libraries(VM ${CMAKE_DL_LIBS})
target_link_libraries(repl ${CMAKE_DL_LIBS})
target_link_libraries(assembler ${CMAKE_DL_LIBS})
target_link_libraries(disassembler ${CMAKE_DL_LIBS})
target_link_libraries(mkcc ${CMAKE_DL_LIBS})
