#ifndef GLDRAWINGAREA_H
#define GLDRAWINGAREA_H

#include <QOpenGLWidget>
#include "graph/LGraph.h"
#include "graph/LNode.h"
#include "graph/LEdge.h"

#include <QBrush>
#include <QPen>
#include <vector>



namespace qt
{
    class GLdrawingArea : public QOpenGLWidget
    {
        Q_OBJECT

        private :

        enum _State
        {
            ST_NORMAL,
            ST_PLACING_OBSTACLE
        };

        _State m_state;

        QRectF* m_rectTooltip;

        QBrush m_bgBrush;
        QPen m_bgPen;

        QBrush m_nodeBrush;
        QPen m_nodePen;

        QBrush m_edgeBrush;
        QPen m_edgePen;

        QBrush m_obstacleBrush;
        QPen m_obstaclePen;

        QBrush m_pathBrush;
        QPen m_pathPen;

        std::vector<QRectF> m_obstacles;

        DS::LNode<DS::LGraph<int, double>>* m_pick_node_from;
        DS::LNode<DS::LGraph<int, double>>* m_pick_node_to;
        DS::LNode<DS::LGraph<int, double>>* m_path_node;

        void calculatePath();

        public :

        DS::LGraph<int, double> graph;

        GLdrawingArea( QWidget *parent = nullptr );
        void initGraphNodes();
        void initGraphConnections();
        void placeObstacle();
        void saveGraph( QString pFileName );
        void openGraph( QString pFileName );
        void precalculate();

        void mouseMoveEvent( QMouseEvent* ev ) override;
        void mousePressEvent( QMouseEvent* ev ) override;

        public slots :

        void nextAnimationFrame();

        protected :

        void paintEvent( QPaintEvent* event ) override;
    };
}

#endif // GLDRAWINGAREA_H
