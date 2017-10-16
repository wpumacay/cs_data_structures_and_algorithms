
#pragma once


#include "../base/LGraphicsObject.h"
#include "../shaders/LShaderManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"

#include <iostream>

using namespace std;

namespace engine
{

	namespace gl
	{

        namespace primitive
        {
            enum _primitive
            {
                PRIMITIVE_POINT = 0,
                PRIMITIVE_LINE = 1,
                PRIMITIVE_TRIANGLE = 2,
                PRIMITIVE_QUAD = 3,
                PRIMITIVE_CIRCLE = 4,
                PRIMITIVE_RECT = 5,
                PRIMITIVE_POINT_SWARM = 6,
                PRIMITIVE_LINE_SWARM = 7
            };
        }

        class LPrimitive : public LGraphicsObject
        {

        	public :

        	primitive::_primitive type;
            
            virtual void initGeometry()
            {
                // Override this
            }

	        virtual void drawGeometry( const LRenderInfo& rInfo, GLuint _programId )
            {
                // Override this
            }

            LPrimitive() : LGraphicsObject()
            {

            }

            virtual void init()
            {
                initGeometry();
                glGenBuffers( 1, &vbo );

                glGenVertexArrays( 1, &vao );
                glBindVertexArray( vao );

                glBindBuffer( GL_ARRAY_BUFFER, vbo );
                glBufferData( GL_ARRAY_BUFFER, 
                              sizeof( GLfloat ) * 3 * m_numVertices,
                              m_vertices, GL_STATIC_DRAW );

                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );
                glEnableVertexAttribArray( 0 );

                glBindVertexArray( 0 );
            }

            virtual bool isDrawable( const LRenderInfo& rInfo )
            {
                if ( !this->visible )
                {
                    return false;
                }

                float _adx = abs( this->xy.x + rInfo.cameraX );
                float _ady = abs( this->xy.y + rInfo.cameraY );

                if ( _adx > rInfo.cameraWidth / 2 ||
                     _ady > rInfo.cameraHeight / 2 )
                {
                    return false;
                }
                return true;
            }

	        void render( const LRenderInfo& rInfo )
	        {
	        	Program& _program = LShaderManager::instance->getProgram( programResIndx );
	        	GLuint _programId = _program.id;

	        	glUseProgram( _programId );

	        	glBindVertexArray( vao );

	        	if ( _programId != 0 )
	        	{
	        		GLuint u_color  = glGetUniformLocation( _programId, "u_color" );
                    GLuint u_tModel = glGetUniformLocation( _programId, "u_tModel" );
                    GLuint u_tView  = glGetUniformLocation( _programId, "u_tView" );
                    GLuint u_tProj  = glGetUniformLocation( _programId, "u_tProj" );

                    glm::mat4 _matModel = glm::mat4( 1.0f );
                    _matModel = glm::scale( _matModel, glm::vec3( this->scale.x, this->scale.y, 1.0f ) );
                    _matModel = glm::rotate( _matModel, this->rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
                    _matModel = glm::translate( _matModel, glm::vec3( this->xy.x, this->xy.y, 0.0f ) );

	        		glUniform4fv( u_color, 1, glm::value_ptr( m_color ) );
                    glUniformMatrix4fv( u_tModel, 1, GL_FALSE, glm::value_ptr( _matModel ) );
                    glUniformMatrix4fv( u_tView, 1, GL_FALSE, glm::value_ptr( rInfo.mat_view ) );
                    glUniformMatrix4fv( u_tProj, 1, GL_FALSE, glm::value_ptr( rInfo.mat_proj ) );

		        	drawGeometry( rInfo, _programId );
	        	}

	        	glBindVertexArray( 0 );

	        	glUseProgram( 0 );
	        }
        };

    }
}