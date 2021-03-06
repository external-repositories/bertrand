cmake_minimum_required(VERSION 3.12)

find_program(CONAN conan)
if(NOT CONAN)
  message(WARNING "Conan not found. Skipping tests")
  return()
endif()

if(CONAN_EXPORTED)
  include(${PROJECT_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
else()
  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
    message(
      STATUS
        "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
      DOWNLOAD
      "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.15/conan.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/conan.cmake")
  endif()

  include(${CMAKE_CURRENT_BINARY_DIR}/conan.cmake)
  conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP CMAKE_TARGETS BUILD
                  missing)
endif()

# Because of the location of conan.cmake the FindCatch2.cmake generated by conan
# is not found on windows using msvc
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})

find_package(Catch2 REQUIRED)

add_executable(bertrand_tests)
target_sources(bertrand_tests PRIVATE src/Catch2Main.cc src/text_tests.cc
               src/bertrand_tests.cc)

target_include_directories(
  bertrand_tests PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_link_libraries(bertrand_tests Catch2::Catch2)

target_compile_features(bertrand_tests PRIVATE cxx_std_17)

target_compile_options(
  bertrand_tests
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:MSVC>:/DBERTRAND_CONTRACTS_ARE_EXCEPTIONS>
    $<$<CXX_COMPILER_ID:MSVC>:/DBERTRAND_ENABLE_CONTRACTS>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Werror>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-pedantic>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-DBERTRAND_CONTRACTS_ARE_EXCEPTIONS>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-DBERTRAND_ENABLE_CONTRACTS>
)

add_executable(failing_contract)
target_compile_features(failing_contract PRIVATE cxx_std_17)
target_sources(failing_contract PRIVATE src/failing_contract.cc)

target_include_directories(
  failing_contract PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

add_test(NAME bertrand_tests COMMAND bertrand_tests)
