cmake_minimum_required(VERSION 3.4.1)

project("droidblaster")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(PNG_STATIC ON)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
add_library(png STATIC IMPORTED)

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/SoundQueue.cpp
        src/Sound.cpp
        src/MoveableBody.cpp
        src/SpriteBatch.cpp
        src/Sprite.cpp
        src/Asteroid.cpp
        src/Ship.cpp
        src/DroidBlaster.cpp
        src/EventLoop.cpp
        src/Log.cpp
        src/manager/InputManager.cpp
        src/manager/SoundManager.cpp
        src/manager/Resource.cpp
        src/manager/PhysicsManager.cpp
        src/manager/TimeManager.cpp
        src/manager/GraphicsManager.cpp
        app/droid-blaster.cpp)

set_target_properties(png PROPERTIES IMPORTED_LOCATION
        ${ANDROID_NDK}/sources/android/libpng-stable/obj/local/${ANDROID_ABI}/libpng.a)
set_target_properties(png PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES z)

include_directories(include)
include_directories(${ANDROID_NDK}/sources/android/libpng-stable/jni)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${ANDROID_NDK}/sources/android/libpng-stable/jni)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenSLES
        GLESv2
        EGL
        android
        log
        native_app_glue
        png)