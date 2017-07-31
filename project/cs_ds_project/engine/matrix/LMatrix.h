
#pragma once

#include <cassert>
#include <iostream>

using namespace std;

namespace engine
{

    namespace mat
    {
        namespace fill
        {
            enum _fill
            {
                ZEROS,
                ONES,
                EYE
            };
        }


        template<class T>
        class LMat
        {


        private :

            T* m_buff;


            int m_rows;
            int m_cols;


        public :

            LMat( int rows, int cols )
            {
                m_rows = rows;
                m_cols = cols;
                m_buff = new T[rows * cols];

                for ( int p = 0; p < m_rows; p++ )
                {
                    for ( int q = 0; q < m_cols; q++ )
                    {
                        m_buff[p * m_cols + q] = 0;
                    }
                }
            }

            LMat( const LMat<T>& other )
            {
                m_rows = other.m_rows;
                m_cols = other.m_cols;

                m_buff = new T[m_rows * m_cols];

                for ( int p = 0; p < m_rows; p++ )
                {
                    for ( int q = 0; q < m_cols; q++ )
                    {
                        m_buff[p * m_cols + q] = other.m_buff[p * m_cols + q];
                    }
                }
            }

            ~LMat()
            {
                if ( m_buff != nullptr )
                {
                    delete[] m_buff;
                }
                m_buff = nullptr;
            }

            LMat<T> operator= ( const LMat<T>& other )
            {
                delete[] m_buff;

                m_rows = other.m_rows;
                m_cols = other.m_cols;

                m_buff = new T[m_rows * m_cols];

                for ( int p = 0; p < m_rows; p++ )
                {
                    for ( int q = 0; q < m_cols; q++ )
                    {
                        m_buff[p * m_cols + q] = other.at( p, q );
                    }
                }

                return *this;
            }

            LMat<T> operator+ ( const LMat<T>& other )
            {
                assert( m_rows == other.m_rows &&
                        m_cols == other.m_cols );

                LMat<T> _res = other;

                for ( int p = 0; p < m_rows; p++ )
                {
                    for ( int q = 0; q < m_cols; q++ )
                    {
                        _res.set( p, q , m_buff[p * m_cols + q] +
                                         _res.at( p, q ) );
                    }
                }

                return _res;
            }

            LMat<T> operator- ( const LMat<T>& other )
            {
                assert( m_rows == other.m_rows &&
                        m_cols == other.m_cols );

                LMat<T> _res( m_rows, m_cols );

                for ( int p = 0; p < m_rows; p++ )
                {
                    for ( int  q = 0; q < m_cols; q++ )
                    {
                        _res.set( p, q, at( p, q ) - other.at( p, q ) );
                    }
                }

                return _res;
            }

            LMat<T> operator* ( const double &val )
            {
                for ( int p = 0; p < m_rows; p++ )
                {
                    for ( int q = 0; q < m_cols; q++ )
                    {
                        m_buff[p * m_cols + q] *= val;
                    }
                }

                return *this;
            }

            LMat<T> operator* ( const LMat<T>& other )
            {
                assert( m_cols == other.m_rows );

                int N = m_cols;

                int rows = m_rows;
                int cols = other.m_cols;

                LMat<T> _res( m_rows, other.m_cols );

                for ( int p = 0; p < rows; p++ )
                {
                    for ( int q = 0; q < cols; q++ )
                    {
                        _res.m_buff[p * cols + q] = 0;
                        for ( int k = 0; k < N; k++ )
                        {
                            _res.m_buff[p * cols + q] += at( p, k ) * other.m_buff[k * other.m_cols + q];
                        }
                    }
                }

                return _res;
            }

            T& operator() ( int row, int col )
            {
                assert( row < m_rows && row >= 0 &&
                        col < m_cols && col >= 0 );

                return m_buff[row * m_cols + col];
            }

            T at( int row, int col ) const
            {
                return m_buff[m_cols * row + col];
            }

            void set( int row, int col, T val )
            {
                m_buff[m_cols * row + col] = val;
            }

            int rows() const
            {
                return this->m_rows;
            }

            int cols() const
            {
                return this->m_cols;
            }


            void print()
            {
                cout << "{";

                for ( int p = 0; p < m_rows; p++ )
                {
                    cout << endl;

                    for ( int q = 0; q < m_cols; q++ )
                    {
                        cout << " ";
                        cout << at( p, q ) ;
                    }
                }

                cout << endl << "}" << endl;
            }


        // Static matrix-creation methods

            static LMat<T> simple( int rows, int cols, fill::_fill pFillOpt )
            {
                LMat<T> _res( rows, cols );

                for ( int p = 0; p < _res.rows(); p++ )
                {
                    for ( int q = 0; q < _res.cols(); q++ )
                    {
                        if ( pFillOpt == fill::ZEROS )
                        {
                            continue;
                        }
                        else if ( pFillOpt == fill::ONES )
                        {
                            _res.set( p, q, 1 );
                        }
                        else if ( pFillOpt == fill::EYE )
                        {
                            _res.set( p, q, p == q ? 1 : 0 );
                        }
                    }
                }

                return _res;
            }

        };


        typedef LMat<double> LMatD;

    }

}
