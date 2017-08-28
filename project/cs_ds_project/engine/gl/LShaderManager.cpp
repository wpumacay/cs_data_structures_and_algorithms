
#include "LShaderManager.h"

#include <iostream>
#include <cstring>

using namespace std;

engine::gl::ShaderManager* engine::gl::ShaderManager::instance = NULL;

engine::gl::Shader::Shader( GLuint pType, const GLchar* pShaderCode )
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

engine::gl::Shader::~Shader()
{

}

engine::gl::Shader engine::gl::Shader::createFromFile( GLuint pType, const GLchar* pPath )
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
	catch ( ... )
	{
		cout << "Shader::createFromFile> failed opening the resource file" << endl;
	}

	const GLchar* _shaderCode_cstr = _shaderStrCode.c_str();

	return Shader( pType, _shaderCode_cstr );
}



GLuint engine::gl::Shader::type()
{
	return m_type;
}


engine::gl::Program::Program( const vector<engine::gl::Shader> &pShaders )
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


engine::gl::Program engine::gl::Program::createProgram( const GLchar* pVertexShaderResPath,
										  				const GLchar* pFragmentShaderResPath )
{

	vector<engine::gl::Shader> _shaders;
	_shaders.push_back( engine::gl::Shader::createFromFile( GL_VERTEX_SHADER, pVertexShaderResPath ) );
	_shaders.push_back( engine::gl::Shader::createFromFile( GL_FRAGMENT_SHADER, pFragmentShaderResPath ) );

	engine::gl::Program _res_program = engine::gl::Program( _shaders );

	return _res_program;
}

engine::gl::Program engine::gl::Program::createProgramAdv( const GLchar* pVertexShaderResPath,
										  				   const GLchar* pFragmentShaderResPath,
										  				   const GLchar* pGeometryShaderResPath )
{

	vector<engine::gl::Shader> _shaders;
	_shaders.push_back( engine::gl::Shader::createFromFile( GL_VERTEX_SHADER, pVertexShaderResPath ) );
	_shaders.push_back( engine::gl::Shader::createFromFile( GL_FRAGMENT_SHADER, pFragmentShaderResPath ) );
	_shaders.push_back( engine::gl::Shader::createFromFile( GL_GEOMETRY_SHADER, pGeometryShaderResPath ) );

	engine::gl::Program _res_program = engine::gl::Program( _shaders );

	return _res_program;
}

engine::gl::Program::~Program()
{

}

engine::gl::ShaderManager::ShaderManager()
{

}

engine::gl::ShaderManager::~ShaderManager()
{

}

GLuint engine::gl::ShaderManager::addProgram( engine::gl::Program pProgram )
{
	m_programs.push_back( pProgram );
	return m_programs.size() - 1;
}

engine::gl::Program& engine::gl::ShaderManager::getProgram( GLuint pId )
{
	if ( pId < 0 && pId >= m_programs.size() )
	{
		throw "engine::gl::ShaderManager::getProgram> program not found!!!";
	}
	return m_programs[pId];
}

void engine::gl::ShaderManager::create()
{
	if ( engine::gl::ShaderManager::instance != NULL )
	{
		delete engine::gl::ShaderManager::instance;
	}

	engine::gl::ShaderManager::instance = new engine::gl::ShaderManager;
}


GLuint engine::gl::ShaderManager::createProgram( const GLchar* pVertexShaderResPath,
							   	  				 const GLchar* pFragmentShaderResPath )
{
	engine::gl::Program _program = engine::gl::Program::createProgram( pVertexShaderResPath,
																  	   pFragmentShaderResPath );
	GLuint _resId = addProgram( _program );
	
	return _resId;
}

GLuint engine::gl::ShaderManager::createProgramAdv( const GLchar* pVertexShaderResPath,
							   	  				    const GLchar* pFragmentShaderResPath,
												    const GLchar* pGeometryShaderResPath )
{
	engine::gl::Program _program = engine::gl::Program::createProgramAdv( pVertexShaderResPath,
																  	      pFragmentShaderResPath,
																  	      pGeometryShaderResPath );
	GLuint _resId = addProgram( _program );
	
	return _resId;
}
