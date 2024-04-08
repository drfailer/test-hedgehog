add_executable(matrix-vector-product
		matrix-vector-product.cc)

target_link_libraries(matrix-vector-product ${Hedgehog_LIBRARIES})
target_include_directories(matrix-vector-product PUBLIC ${Hedgehog_INCLUDE_DIR})
