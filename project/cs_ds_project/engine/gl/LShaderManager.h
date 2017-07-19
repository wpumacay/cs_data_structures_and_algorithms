

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
			Program( const vector<Shader> &pShaders );
			~Program();
			GLuint id;
		};


		class ShaderManager
		{

			private :

			std::vector<Program> m_programs;

			ShaderManager();

			public :

			static ShaderManager* instance;
			static void create();
			~ShaderManager();

			GLuint createProgram( const GLchar* pVertexShaderResPath,
							   	  const GLchar* pFragmentShaderResPath );

			GLuint addProgram( Program pProgram );
			Program& getProgram( GLuint pId );
		};


	}

}

engine::gp::Shader::Shader( GLuint pType, const GLchar* pShaderCode )
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

engine::gp::Shader::~Shader()
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


engine::gp::Program::Program( const vector<engine::gp::Shader> &pShaders )
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


engine::gp::Program engine::gp::Program::createProgram( const GLchar* pVertexShaderResPath,
										  				const GLchar* pFragmentShaderResPath )
{
	engine::gp::Program _res_program;

	vector<engine::gp::Shader> _shaders;
	_shaders.push_back( engine::gp::Shader::createFromFile( GL_VERTEX_SHADER, pVertexShaderResPath ) );
	_shaders.push_back( engine::gp::Shader::createFromFile( GL_FRAGMENT_SHADER, pFragmentShaderResPath ) );

	_res_program = engine::Program( _shaders );

	return _res_program;
}

engine::gp::Program::~Program()
{

}

engine::gp::ShaderManager::ShaderManager()
{

}

engine::gp::ShaderManager::~ShaderManager()
{

}

GLuint engine::gp::ShaderManager::addProgram( engine::gp::Program pProgram )
{
	m_programs.push_back( pProgram );
	return m_programs.size() - 1;
}

engine::gp::Program* engine::gp::ShaderManager::getProgram( GLuint pId )
{
	if ( pId >= 0 && pId < m_programs.size() )
	{
		return m_programs[pId];
	}
	return NULL;
}

void engine::gp::ShaderManager::create()
{
	if ( engine::gp::ShaderManager::instance != NULL )
	{
		delete engine::gp::ShaderManager::instance;
	}

	engine::gp::ShaderManager::instance = new engine::gp::ShaderManager;
}


GLuint engine::gp::ShaderManager::createProgram( const GLchar* pVertexShaderResPath,
							   	  				 const GLchar* pFragmentShaderResPath )
{
	engine::gp::Program _program = engine::gp::Program::createProgram( pVertexShaderResPath,
															  pFragmentShaderResPath );
	GLuint _resId = addProgram( _program );
	return _resId;
}
