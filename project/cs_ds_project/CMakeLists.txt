cmake_minimum_required(VERSION 3.5.0)
project( cs_ds_project )

find_package( CUDA )

set( CMAKE_CXX_STANDARD 11 )

include_directories( engine/ itsvnd/ utils/ )

file( GLOB  CUDA_FILES "/itsvnd/intensification/" *.cu)

set( SOURCES 
        engine/gl/LApp.cpp
		engine/LSolver.cpp 
		engine/LConfiguration.cpp
        itsvnd/intensification/LVNDintensifier.cpp
        itsvnd/intensification/cuda_vnd.cu
		itsvnd/neighborhood/Neighborhoods.cpp )

#cuda_add_library( HELPER_CUDA_VND CUDA_FILES )

#cuda_compile( HELPER_CUDA_VND CUDA_FILES )
#cuda_compile( VND_CUDA itsvnd/intensification/LVNDintensifier.cpp ${HELPER_CUDA} )

#add_custom_target( cuda_intensifier 
#                   g++ -std=c++11 
#                   itsvnd/intensification/LVNDintensifier.cpp 
#                   ${HELPER_CUDA_VND} 
#                   itsvnd/neighborhood/Neighborhoods.cpp -I./engine -I./itsvnd -I./utils )

cuda_add_executable( cs_ds_project 
				main.cpp 
                engine/gl/LApp.cpp
                engine/LSolver.cpp
                engine/LConfiguration.cpp
                itsvnd/intensification/LVNDintensifier.cpp
                #${HELPER_CUDA_VND}
                #${VND_CUDA}
                ${CUDA_FILES}
				itsvnd/neighborhood/Neighborhoods.cpp )


link_directories( /usr/lib /usr/lib64 /usr/local/lib )
target_link_libraries( cs_ds_project GL glfw GLEW )