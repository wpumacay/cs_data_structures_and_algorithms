

#include "gldrawingarea.h"

#include <QPainter>
#include <QTimer>
#include <QRectF>
#include <QMouseEvent>
#include <vector>
#include <cstdlib>
#include <QByteArray>
#include <fstream>
#include <map>
#include <queue>

#include "lib/delaunay.h"

// *********************************************************
// NUMBER OF RANDOM NODES IN THE GRAPH
#define GRAPH_NODES 100
// PARAMETER FOR THE COLLISION DETECTION OF OBSTACLE-EDGE
#define DELTA 10
// OBSTACLE DIMENSIONS
#define OBS_WIDTH 200
#define OBS_HEIGHT 200
// GRID SIZE
#define GRID_X 10
#define GRID_Y 10
// PARAMETER FOR THE COLLISION DETECTION WITH A NODE
#define RANGE_PICK 5.0
// *********************************************************

#define RANDOM() ( rand() / ( float )RAND_MAX )

using namespace std;

namespace qt
{

    GLdrawingArea::GLdrawingArea( QWidget *parent )
    {
        setFocus();

        setMinimumHeight( 800 );
        setMaximumHeight( 800 );
        setMinimumWidth( 800 );
        setMaximumWidth( 800 );

        m_state = ST_NORMAL;
        m_rectTooltip = nullptr;
        m_pick_node_from = nullptr;
        m_pick_node_to = nullptr;
        m_path_node = nullptr;

        // Colors to use **********************************
        m_bgBrush = QBrush( QColor( 0, 0, 0 ) );
        m_bgPen = QPen( Qt::black );

        m_nodeBrush = QBrush( QColor( 0, 255, 255 ) );
        m_nodePen = QPen( Qt::cyan );

        m_edgeBrush = QBrush( QColor( 255, 255, 255 ) );
        m_edgePen = QPen( Qt::white );

        m_obstacleBrush = QBrush( QColor( 255, 0, 0 ) );
        m_obstaclePen = QPen( Qt::red );

        m_pathBrush = QBrush( QColor( 0, 255, 0 ) );
        m_pathPen = QPen( Qt::green, 5 );
        // *************************************************

        QTimer* _timer = new QTimer( this );

        connect( _timer, SIGNAL( timeout() ),
                 this, SLOT( nextAnimationFrame() ) );

        _timer->start( 20 );
    }

    void GLdrawingArea::nextAnimationFrame()
    {
        // Check if obstacle contains a node or edges, and delete them accordingly ****************
        for ( int q = 0; q < this->graph.nodes.size(); q++ )
        {
            // Check with nodes
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
            // Check with edges 
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
        // ******************************************************************************************
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

        double delta_x = width() / (double)GRID_X;
        double delta_y = height() / (double)GRID_Y;

        _painter.setBrush( m_obstacleBrush );
        _painter.setPen( m_obstaclePen );
        // Draw grid *******************************
        for ( int x = 0; x < GRID_X ; x++ )
        {
            _painter.drawLine( delta_x * x, 0, delta_x * x, height() );
        }

        for ( int y = 0; y < GRID_Y ; y++ )
        {
            _painter.drawLine( 0, delta_y * y, width(), delta_y * y );
        }
        // ******************************************

        // Draw nodes and edges *********************
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
            _painter.drawText( QPointF( _pos.x() + 2.5, _pos.y() + 2.5 ), QString::number( _node->id ) );

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
        // ******************************************

        // Draw obstacles ***************************
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
        // ******************************************

        // Draw a path requested using A* search ****
        if ( m_path_node != nullptr )
        {
            _painter.setBrush( m_pathBrush );
            _painter.setPen( m_pathPen );

            DS::LNode<DS::LGraph<int,double>>* _node = m_path_node;
            DS::LNode<DS::LGraph<int,double>>* _node_parent = m_path_node->parent;
            while( _node_parent != nullptr )
            {
                _painter.drawLine( _node->x, _node->y, _node_parent->x, _node_parent->y );
                _node = _node_parent;
                _node_parent = _node_parent->parent;
            }
        }
        // ******************************************
        _painter.restore();
        _painter.end();
    }

    // Initialize the nodes randomly in the scene *****
    void GLdrawingArea::initGraphNodes()
    {
        m_path_node = nullptr;
        this->graph = DS::LGraph<int, double>();

        for ( int q = 0; q < GRAPH_NODES; q++ )
        {
            double _x = RANDOM() * width();
            double _y = RANDOM() * height();
            this->graph.insertNode( q, _x, _y, q );
        }
    }
    // ************************************************

    // Initialize the connections using the delaunay triangulation ****************
    void GLdrawingArea::initGraphConnections()
    {
        m_path_node = nullptr;
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
    // *******************************************************************************

    // Mouse move event received ****************************
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
    // *******************************************************

    // Mouse press received when placing an obstacle or selecting nodes ******************
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
                        if ( m_pick_node_from == _node )
                        {
                            continue;
                        }
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
    // ******************************************************************************

    // Callback called when requesting an obstacle **********************************
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
    // ******************************************************************************

    // Callback called when requesting to save the graph in a .txt file ****************************
    void GLdrawingArea::saveGraph( QString pFileName )
    {
        QByteArray _fileName_bytes = pFileName.toLatin1();

        ofstream _fileHandle ( _fileName_bytes.data() );
        if ( _fileHandle.is_open() )
        {
            for ( int q = 0; q < this->graph.nodes.size(); q++ )
            {
                DS::LNode<DS::LGraph<int,double>>* _node = this->graph.nodes[q];
                double _x = _node->x;
                double _y = _node->y;
                int _id = _node->id;

                std::vector<int> _neighIds;

                for ( int s = 0; s <_node->edges.size(); s++ )
                {
                    DS::LEdge<DS::LGraph<int,double>>* _edge = _node->edges[s];
                    DS::LNode<DS::LGraph<int,double>>* _node_to = _edge->nodes[1];
                    _neighIds.push_back( _node_to->id );
                }

                _fileHandle << _x << "," << _y << "," << _id << "," << _neighIds.size() << ",";
                for ( int s = 0; s < _neighIds.size(); s++ )
                {
                    _fileHandle << _neighIds[s] << ",";
                }
                _fileHandle << "\n";
            }

            _fileHandle.close();
        }
    }
    // ************************************************************************************************

    // Callback called when requesting to open a graph from a .txt file *******************************
    void GLdrawingArea::openGraph( QString pFileName )
    {
        QByteArray _fileName_bytes = pFileName.toLatin1();

        ifstream _fileHandle ( _fileName_bytes.data() );

        std::string _line;
        std::vector<std::string> _lines;

        if ( _fileHandle.is_open() )
        {
            while( std::getline( _fileHandle, _line ) )
            {
                _lines.push_back( _line );
            }
            _fileHandle.close();
        }

        m_path_node = nullptr;
        this->graph = DS::LGraph<int, double>();
        std::vector<std::vector<int>> _adjVector;

        for ( int q = 0; q < _lines.size(); q++ )
        {
            std::string _node_line = _lines[q];
            int _pos = _node_line.find( "," );
            double _x = std::stod( _node_line.substr( 0, _pos ) );
            _node_line.replace( 0, _pos + 1, "" );
            _pos = _node_line.find( "," );
            double _y = std::stod( _node_line.substr( 0, _pos ) );
            _node_line.replace( 0, _pos + 1, "" );
            _pos = _node_line.find( "," );
            int _id = std::stoi( _node_line.substr( 0, _pos ) );
            _node_line.replace( 0, _pos + 1, "" );
            _pos = _node_line.find( "," );
            int _num_neighbors = std::stoi( _node_line.substr( 0, _pos ) );
            _node_line.replace( 0, _pos + 1, "" );
            std::vector<int> _neighIds;
            for ( int s = 0; s < _num_neighbors; s++ )
            {
                _pos = _node_line.find( "," );
                _neighIds.push_back( std::stoi( _node_line.substr( 0, _pos ) ) );
                _node_line.replace( 0, _pos + 1, "" );
            }
            _adjVector.push_back( _neighIds );

            this->graph.insertNode( q, _x, _y, _id );
        }

        for ( int q = 0; q < _adjVector.size(); q++ )
        {
            std::vector<int> _neighbors = _adjVector[q];
            for ( int p = 0; p < _neighbors.size(); p++ )
            {
                DS::LNode<DS::LGraph<int,double>>* _node_from = this->graph.nodes[q];
                DS::LNode<DS::LGraph<int,double>>* _node_to = this->graph.nodes[_neighbors[p]];
                double dx = _node_from->x - _node_to->x;
                double dy = _node_from->y - _node_to->y;
                double dist = sqrt( dx * dx + dy * dy );
                this->graph.insertEdge( _node_from,
                                        _node_to,
                                        dist );
            }
        }
    }
    // ************************************************************************************************

    /**
     * Calculate path using A* search *****************************************************************
     */
    void GLdrawingArea::calculatePath()
    {
        if ( m_pick_node_from == nullptr ||
             m_pick_node_to == nullptr )
        {
            return;
        }

        map<int,DS::LNode<DS::LGraph<int,double>>* > _explored;
        vector<DS::LNode<DS::LGraph<int,double>>* > _toExplore;
        _toExplore.push_back( m_pick_node_from );

        // Calculate the first heuristic value
        double _dx = m_pick_node_from->x - m_pick_node_to->x;
        double _dy = m_pick_node_from->y - m_pick_node_to->y;
        double _h = sqrt( _dx * _dx + _dy * _dy );
        m_pick_node_from->g = 0;
        m_pick_node_from->h = _h;
        m_pick_node_from->f = _h;
        m_pick_node_from->parent = nullptr;

        bool found = false;
        DS::LNode<DS::LGraph<int, double>>* _pathNode = nullptr;

        while ( !_toExplore.empty() )
        {
            int _bestIndx = 0;
            for ( int q = 1; q < _toExplore.size(); q++ )
            {
                DS::LNode<DS::LGraph<int,double>>* _nodeFrom = _toExplore[q];
                if ( _nodeFrom->f < _toExplore[_bestIndx]->f )
                {
                    _bestIndx = q;
                }
            }
            DS::LNode<DS::LGraph<int,double>>* _nextToExplore = _toExplore[_bestIndx];
            _toExplore.erase( _toExplore.begin() + _bestIndx );
            // Expand this node
            _explored[_nextToExplore->id] = _nextToExplore;

            for ( int q = 0; q < _nextToExplore->edges.size(); q++ )
            {
                DS::LEdge<DS::LGraph<int,double>>* _edge = _nextToExplore->edges[q];
                DS::LNode<DS::LGraph<int,double>>* _successor = _edge->nodes[1];

                if ( _explored.find( _successor->id ) != _explored.end() )
                {
                    // Already explored, don't count it
                    continue;
                }

                _successor->parent = _nextToExplore;

                if ( _successor == m_pick_node_to )
                {
                    found = true;
                    _pathNode = _successor;
                    break;
                }

                double dx = _successor->x - m_pick_node_to->x;
                double dy = _successor->y - m_pick_node_to->y;
                double dist = sqrt( dx * dx + dy * dy );
                _successor->g = _nextToExplore->g + _edge->data;
                _successor->h = dist;
                _successor->f = _successor->g + _successor->h;

                for ( int s = 0; s < _toExplore.size(); s++ )
                {
                    if ( _successor->id == _toExplore[s]->id )
                    {
                        continue;
                    }
                }

                _toExplore.push_back( _successor );
            }

            if ( found )
            {
                break;
            }
        }

        if ( _pathNode != nullptr )
        {
            m_path_node = _pathNode;
        }
    }
    // ************************************************************************************************

    // TODO: Precalc method
    void GLdrawingArea::precalculate()
    {

    }
}











