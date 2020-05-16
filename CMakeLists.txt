cmake_minimum_required (VERSION 3.10)

project(systemd-manager)

include(FindPkgConfig)

set (CMAKE_CXX_STANDARD 11)

pkg_check_modules(LIBSYSTEMD libsystemd>=211 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(
    GLOB
    SOURCES
    *.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBSYSTEMD_LIBRARIES})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
