

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>

#define ERRORLOG_BUFF_LENGTH 512

using namespace std;

#define BASE_SHADER_CIRCLE 0
#define BASE_SHADER_LINE   1

namespace engine
{

	namespace gl
	{


		class Shader
		{

		private :

			GLuint m_type;
			Shader( GLuint pType, const GLchar* pShaderCode );

		public :

			GLuint id;

			static Shader createFromFile( GLuint pType, const GLchar* pPath );
			~Shader();
			GLuint type();

		};


		class Program
		{

		public :

			static Program createProgram( const GLchar* pVertexShaderResPath,
										  const GLchar* pFragmentShaderResPath );

			static Program createProgramAdv( const GLchar* pVertexShaderResPath,
										  	 const GLchar* pFragmentShaderResPath,
										  	 const GLchar* pGeometryShaderResPath );

			Program( const vector<Shader> &pShaders );
			~Program();
			GLuint id;
		};


		class LShaderManager
		{

			private :

			std::vector<Program> m_programs;

			LShaderManager();

			public :

			static LShaderManager* instance;
			static void create();
			~LShaderManager();

			void initialize();

			GLuint createProgram( const GLchar* pVertexShaderResPath,
							   	  const GLchar* pFragmentShaderResPath );

			GLuint createProgramAdv( const GLchar* pVertexShaderResPath,
							   	  	 const GLchar* pFragmentShaderResPath,
								  	 const GLchar* pGeometryShaderResPath );

			GLuint addProgram( Program pProgram );
			Program& getProgram( GLuint pId );

			GLuint loadedShaders[100];
		};


	}

}