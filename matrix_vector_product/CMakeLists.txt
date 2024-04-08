add_executable(matrix_vector_product
		matrix_vector_product.cc)

target_link_libraries(matrix_vector_product ${Hedgehog_LIBRARIES})
target_include_directories(matrix_vector_product PUBLIC ${Hedgehog_INCLUDE_DIR})
