cmake_minimum_required(VERSION 3.16)
project(ProjectSample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "cmake-build-.*")

add_executable(${PROJECT_NAME} ${SOURCES})

# Собираем все папки и их родительские директории
file(GLOB_RECURSE HEADERS "*.h" "*.hpp")
set(ALL_INCLUDE_DIRS "")

foreach(HEADER ${HEADERS})
    get_filename_component(DIR ${HEADER} DIRECTORY)
    # Добавляем саму папку
    list(APPEND ALL_INCLUDE_DIRS ${DIR})
    # Добавляем все родительские папки до корня
    set(CURRENT ${DIR})
    while(NOT CURRENT STREQUAL ${CMAKE_SOURCE_DIR})
        get_filename_component(PARENT ${CURRENT} DIRECTORY)
        if(NOT PARENT STREQUAL ${CMAKE_SOURCE_DIR})
            list(APPEND ALL_INCLUDE_DIRS ${PARENT})
        endif()
        set(CURRENT ${PARENT})
    endwhile()
endforeach()

# Добавляем корень проекта
list(APPEND ALL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR})

if(ALL_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)
    foreach(DIR ${ALL_INCLUDE_DIRS})
        target_include_directories(${PROJECT_NAME} PRIVATE ${DIR})
    endforeach()
endif()