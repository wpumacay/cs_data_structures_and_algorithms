

#include <QPainter>
#include <QPointF>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

#include "lcirclerenderer.h"
#include "../LSolver.h"
#include "../../itsvnd/neighborhood/Neighborhoods.h"

namespace engine
{
    namespace qtgp
    {
        LCircleRenderer::LCircleRenderer( QWidget *parent )
            : QWidget( parent )
        {
            frameNo = 0;

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

        void LCircleRenderer::nextAnimationFrame()
        {
            frameNo++;
            update();
        }

        void LCircleRenderer::paintEvent( QPaintEvent *event )
        {
            QPainter _painter( this );
            LConfiguration* circleConfiguration = LSolver::instance->configuration();

            _painter.setRenderHint( QPainter::Antialiasing, true );
            _painter.translate( width() / 2, height() / 2 );

            if ( circleConfiguration == nullptr )
            {
                return;
            }

            // Do a check of hits between circles to set an appropiate color
            for ( int i = 0; i < circleConfiguration->size; i++ )
            {
                LCircle& _circle = circleConfiguration->getCircleByIndx( i );

                if ( _circle.hitTest( circleConfiguration->getContainer() ) )
                {
                    _circle.hit = true;
                    circleConfiguration->getContainer().hit = true;
                }

                for ( int j = i + 1; j < circleConfiguration->size; j++ )
                {
                    LCircle& _other = circleConfiguration->getCircleByIndx( j );
                    if ( _circle.hitTest( _other ) )
                    {
                        _circle.hit = true;
                        _other.hit = true;
                    }
                }
            }

            QPen _color_no_hit( QColor( 0, 0, 255, 255 ), 2 );
            QPen _color_hit( QColor( 255, 0, 0, 255 ), 2 );
            QPen _color_txt( QColor( 0, 0, 0, 255 ), 2 );
            _painter.setPen( _color_no_hit );

            for ( int q = 0; q < circleConfiguration->size; q++ )
            {
                LCircle& _circle = circleConfiguration->getCircleByIndx( q );
                QPointF _center( _circle.pos.x * DRAW_SCALE, _circle.pos.y * DRAW_SCALE );
                _painter.setPen( _circle.hit ? _color_hit : _color_no_hit );
                _painter.drawEllipse( _center, _circle.r * DRAW_SCALE, _circle.r * DRAW_SCALE );
                _painter.setPen( _color_txt );
                _painter.drawText( _center, QString::number( _circle.id ) );
            }
            LCircle& _container = circleConfiguration->getContainer();
            QPointF _center( _container.pos.x * DRAW_SCALE, _container.pos.y * DRAW_SCALE );
            _painter.setPen( _container.hit ? _color_hit : _color_no_hit );
            _painter.drawEllipse( _center, _container.r * DRAW_SCALE, _container.r * DRAW_SCALE );

            InsertNeighborhood& _pts = LSolver::instance->p_insNeigh;

            for ( int q = 0; q < _pts.size(); q++ )
            {
                QPointF _center( _pts[q].first.x * DRAW_SCALE, _pts[q].first.y * DRAW_SCALE );
                _painter.setPen( _color_no_hit );
                _painter.drawEllipse( _center, 1 * DRAW_SCALE, 1 * DRAW_SCALE );                
            }
        }
    }

}











