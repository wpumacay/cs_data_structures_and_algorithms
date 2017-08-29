
#pragma once

#include "LGraphicsObject.h"
#include "LShaderManager.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"

namespace engine
{

    namespace gl
    {

        class LBaseObject2D : public LGraphicsObject
        {

            public :
            
            LBaseObject2D() : LGraphicsObject()
            {
                m_numVertices = 4;

                m_vertices = new GLfloat[ 3 * m_numVertices ];
                m_vertices[ 0 * 3 + 0 ] = 0.1f;
                m_vertices[ 0 * 3 + 1 ] = 0.1f;
                m_vertices[ 0 * 3 + 2 ] = 0.0f;

                m_vertices[ 1 * 3 + 0 ] = 0.1f;
                m_vertices[ 1 * 3 + 1 ] = -0.1f;
                m_vertices[ 1 * 3 + 2 ] = 0.0f;

                m_vertices[ 2 * 3 + 0 ] = -0.1f;
                m_vertices[ 2 * 3 + 1 ] = -0.1f;
                m_vertices[ 2 * 3 + 2 ] = 0.0f;

                m_vertices[ 3 * 3 + 0 ] = -0.1f;
                m_vertices[ 3 * 3 + 1 ] = 0.1f;
                m_vertices[ 3 * 3 + 2 ] = 0.0f;

                m_numTriangles = 2;
                m_indices = new GLuint[ 3 * m_numTriangles ];
                m_indices[ 0 * 3 + 0 ] = 0;
                m_indices[ 0 * 3 + 1 ] = 1;
                m_indices[ 0 * 3 + 2 ] = 3;

                m_indices[ 1 * 3 + 0 ] = 1;
                m_indices[ 1 * 3 + 1 ] = 2;
                m_indices[ 1 * 3 + 2 ] = 3;

                glGenBuffers( 1, &vbo );
                glGenBuffers( 1, &ebo );

                glGenVertexArrays( 1, &vao );
                glBindVertexArray( vao );

                glBindBuffer( GL_ARRAY_BUFFER, vbo );
                glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * m_numVertices, m_vertices, GL_STATIC_DRAW );

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
                glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLuint ) * 3 * m_numTriangles, m_indices, GL_STATIC_DRAW );

                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void * ) 0 );
                glEnableVertexAttribArray( 0 );

                glBindBuffer( GL_ARRAY_BUFFER, 0 );

                glBindVertexArray( 0 );
                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

                programResIndx = ShaderManager::instance->createProgram( "./engine/gl/shaders/gl_base2d_vertex_shader.glsl",
                                                                         "./engine/gl/shaders/gl_base2d_fragment_shader.glsl" );
            }

            LBaseObject2D( double x, double y ) : LBaseObject2D()
            {
                this->xy.x = x;
                this->xy.y = y;
            }

            void render( const LRenderInfo& rInfo )
            {
                //cout << "rendering" << endl;
                //cout << "x: " << this->xy.x << endl;
                //cout << "y: " << this->xy.y << endl;
                //cout << "scale.x: " << this->scale.x << endl;
                //cout << "scale.y: " << this->scale.y << endl;
                //cout << "rotation: " << this->rotation << endl;

                Program& _program = ShaderManager::instance->getProgram( programResIndx );
                GLuint _programId = _program.id;
                // Render the object
                glUseProgram( _programId );

                glBindVertexArray( vao );

                if ( _programId != 0 )
                {
                    GLuint u_transform = glGetUniformLocation( _programId, "u_transform" );
                    //cout << "u_transform::id> " << u_transform << endl;
                    glm::mat4 _mat = glm::mat4( 1.0f );
                    _mat = glm::scale( _mat, glm::vec3( this->scale.x, this->scale.y, 1.0f ) );
                    _mat = glm::rotate( _mat, this->rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
                    _mat = glm::translate( _mat, glm::vec3( this->xy.x, this->xy.y, 0.0f ) );

                    //cout << glm::to_string( _mat ) << endl;

                    glUniformMatrix4fv( u_transform, 1, GL_FALSE, glm::value_ptr( _mat ) );
                }
                glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

                glBindVertexArray( 0 );

                glUseProgram( 0 );
            }
        };
    }
}