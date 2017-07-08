

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>

#define ERRORLOG_BUFF_LENGTH 512

using namespace std;

namespace engine
{

	namespace gp
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
			~Program();
			GLuint id;

		private :

			Program( const vector<Shader> &pShaders );

		};


	}

}

engine::gp::Shader( GLuint pType, const GLchar* pShaderCode )
{
	m_type = pType;
	id = glCreateShader( m_type );
	glShaderSource( id, 1, &pShaderCode, NULL );
	glCompileShader( id );

	GLint _success;
	GLchar _infoLog[ERRORLOG_BUFF_LENGTH];
	glGetShaderiv( id, GL_COMPILE_STATUS, &_success );

	if ( !_success )
	{
		glGetShaderInfoLog( id, ERRORLOG_BUFF_LENGTH, NULL, _infoLog );
		cout << "Shader::Shader> error compiling " 
			 << ( m_type == GL_VERTEX_SHADER ? "vertex" : 
			 		( m_type == GL_FRAGMENT_SHADER ? "fragment" : "unknown" ) )
			 <<" shader" << endl;
		cout << "COMPILATION_ERROR: " << _infoLog << endl;
	}
}

engine::gp::~Shader()
{

}

engine::gp::Shader engine::gp::Shader::createFromFile( GLuint pType, const GLchar* pPath )
{
	string _shaderStrCode;
	ifstream _shaderFile;

	_shaderFile.exceptions( ifstream::badbit );

	try 
	{
		_shaderFile.open( pPath );
		stringstream _shaderStream;
		_shaderStream << _shaderFile.rdbuf();

		_shaderFile.close();

		_shaderStrCode = _shaderStream.str();
	}
	catch
	{
		cout << "Shader::createFromFile> failed opening the resource file" << endl;
	}

	const CLchar* _shaderCode_cstr = _shaderStrCode.c_str();

	return Shader( pType, _shaderCode_cstr );
}



GLuint engine::gp::Shader::type()
{
	return m_type;
}


engine::gp::Program( const vector<engine::gp::Shader> &pShaders )
{
	id = glCreateProgram();
	for ( unsigned int i = 0; i < pShaders.size(); i++ )
	{
		glAttachShader( id, pShaders[i].id );
	}
	glLinkProgram( id );

	for( unsigned int i = 0; i < pShaders.size(); i++ )
	{
		glDetachShader( id, pShaders[i].id );
		glDeleteShader( pShaders[i].id );
	}

	GLint _success;
	GLchar _infoLog[ERRORLOG_BUFF_LENGTH];

}
