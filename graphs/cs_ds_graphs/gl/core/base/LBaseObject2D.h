
#pragma once

#include "LGraphicsObject.h"
#include "../shaders/LShaderManager.h"
#include "glm/glm.hpp"
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

        class LBaseObject2D : public LGraphicsObject
        {

            public :
            
            LBaseObject2D() : LGraphicsObject()
            {
            }

            LBaseObject2D( double x, double y ) : LBaseObject2D()
            {
                this->xy.x = x;
                this->xy.y = y;
            }

            void init()
            {
                cout << "initialized the baseobj 2D" << endl;
                m_numVertices = 4;

                m_vertices = new GLfloat[ 3 * m_numVertices ];
                m_vertices[ 0 * 3 + 0 ] = 10.0f;
                m_vertices[ 0 * 3 + 1 ] = 10.0f;
                m_vertices[ 0 * 3 + 2 ] = 0.0f;

                m_vertices[ 1 * 3 + 0 ] = 10.0f;
                m_vertices[ 1 * 3 + 1 ] = -10.0f;
                m_vertices[ 1 * 3 + 2 ] = 0.0f;

                m_vertices[ 2 * 3 + 0 ] = -10.0f;
                m_vertices[ 2 * 3 + 1 ] = -10.0f;
                m_vertices[ 2 * 3 + 2 ] = 0.0f;

                m_vertices[ 3 * 3 + 0 ] = -10.0f;
                m_vertices[ 3 * 3 + 1 ] = 10.0f;
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
                //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

                programResIndx = LShaderManager::instance->createProgram( "gl/core/shaders/old/gl_base2d_vertex_shader.glsl",
                                                                          "gl/core/shaders/old/gl_base2d_fragment_shader.glsl" );
                //programResIndx = LShaderManager::instance->createProgram( "gl/core/shaders/primitives/gl_primitive_circle_vertex_shader.glsl",
                //                                                          "gl/core/shaders/primitives/gl_primitive_circle_fragment_shader.glsl" );
            }

            void render( const LRenderInfo& rInfo )
            {
                Program& _program = LShaderManager::instance->getProgram( programResIndx );
                GLuint _programId = _program.id;
                
                glUseProgram( _programId );

                glBindVertexArray( vao );

                //cout << "pid: " << _programId << endl;

                if ( _programId != 0 )
                {

                    //cout << "rendering" << endl;
                    //cout << "x: " << this->xy.x << endl;
                    //cout << "y: " << this->xy.y << endl;
                    //cout << "scale.x: " << this->scale.x << endl;
                    //cout << "scale.y: " << this->scale.y << endl;
                    //cout << "rotation: " << this->rotation << endl;

                    GLuint u_color  = glGetUniformLocation( _programId, "u_color" );
                    GLuint u_tModel = glGetUniformLocation( _programId, "u_tModel" );
                    GLuint u_tView  = glGetUniformLocation( _programId, "u_tView" );
                    GLuint u_tProj  = glGetUniformLocation( _programId, "u_tProj" );

                    glm::mat4 _matModel = glm::mat4( 1.0f );
                    _matModel = glm::scale( _matModel, glm::vec3( this->scale.x, this->scale.y, 1.0f ) );
                    _matModel = glm::rotate( _matModel, this->rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
                    _matModel = glm::translate( _matModel, glm::vec3( this->xy.x, this->xy.y, 0.0f ) );

                    //glm::mat4 _matfoo = glm::mat4( 1.0f );
                    //_matfoo = glm::rotate( _matfoo, this->rotation, glm::vec3( 0.0f, 0.0f, 1.0f ) );
                    //cout << glm::to_string( _matfoo ) << endl;  

                    glUniform4fv( u_color, 1, glm::value_ptr( m_color ) );
                    glUniformMatrix4fv( u_tModel, 1, GL_FALSE, glm::value_ptr( _matModel ) );
                    glUniformMatrix4fv( u_tView, 1, GL_FALSE, glm::value_ptr( rInfo.mat_view ) );
                    glUniformMatrix4fv( u_tProj, 1, GL_FALSE, glm::value_ptr( rInfo.mat_proj ) );

                    //glm::mat4 _mat_mvp = rInfo.mat_proj * rInfo.mat_view * _matModel;
                    //glm::vec4 _c1( m_vertices[0], m_vertices[1], 0.0f, 1.0f );
                    //glm::vec4 _c2( m_vertices[3], m_vertices[4], 0.0f, 1.0f );
                    //glm::vec4 _c3( m_vertices[6], m_vertices[7], 0.0f, 1.0f );
                    //glm::vec4 _c4( m_vertices[9], m_vertices[10], 0.0f, 1.0f );

                    //glm::vec4 _c_c1 = _mat_mvp * _c1;
                    //glm::vec4 _c_c2 = _mat_mvp * _c2;
                    //glm::vec4 _c_c3 = _mat_mvp * _c3;
                    //glm::vec4 _c_c4 = _mat_mvp * _c4;
                    //cout << "vertices in clip space" << endl;
                    //cout << glm::to_string( _c_c1 ) << endl;
                    //cout << glm::to_string( _c_c2 ) << endl;
                    //cout << glm::to_string( _c_c3 ) << endl;
                    //cout << glm::to_string( _c_c4 ) << endl;

                    //cout << glm::to_string( _matModel ) << endl;
                    //cout << glm::to_string( m_color ) << endl;

                    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
                    //glDrawArrays( GL_TRIANGLES, 0, 3 );
                }

                glBindVertexArray( 0 );

                glUseProgram( 0 );
            }
        };
    }
}