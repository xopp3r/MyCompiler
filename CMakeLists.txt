cmake_minimum_required(VERSION 3.28)

set (CMAKE_CXX_STANDARD 20)

project(MicroCC)


# Автоматически собрать все .cpp файлы из папки src/
file(GLOB SOURCES "src/*.cpp")



# if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#     add_compile_options(-fmodules-ts)  # Для GCC
# endif()

# Собрать исполняемый файл
add_executable(main ${SOURCES})