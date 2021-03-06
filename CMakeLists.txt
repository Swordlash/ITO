cmake_minimum_required(VERSION 3.17)
project(ITO)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(ZLIB REQUIRED)

enable_testing()

if (APPLE)
    set(PKG_CONFIG_EXECUTABLE /opt/homebrew/bin/pkg-config)
endif()

# search for pkg-config
include (FindPkgConfig)
if (NOT PKG_CONFIG_FOUND)
    message (FATAL_ERROR "pkg-config not found")
endif ()

# check for libpng
pkg_check_modules (LIBPNG libpng16 REQUIRED)
if (NOT LIBPNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng16 development libraries installed")
else ()
    include_directories (${LIBPNG_INCLUDE_DIRS})
    link_directories (${LIBPNG_LIBRARY_DIRS})
    link_libraries (${LIBPNG_LIBRARIES})
endif ()

set(CMAKE_CXX_STANDARD 20)
include_directories(${PNG_INCLUDE_DIR} external_libs/png external_libs/Eigen)

# Glob for the project files
add_executable(ITO main.cpp ${LIBPNG_LINK_FLAGS} image.hpp image.cpp crls_pca.hpp crls_pca.cpp logging.hpp test.hpp test_utils.cpp pca.hpp utils.hpp svd_pca.hpp svd_pca.cpp logging.cpp)

add_executable(ITO_test ${LIBPNG_LINK_FLAGS} test.cpp test_utils.cpp test.hpp image.cpp image.hpp crls_pca.cpp crls_pca.hpp pca.hpp utils.hpp svd_pca.hpp svd_pca.cpp logging.cpp)
add_test(ITO_test ITO_test "" ITO_test)