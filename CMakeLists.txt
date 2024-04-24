cmake_minimum_required(VERSION 3.16)
project(test-hedgehog)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

set(Hedgehog_INCLUDE_DIR ./lib/AtomicHedeghog/)

# added tclap
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/)

# Need to defined variable Hedgehog_INCLUDE_DIR to the hedgehog path
# hedgehog
find_package(Hedgehog REQUIRED)

# OpenBLAS
find_package(OpenBLAS QUIET)

# Cuda
find_package(CUDA QUIET)
find_package(NVTX QUIET)

if (CUDA_FOUND)
	message(STATUS "CUDA found")
	set(CUDA_PROPAGATE_HOST_FLAGS OFF)
	set(CUDA_NVCC_FLAGS_RELEASE -O3; -DNDEBUG)
endif (CUDA_FOUND)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Hedgehog_CXX_FLAGS})

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror -pedantic -g")
endif (NOT MSVC)

#Gather tclap headers
file(GLOB tclap_headers "../../../utils/tclap/*.h")

# Add tutorials target
add_subdirectory(matrix_vector_product)
add_subdirectory(matrix_vector_product_state)
add_subdirectory(game_of_life)
add_subdirectory(gauss)
if (CUDA_FOUND)
    add_subdirectory(gauss_cuda)
endif (CUDA_FOUND)
