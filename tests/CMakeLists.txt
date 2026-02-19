# fuck cmake and thank you AI

CMAKE_MINIMUM_REQUIRED(VERSION 3.16)


SET(CPM_DOWNLOAD_VERSION 0.42.1)
SET(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

IF (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
  MESSAGE(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION} ")

  FILE(DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
    ${CPM_DOWNLOAD_LOCATION}
  )
ENDIF()

INCLUDE(${CPM_DOWNLOAD_LOCATION})

# https://github.com/cpm-cmake/CPM.cmake/blob/master/examples/gtest/CMakeLists.txt

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  VERSION 1.17.0
  OPTIONS
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON" # for Windows
)

SET(TEST_SRCS
  unit.cc
)


ADD_EXECUTABLE(ntrusive_tests ${TEST_SRCS})

TARGET_LINK_LIBRARIES(ntrusive_tests
  PRIVATE
    ntrusive::ntrusive
    GTest::gtest_main
)

INCLUDE(GoogleTest)
gtest_discover_tests(ntrusive_tests)
