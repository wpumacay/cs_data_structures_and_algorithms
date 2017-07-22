
#pragma once

#include <QWidget>
#include <QLabel>
#include "lcirclerenderer.h"

namespace engine
{
    namespace qtgp
    {
        class LWindow : public QWidget
        {
            Q_OBJECT

        public :

            LWindow( QWidget *parent = nullptr );

        private :

            LCircleRenderer* m_circleRenderer;

        signals:

        public slots:
            void onTest_BFGS();
            void onTest_Potential_U();
            void onTest_InstanceSelected( int pIndxSelected );
            void onTest_Start();
            void onTest_Pause();
            void onTest_Reset();

            void onTest_General();

            void onTest_SwapNeighborhood();
        };
    }
}
