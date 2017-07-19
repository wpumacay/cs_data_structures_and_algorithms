
#pragma once

#include <QWidget>
#include "../LConfiguration.h"

#define RANDOM() ( rand() / ( float )RAND_MAX )
#define DRAW_SCALE 10
// #define SHOW_INITIALIZATION
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

            LConfiguration* circleConfiguration;

            LCircleRenderer( QWidget *parent = nullptr );

            QSize minimumSizeHint() const override;
            QSize sizeHint() const override;
            void initializeFromInstance( circleInstance::_circleInstance pCircleInstance, int size );

        public slots :

            void nextAnimationFrame();

        protected :

            void paintEvent( QPaintEvent *event ) override;

        };
    }
}
