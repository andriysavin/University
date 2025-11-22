cmake_minimum_required(VERSION 3.10.0)
project(FileSystem VERSION 0.1.0 LANGUAGES C CXX)

# Set a common directory for all binaries
set(BIN_DIR "${CMAKE_SOURCE_DIR}/out/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${BIN_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/Release)

file(GLOB SRC_FILES *.cpp)
add_executable(FileSystem ${SRC_FILES})

set_target_properties(FileSystem PROPERTIES OUTPUT_NAME "FileSystem.out")
set_target_properties(FileSystem PROPERTIES CXX_STANDARD 17)

target_link_options(FileSystem PRIVATE -fsanitize=address)

add_custom_command(TARGET FileSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:FileSystem>/assets"
)