cmake_minimum_required(VERSION 3.10)
project(shadow_echo)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(raylib REQUIRED raylib)

add_executable(shadow_echo 
    src/main.cpp
    src/M01_Core.cpp
    src/M02_Input.cpp
    src/M03_Physics.cpp
    src/M04_PlayerState.cpp
    src/M05_ShadowRecorder.cpp
    src/M06_ShadowEntity.cpp
    src/M07_MechanicLogic.cpp
    src/M08_MechanicRender.cpp
    src/M09_EnemyAI.cpp
    src/M10_LevelLoader.cpp
    src/M11_RenderBasic.cpp
    src/M12_Animation.cpp
    src/M13_Camera.cpp
    src/M14_UI.cpp
    src/M15_Audio.cpp
)

target_link_libraries(shadow_echo ${raylib_LIBRARIES})
