
#include "lcirclerenderer.h"
#include <QPainter>
#include <QPointF>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

namespace engine
{
    namespace qtgp
    {
        LCircleRenderer::LCircleRenderer( QWidget *parent )
            : QWidget( parent )
        {
            frameNo = 0;

            circleConfiguration = nullptr;
            m_initializationIterations = 0;

            setBackgroundRole( QPalette::Base );
            setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        }

        QSize LCircleRenderer::minimumSizeHint() const
        {
            return QSize( 50, 50 );
        }

        QSize LCircleRenderer::sizeHint() const
        {
            return QSize( 180, 180 );
        }

        void LCircleRenderer::initializeFromInstance( circleInstance::_circleInstance pCircleInstance, int pSize = 7 )
        {
            if ( circleConfiguration != nullptr )
            {
                delete circleConfiguration;
            }
            circleConfiguration = new engine::LConfiguration();
            circleConfiguration->size = pSize;

            m_initializationIterations = 0;

            double _containerRadius = 0.0;

            for ( int q = 0; q < pSize; q++ )
            {
                engine::LCircle _circle( 0, 0, 1 );

                switch ( pCircleInstance )
                {

                    case engine::circleInstance::INST_r_i :
                        _circle.r = ( q + 1 );
                    break;

                    case engine::circleInstance::INST_r_sqrt_i :
                        _circle.r = sqrt( q + 1 );
                    break;

                    case engine::circleInstance::INST_r_neg_sqrt_i :
                        _circle.r = pSize / sqrt( q + 1 );
                    break;
                }

                _containerRadius +=_circle.r * _circle.r;
                circleConfiguration->addCircle( _circle );
            }

            _containerRadius = 2 * sqrt( _containerRadius );
            engine::LCircle _cContainer( 0, 0, _containerRadius );
            _cContainer.inInitialization = true;
            _cContainer.isDynamic = false;
            _cContainer.vx = 0;
            _cContainer.vy = 0;
            circleConfiguration->setContainer( _cContainer );

            for ( int q = 0; q < pSize; q++ )
            {
                LCircle& _circle = circleConfiguration->getCircleByIndx( q );

                _circle.inInitialization = true;
                _circle.isDynamic = true;
                _circle.vx = RANDOM() * 0.1;
                _circle.vy = RANDOM() * 0.1;
                double _r = RANDOM() * _containerRadius - _circle.r;
                double _t = RANDOM() * 2 * M_PI;
                _circle.pos.x = _r * cos( _t );
                _circle.pos.y = _r * sin( _t );
            }

            m_initializationIterations = 0;

            #ifndef SHOW_INITIALIZATION
            for( int q = 0; q < INITIALIZATION_ITERS; q++ )
            {
                for ( int p = 0; p < circleConfiguration->size; p++ )
                {
                    LCircle& _circle = circleConfiguration->getCircleByIndx( p );
                    // First, check with container
                    if ( _circle.hitTest( circleConfiguration->getContainer() ) )
                    {
                        _circle.onCollision( circleConfiguration->getContainer() );
                    }

                    // Check now with the other circles
                    for ( int r = p + 1; r < circleConfiguration->size; r++ )
                    {
                        if ( _circle.hitTest( circleConfiguration->getCircleByIndx( r ) ) )
                        {
                            _circle.onCollision( circleConfiguration->getCircleByIndx( r ) );
                        }
                    }
                }
                for ( int s = 0; s < circleConfiguration->size; s++ )
                {
                    ( circleConfiguration->getCircleByIndx( s ) ).onTickPhysics( 0.02 );
                }

                m_initializationIterations++;
            }


            #endif

        }

        void LCircleRenderer::nextAnimationFrame()
        {
            frameNo++;
            update();
        }

        void LCircleRenderer::paintEvent( QPaintEvent *event )
        {
            QPainter _painter( this );
            _painter.setRenderHint( QPainter::Antialiasing, true );
            _painter.translate( width() / 2, height() / 2 );

            if ( circleConfiguration == nullptr )
            {
                return;
            }

            #ifdef SHOW_INITIALIZATION
            if ( m_initializationIterations < INITIALIZATION_ITERS )
            {
                m_initializationIterations++;

                for ( int p = 0; p < circleConfiguration->size; p++ )
                {
                    LCircle& _circle = circleConfiguration->getCircleByIndx( p );
                    // First, check with container
                    if ( _circle.hitTest( circleConfiguration->getContainer() ) )
                    {
                        _circle.onCollision( circleConfiguration->getContainer() );
                    }

                    // Check now with the other circles
                    for ( int r = p + 1; r < circleConfiguration->size; r++ )
                    {
                        if ( _circle.hitTest( circleConfiguration->getCircleByIndx( r ) ) )
                        {
                            _circle.onCollision( circleConfiguration->getCircleByIndx( r ) );
                        }
                    }
                }
                for ( int q = 0; q < circleConfiguration->size; q++ )
                {
                    ( circleConfiguration->getCircleByIndx( q ) ).onTickPhysics( 0.02 );
                }
            }
            #endif

            _painter.setPen( QPen( QColor( 0, 0, 255, 255 ), 3 ) );

            for ( int q = 0; q < circleConfiguration->size; q++ )
            {
                LCircle& _circle = circleConfiguration->getCircleByIndx( q );
                QPointF _center( _circle.pos.x * DRAW_SCALE, _circle.pos.y * DRAW_SCALE );
                _painter.drawEllipse( _center, _circle.r * DRAW_SCALE, _circle.r * DRAW_SCALE );
            }
            LCircle _container = circleConfiguration->getContainer();
            QPointF _center( _container.pos.x * DRAW_SCALE, _container.pos.y * DRAW_SCALE );
            _painter.drawEllipse( _center, _container.r * DRAW_SCALE, _container.r * DRAW_SCALE );
        }
    }

}











