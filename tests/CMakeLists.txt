CMAKE_MINIMUM_REQUIRED(VERSION 3.16)


INCLUDE(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

FetchContent_MakeAvailable(googletest)
