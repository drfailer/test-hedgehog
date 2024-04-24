add_subdirectory(${CMAKE_SOURCE_DIR}/gauss_cuda/cuda/)

add_executable(gauss_cuda
		gauss.cc
        utils.cc)

target_compile_definitions(gauss_cuda PUBLIC HH_USE_CUDA)
target_link_libraries(gauss_cuda ${Hedgehog_LIBRARIES} ${CUDA_LIBRARIES} cuda cugauss)
target_link_directories(gauss_cuda PRIVATE ${CMAKE_SOURCE_DIR}/build/gauss_cuda/cuda/)
target_include_directories(gauss_cuda PUBLIC ${Hedgehog_INCLUDE_DIR} ${CUDA_INCLUDE_DIRS})
