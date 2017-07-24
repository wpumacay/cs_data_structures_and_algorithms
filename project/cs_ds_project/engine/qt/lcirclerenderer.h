
#pragma once

#include <QWidget>
#include "../LConfiguration.h"

#define RANDOM() ( rand() / ( float )RAND_MAX )
#define DRAW_SCALE 2.5
// #define SHOW_INITIALIZATION
// #define USE_PHYSICS_BASED_INITIALIZATION
#define INITIALIZATION_ITERS 5000

namespace engine
{
    namespace qtgp
    {
        class LCircleRenderer : public QWidget
        {
            Q_OBJECT

        private :
            int m_initializationIterations;
            int frameNo;

        public :

            LCircleRenderer( QWidget *parent = nullptr );

            QSize minimumSizeHint() const override;
            QSize sizeHint() const override;

        public slots :

            void nextAnimationFrame();

        protected :

            void paintEvent( QPaintEvent *event ) override;

        };
    }
}
