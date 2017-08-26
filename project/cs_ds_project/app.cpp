

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
				 int pAction, int pMode );

#define APP_WIDTH 800
#define APP_HEIGHT 800


int main()
{

	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	GLFWwindow* _window = glfwCreateWindow( APP_WIDTH, APP_HEIGHT,
											"ITS-VND application",
											NULL, NULL );

	if ( _window == NULL )
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent( _window );

	// Initialize glew
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback( _window, onKeyEvent );

	int _width, _height;
	glfwGetFramebufferSize( _window, &_width, &_height );
	glViewport( 0, 0, _width, _height );

	while ( !glfwWindowShouldClose( _window ) )
	{
		glfwPollEvents();


		glfwSwapBuffers( _window );
	}

	glfwTerminate();

	return 0;
}

void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
				 int pAction, int pMode )
{
	if ( pKey == GLFW_KEY_ESCAPE && pAction == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( pWindow, GL_TRUE );
	}
}

