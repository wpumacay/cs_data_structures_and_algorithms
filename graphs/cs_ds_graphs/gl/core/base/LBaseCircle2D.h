
#pragma once

#define GLM_ENABLE_EXPERIMENTAL

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

        class LBaseCircle2D : public LGraphicsObject
        {

            public :

            float radius;

            LBaseCircle2D() : LGraphicsObject()
            {
                radius = 0.4;

                m_numVertices = 1;

                m_vertices = new GLfloat[ 3 * m_numVertices ];
                m_vertices[ 0 * 3 + 0 ] = 0.0f;
                m_vertices[ 0 * 3 + 1 ] = 0.0f;
                m_vertices[ 0 * 3 + 2 ] = 0.0f;

                glGenBuffers( 1, &vbo );

                glGenVertexArrays( 1, &vao );
                glBindVertexArray( vao );

                glBindBuffer( GL_ARRAY_BUFFER, vbo );
                glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * m_numVertices, m_vertices, GL_STATIC_DRAW );

                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void * ) 0 );
                glEnableVertexAttribArray( 0 );

                glBindBuffer( GL_ARRAY_BUFFER, 0 );

                glBindVertexArray( 0 );

                programResIndx = ShaderManager::instance->createProgramAdv( "./engine/gl/shaders/gl_circle_vertex_shader.glsl",
                                                                            "./engine/gl/shaders/gl_circle_fragment_shader.glsl",
                                                                            "./engine/gl/shaders/gl_circle_geometry_shader.glsl" );
            }

            LBaseCircle2D( double x, double y ) : LBaseCircle2D()
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

                // Transform from world to gl
                LPoint2D xy_gl;
                xy_gl.x = ( 1. / rInfo.worldWidth ) * this->xy.x;
                xy_gl.y = ( 1. / rInfo.worldHeight ) * this->xy.y;

                float radius_gl = ( 1. / rInfo.worldWidth ) * this->radius;

                if ( _programId != 0 )
                {
                    GLuint u_transform = glGetUniformLocation( _programId, "u_transform" );
                    //cout << "u_transform::id> " << u_transform << endl;
                    glm::mat4 _mat = glm::mat4( 1.0f );
                    _mat = glm::scale( _mat, glm::vec3( this->scale.x, this->scale.y, 1.0f ) );
                    _mat = glm::rotate( _mat, this->rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
                    _mat = glm::translate( _mat, glm::vec3( xy_gl.x, xy_gl.y, 0.0f ) );

                    //cout << glm::to_string( _mat ) << endl;

                    glUniformMatrix4fv( u_transform, 1, GL_FALSE, glm::value_ptr( _mat ) );

                    GLuint u_cRadius = glGetUniformLocation( _programId, "u_cRadius" );

                    glUniform1f( u_cRadius, radius_gl );
                }
                glDrawArrays( GL_POINTS, 0, 1 );

                glBindVertexArray( 0 );

                glUseProgram( 0 );
            }
        };
    }
}