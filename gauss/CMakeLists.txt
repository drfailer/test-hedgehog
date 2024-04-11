add_executable(gauss
		gauss.cc)

target_link_libraries(gauss ${Hedgehog_LIBRARIES})
target_include_directories(gauss PUBLIC ${Hedgehog_INCLUDE_DIR})

