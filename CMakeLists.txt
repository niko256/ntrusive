CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(IntrusiveList
  DESCRIPTION "Header-only intrusive doubly-linked list"
  LANGUAGES CXX
)

OPTION(INTRUSIVE_LIST_BUILD_TESTS "Build unit tests" ON)
OPTION(INTRUSIVE_LIST_ENABLE_ASAN "Enable AddressSanitizer" OFF)
OPTION(INTRUSIVE_LIST_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------- #

FILE(GLOB HEADERS include/intrusive/*.hpp)

ADD_LIBRARY(intrusive_list INTERFACE)


TARGET_SOURCES(intrusive_list INTERFACE ${HEADERS})

TARGET_INCLUDE_DIRECTORIES(intrusive_list INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

TARGET_COMPILE_FEATURES(intrusive_list INTERFACE cxx_std_20)

# ------------------------------------------------------------- #

if(INTRUSIVE_LIST_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# ------------------------------------------------------------- #

include(GNUInstallDirs)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS intrusive_list
    EXPORT intrusive_list-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT intrusive_list-targets
    FILE intrusive_list-config.cmake
    NAMESPACE intrusive::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/intrusive_list
)

INCLUDE(CMakePackageConfigHelpers)
WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_CURRENT_BINARY_DIR}/intrusive_list-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

INSTALL(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/intrusive_list-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/intrusive_list
)
