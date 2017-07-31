cmake_minimum_required(VERSION 3.5.0)
project( cs_ds_project )

set( CMAKE_CXX_STANDARD 11 )

include_directories( engine/ itsvnd/ utils/ )

set( SOURCES 
		engine/LSolver.cpp 
		engine/LConfiguration.cpp
		itsvnd/neighborhood/Neighborhoods.cpp )

add_executable( cs_ds_project 
				main.cpp 
				engine/LSolver.cpp
				engine/LConfiguration.cpp
				itsvnd/neighborhood/Neighborhoods.cpp )