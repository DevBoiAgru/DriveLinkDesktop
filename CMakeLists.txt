cmake_minimum_required(VERSION 3.13)
project(DriveLinkDesktop)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


file(
    GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(SOURCES
    src/main.cpp
    src/dl/app/DriveLink.cpp
    src/dl/core/Server.cpp
    src/dl/input/GamepadFeeder.cpp
    src/dl/resources/ResourceManager.cpp
    
    src/dl/ui/button.cpp
    src/dl/ui/buttonIndicator.cpp
    src/dl/ui/cross.cpp
    src/dl/ui/progress.cpp
    src/dl/ui/text.cpp
    
    src/dl/utility/Logger.cpp
)


if(WIN32)
    # Windows specific
    list(APPEND SOURCES
        src/dl/platform/windows/vJoyGamepad.cpp
        src/dl/platform/windows/PlatformUtils.cpp
    )
    
    # Get vJoy library
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bit system
        set(VJOY_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vJoy/lib/x64")
    else()
        # 32 bit system
        set(VJOY_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vJoy/lib/x86")
    endif()

    set(VJOY_LIBRARY "${VJOY_LIB_DIR}/vJoyInterface.lib")
    set(VJOY_DLL "${VJOY_LIB_DIR}/vJoyInterface.dll")
else()
    # Linux specific
    list(APPEND SOURCES
        src/dl/platform/linux/PlatformUtils.cpp
        src/dl/platform/linux/linuxGamepad.cpp
    )
endif()

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# Include vJoy headers and link the vJoy dll and lib for windows
if(WIN32)
    # Include vJoy headers
    target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/vJoy/inc")

    target_link_libraries(${PROJECT_NAME} PRIVATE "${VJOY_LIBRARY}")

    # Copy the vJoy dll next to the exe
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VJOY_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Network)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Copy the assets folder next to the executable
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# Build commands:
# cmake -B build
# cmake --build build
