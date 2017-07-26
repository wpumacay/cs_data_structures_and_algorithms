

#include "gldrawingarea.h"

#include <QPainter>
#include <QTimer>
#include <QRectF>
#include <QMouseEvent>
#include <vector>
#include <cstdlib>

#include "lib/delaunay/delaunay.h"

#define GRAPH_NODES 100

#define DELTA 10

#define GRAPH_AREA_X 100
#define GRAPH_AREA_Y 100

#define OBS_WIDTH 200
#define OBS_HEIGHT 200

#define RANGE_PICK 5.0

#define RANDOM() ( rand() / ( float )RAND_MAX )

namespace qt
{

    GLdrawingArea::GLdrawingArea( QWidget *parent )
    {
        setFocus();

        m_state = ST_NORMAL;
        m_rectTooltip = nullptr;
        m_pick_node_from = nullptr;
        m_pick_node_to = nullptr;

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

            for ( int s = 0; s < m_obstacles.size(); s++ )
            {
                QRectF _obs = m_obstacles[s];
                if ( _obs.contains( _node->x, _node->y ) )
                {
                    cout << "removed node" << endl;
                    this->graph.removeNode( _node );
                    break;
                }
            }

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
                        if ( _obs.contains( x1 + s * DELTA * dx / dist,
                                            y1 + s * DELTA * dy / dist ) )
                        {
                            cout << "removed edge" << endl;
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
            if ( _node == m_pick_node_from ||
                 _node == m_pick_node_to )
            {
                _painter.setBrush( m_obstacleBrush );
                _painter.setPen( m_obstaclePen );
            }
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
        int _numPoints = this->graph.nodes.size();
        del_point2d_t* _points = new del_point2d_t[_numPoints];
        for ( int q = 0; q < _numPoints; q++ )
        {
            DS::LNode<DS::LGraph<int, double> >* _node = this->graph.nodes[q];
            _points[q].x = _node->x;
            _points[q].y = _node->y;
        }

        delaunay2d_t* _res = delaunay2d_from( _points, _numPoints );
        tri_delaunay2d_t* _tres	= tri_delaunay2d_from( _res );

        for ( int q = 0; q < _tres->num_triangles; q++ )
        {
            int _indxs[3] = {0,0,0};
            for ( int p = 0; p < 3; p++ )
            {
                _indxs[p] = _tres->tris[q * 3 + p];
            }
            this->graph.insertEdge( this->graph.nodes[_indxs[0]],
                                    this->graph.nodes[_indxs[1]],
                                    1.0 );
            this->graph.insertEdge( this->graph.nodes[_indxs[1]],
                                    this->graph.nodes[_indxs[2]],
                                    1.0 );
            this->graph.insertEdge( this->graph.nodes[_indxs[2]],
                                    this->graph.nodes[_indxs[0]],
                                    1.0 );
        }

        delete _res;
        delete _tres;
        delete[] _points;
    }

    void GLdrawingArea::mouseMoveEvent( QMouseEvent* ev )
    {
        if ( m_rectTooltip != nullptr )
        {
            m_rectTooltip->setLeft( ev->x() );
            m_rectTooltip->setTop( ev->y() );

            m_rectTooltip->setWidth( OBS_WIDTH );
            m_rectTooltip->setHeight( OBS_HEIGHT );
        }
    }

    void GLdrawingArea::calculatePath()
    {
        if ( m_pick_node_from == nullptr ||
             m_pick_node_to == nullptr )
        {
            return;
        }


    }

    void GLdrawingArea::mousePressEvent( QMouseEvent* ev )
    {
        if ( m_state == ST_NORMAL )
        {
            if ( m_pick_node_to != nullptr &&
                 m_pick_node_from != nullptr )
            {
                m_pick_node_from = nullptr;
                m_pick_node_to = nullptr;
            }
            for ( int q = 0; q < this->graph.nodes.size(); q++ )
            {
                DS::LNode<DS::LGraph<int, double>>* _node = this->graph.nodes[q];

                double dx = ev->pos().x() - _node->x;
                double dy = ev->pos().y() - _node->y;
                double dist = sqrt( dx * dx + dy * dy );
                if ( dist < RANGE_PICK )
                {
                    if ( m_pick_node_from != nullptr )
                    {
                        m_pick_node_to = _node;
                        calculatePath();
                    }
                    else
                    {
                        m_pick_node_from = _node;
                    }

                    break;
                }
            }
        }
        else if ( m_state == ST_PLACING_OBSTACLE )
        {
            setMouseTracking( false );

            m_obstacles.push_back( QRectF( m_rectTooltip->left(),
                                           m_rectTooltip->top(),
                                           m_rectTooltip->width(),
                                           m_rectTooltip->height() ) );

            delete m_rectTooltip;
            m_rectTooltip = nullptr;

            m_state = ST_NORMAL;
        }
    }

    void GLdrawingArea::placeObstacle()
    {
        if ( m_state != ST_NORMAL )
        {
            return;
        }
        setMouseTracking( true );
        m_state = ST_PLACING_OBSTACLE;

        m_rectTooltip = new QRectF( 0, 0, OBS_WIDTH, OBS_HEIGHT );
    }
}
