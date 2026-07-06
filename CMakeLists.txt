cmake_minimum_required(VERSION 3.16)

project(Chapter08_InventorySQLite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Chapter08_InventorySQLite
    src/main.cpp
    src/sqlite3.c
)

target_include_directories(Chapter08_InventorySQLite PRIVATE
    include
)

target_compile_definitions(Chapter08_InventorySQLite PRIVATE
    PROJECT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"
)
