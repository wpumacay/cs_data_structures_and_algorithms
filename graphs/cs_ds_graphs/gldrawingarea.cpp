
#pragma once

#include "gldrawingarea.h"

#include <QPainter>
#include <QTimer>
#include <QRectF>
#include <QMouseEvent>
#include <vector>
#include <cstdlib>

#include "graph/LNode.h"
#include "graph/LEdge.h"
#include "graph/LGraph.h"

#define GRAPH_NODES 4

#define DELTA 10

#define GRAPH_AREA_X 100
#define GRAPH_AREA_Y 100

#define RANDOM() ( rand() / ( float )RAND_MAX )

namespace qt
{

    GLdrawingArea::GLdrawingArea( QWidget *parent )
    {
        setFocus();

        m_state = ST_NORMAL;
        m_rectTooltip = nullptr;

        m_bgBrush = QBrush( QColor( 0, 0, 0 ) );
        m_bgPen = QPen( Qt::black );

        m_nodeBrush = QBrush( QColor( 0, 255, 255 ) );
        m_nodePen = QPen( Qt::cyan );

        m_edgeBrush = QBrush( QColor( 255, 255, 255 ) );
        m_edgePen = QPen( Qt::white );

        m_obstacleBrush = QBrush( QColor( 255, 0, 0 ) );
        m_obstaclePen = QPen( Qt::red );

        QTimer* _timer = new QTimer( this );

        connect( _timer, SIGNAL( timeout() ),
                 this, SLOT( nextAnimationFrame() ) );

        _timer->start( 20 );
    }

    void GLdrawingArea::nextAnimationFrame()
    {
        for ( int q = 0; q < this->graph.nodes.size(); q++ )
        {
            DS::LNode<DS::LGraph<int, double> >* _node = this->graph.nodes[q];

            for ( int p = 0; p < _node->edges.size(); p++ )
            {
                DS::LEdge<DS::LGraph<int, double> >* _edge = _node->edges[p];
                DS::LNode<DS::LGraph<int, double> >* _node_from = _edge->nodes[0];
                DS::LNode<DS::LGraph<int, double> >* _node_to = _edge->nodes[1];
                double x1 = _node_from->x;
                double y1 = _node_from->y;
                double x2 = _node_to->x;
                double y2 = _node_to->y;

                double dx = x2 - x1;
                double dy = y2 - y1;
                double dist = sqrt( dx * dx + dy * dy );
                int N = dist / DELTA;

                for ( int o = 0; o < m_obstacles.size(); o++ )
                {
                    QRectF _obs = m_obstacles[o];
                    for ( int s = 0; s < N; s++ )
                    {
                        if ( _obs.contains( x1 + s * DELTA * dx,
                                            y1 + s * DELTA * dy ) )
                        {
                            graph.removeEdge( _edge );
                            break;
                        }
                    }

                }
            }
        }
        update();
    }

    void GLdrawingArea::paintEvent( QPaintEvent *event )
    {
        QPainter _painter;
        _painter.begin( this );
        _painter.eraseRect( 0, 0, width(), height() );

        _painter.save();

        _painter.setBrush( m_bgBrush );
        _painter.setPen( m_bgPen );
        _painter.drawRect( 0, 0, width(), height() );
        _painter.setRenderHint( QPainter::Antialiasing, true );

        for ( int q = 0; q < this->graph.nodes.size(); q++ )
        {
            _painter.setBrush( m_nodeBrush );
            _painter.setPen( m_nodePen );

            DS::LNode<DS::LGraph<int, double> >* _node = this->graph.nodes[q];
            QPointF _pos( _node->x, _node->y );
            _painter.drawEllipse( _pos, 5, 5 );

            _painter.setBrush( m_edgeBrush );
            _painter.setPen( m_edgePen );
            for ( int p = 0; p < _node->edges.size(); p++ )
            {
                DS::LEdge<DS::LGraph<int, double> >* _edge = _node->edges[p];
                DS::LNode<DS::LGraph<int, double> >* _node_from = _edge->nodes[0];
                DS::LNode<DS::LGraph<int, double> >* _node_to = _edge->nodes[1];
                QPointF _p1( _node_from->x, _node_from->y );
                QPointF _p2( _node_to->x, _node_to->y );

                _painter.drawLine( _p1, _p2 );
            }
        }

        _painter.setBrush( m_obstacleBrush );
        _painter.setPen( m_obstaclePen );

        if ( m_rectTooltip != nullptr )
        {
            _painter.drawRect( QRectF( m_rectTooltip->left(),
                                       m_rectTooltip->top(),
                                       m_rectTooltip->width(),
                                       m_rectTooltip->height() ) );
        }

        for ( int q = 0; q < m_obstacles.size(); q++ )
        {
            _painter.drawRect( m_obstacles[q] );
        }

        _painter.restore();
        _painter.end();
    }

    void GLdrawingArea::initGraphNodes()
    {
        this->graph = DS::LGraph<int, double>();

        for ( int q = 0; q < GRAPH_NODES; q++ )
        {
            double _x = RANDOM() * width();
            double _y = RANDOM() * height();
            this->graph.insertNode( q, _x, _y );
        }
    }

    void GLdrawingArea::initGraphConnections()
    {
        for ( int p = 0; p < this->graph.nodes.size(); p++  )
        {
            for ( int q = p + 1; q < this->graph.nodes.size(); q++ )
            {
                this->graph.insertEdge( this->graph.nodes[p],
                                        this->graph.nodes[q],
                                        1.0 );
            }
        }
    }

    void GLdrawingArea::mouseMoveEvent( QMouseEvent* ev )
    {
        if ( m_rectTooltip != nullptr )
        {
            m_rectTooltip->setLeft( ev->x() );
            m_rectTooltip->setTop( ev->y() );

            m_rectTooltip->setWidth( 50 );
            m_rectTooltip->setHeight( 100 );
        }
    }

    void GLdrawingArea::mousePressEvent( QMouseEvent* ev )
    {
        if ( m_state == ST_NORMAL )
        {
            return;
        }
        setMouseTracking( false );

        m_obstacles.push_back( QRectF( m_rectTooltip->left(),
                                       m_rectTooltip->top(),
                                       m_rectTooltip->width(),
                                       m_rectTooltip->height() ) );

        delete m_rectTooltip;
        m_rectTooltip = nullptr;

        m_state = ST_NORMAL;
    }

    void GLdrawingArea::placeObstacle()
    {
        if ( m_state != ST_NORMAL )
        {
            return;
        }
        setMouseTracking( true );
        m_state = ST_PLACING_OBSTACLE;

        m_rectTooltip = new QRectF( 0, 0, 50, 100 );
    }
}
