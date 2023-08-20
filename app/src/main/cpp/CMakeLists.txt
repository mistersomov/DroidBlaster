cmake_minimum_required(VERSION 3.4.1)

project("droidblaster")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fvisibility=hidden")

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
add_library(${CMAKE_PROJECT_NAME} SHARED
        src/DroidBlaster.cpp
        src/EventLoop.cpp
        src/Log.cpp
        app/droid-blaster.cpp)

include_directories(include)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        native_app_glue)