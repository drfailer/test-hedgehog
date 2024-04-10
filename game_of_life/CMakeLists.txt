add_executable(game_of_life
		game_of_life.cc
        utils.cc)

target_link_libraries(game_of_life ${Hedgehog_LIBRARIES})
target_include_directories(game_of_life PUBLIC ${Hedgehog_INCLUDE_DIR})

