cmake_minimum_required(VERSION 3.5.0)
project( graphApp C CXX )

set(CMAKE_BUILD_TYPE Release)

#find_package( CUDA )
find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( Threads REQUIRED )
#find_package( OpenCL REQUIRED )

find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS_DEBUG "-g -Wno-unused-parameter" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-unused-parameter" )


set( NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE )
set( NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE )
set( NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE )

set(MYPROJ_EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/ext/eigen)
set(NANOGUI_EIGEN_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR} CACHE BOOL " " FORCE)

add_subdirectory( ext/nanogui )
set_property( TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies" )
add_definitions(${NANOGUI_EXTRA_DEFS})

# On top of adding the path to nanogui/include, you may need extras
include_directories( gl/ app/ graph/ lib/ ${GLFW_INCLUDE_DIRS} ${NANOGUI_EXTRA_INCS} ext/nanogui/include/ ${MYPROJ_EIGEN3_INCLUDE_DIR} )
link_directories( /usr/lib/ /usr/lib64/ /usr/local/lib/ )

#file( GLOB  CUDA_FILES "/cuda/robotics/" *.cu)

set( SOURCES 
     gl/LApp.cpp
     gl/core/primitives/LPrimitivesRenderer2D.cpp
     gl/core/shaders/LShaderManager.cpp
     app/LAppGraph.cpp
	 lib/delaunay.c )

#cuda_compile( HELPER_CUDA_ROBOTICS cuda/robotics/LRoboticsCudaHelpers.cu )

#cuda_add_executable( robot2D
#					 main.cpp
#					 gl/LApp.cpp
#					 gl/core/primitives/LPrimitivesRenderer2D.cpp
#					 gl/core/shaders/LShaderManager.cpp
#					 app/graphics/LAppGraphics2D.cpp
#					 app/robotics/LAppRobotics2D.cpp
#					 app/particles2D/LAppParticles2D.cpp
#					 app/robotics/LCommonRobotics2D.cpp
#					 app/robotics/robot/LRobot2D.cpp
#					 app/robotics/robot/LRobotLaserSensor.cpp
#					 opencl/robotics/LopenclHelpers.cpp
#					 app/robotics/robot/localization/LRobotLocalizer.cpp
#					 ${HELPER_CUDA_ROBOTICS} )

add_executable( graphApp
				main.cpp
				gl/LApp.cpp
				gl/core/primitives/LPrimitivesRenderer2D.cpp
				gl/core/shaders/LShaderManager.cpp
				app/LAppGraph.cpp
				lib/delaunay.c )

target_link_libraries( graphApp ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} GLEW::GLEW ${CMAKE_THREAD_LIBS_INIT} nanogui ${NANOGUI_EXTRA_LIBS} )