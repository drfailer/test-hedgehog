add_executable(gauss_cuda
		gauss.cc
        utils.cc)

target_compile_definitions(gauss_cuda PUBLIC HH_USE_CUDA)
target_link_libraries(gauss_cuda ${Hedgehog_LIBRARIES} ${CUDA_LIBRARIES} cuda)
target_include_directories(gauss_cuda PUBLIC ${Hedgehog_INCLUDE_DIR} ${CUDA_INCLUDE_DIRS})
