cmake_minimum_required(VERSION 3.19)
project(SimplePhotoTransfer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(SimplePhotoTransfer
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/SimplePhotoTransfer.cpp
    src/SimplePhotoTransfer.h
    src/SimplePhotoTransfer.ui
    src/ConfigReader.cpp
    src/ConfigReader.h
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(SimplePhotoTransfer PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(SimplePhotoTransfer
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS SimplePhotoTransfer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SimplePhotoTransfer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
