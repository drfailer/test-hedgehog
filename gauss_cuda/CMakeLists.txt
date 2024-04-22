add_executable(gauss_cuda
		gauss.cc
        utils.cc)

target_link_libraries(gauss_cuda ${Hedgehog_LIBRARIES})
target_include_directories(gauss_cuda PUBLIC ${Hedgehog_INCLUDE_DIR})

