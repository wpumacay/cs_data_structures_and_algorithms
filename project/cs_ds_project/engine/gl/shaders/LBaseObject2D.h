
#pragma once

#include "LGraphicsObject.h"
#include "LShaderManager.h"
#include "glm/glm.hpp"

namespace engine
{

    namespace gp
    {

        class LBaseObject2D : public LGraphicsObject
        {

            public :


            LPoint xy;

            LBaseObject2D() : LGraphicsObject()
            {
                
            }

            LBaseObject2D( double x, double y ) : LGraphicsObject( x, y )
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

                glGenVertexArrays( 1, &vao );
                glBindVertexArray( vao );

                glBindBuffer( GL_ARRAY_BUFFER, vbo );
                glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * m_numVertices, m_vertices, GL_STATIC_DRAW );

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
                glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLuint ) * 3 * m_numTriangles, m_indices, GL_STATIC_DRAW );

                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void * ) 0 );

                glBindBuffer( GL_ARRAY_BUFFER, 0 );

                glBindVertexArray( 0 );
                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

                programId = ShaderManager::instance->createProgram( "shaders/gp_base2d_vertex_shader.glsl",
                                                                    "shaders/gp_base2d_fragment_shader.glsl" );

            }

            void render()
            {
                // Render the object
                glUseProgram( programId );

                glBindVertexArray( vao );

                if ( programId != 0 )
                {
                    GLuint u_transform = glGetUniformLocation( programId, "u_transform" );

                    glm::mat4 _mat = glm::mat4( 1.0f );
                    _mat = glm::scale( _mat, glm::vec3( this->scale.x, this->scale.y, 1.0f ) );
                    _mat = glm::rotate( _mat, this->rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
                    _mat = glm::translate( _mat, glm::vec3( this->xy.x, this->xy.y, 0.0f ) );

                    glUniformMatrix4fv( u_transform, 1, GL_FALSE, glm::value_ptr( _mat ) );
                }
                glDrawElements( GL_TRIANGLES, 0, GL_UNSIGNED_INT, 0 );

                glBindVertexArray( 0 );

                glUseProgram( 0 );
            }
        };
    }



}