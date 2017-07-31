
#pragma once

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
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

            QSpinBox* m_sbox_instance_size;
            QComboBox* m_cbox_instance_type;
            QComboBox* m_solver_optimizer_type;
            QComboBox* m_solver_intensifier_type;

            QSpinBox* m_sbox_test_swap_1;
            QSpinBox* m_sbox_test_swap_2;
            QLabel* m_lbl_result;

            LCircleRenderer* m_circleRenderer;

        signals:

        public slots:
            void onInitializeSolver();
            void onInitializeInstance();

            void onTest_Solver();
            void onTest_Potential_U();
            void onTest_Start();
            void onTest_Pause();
            void onTest_Reset();

            void onTest_General();

            void onTest_SwapNeighborhood();
            void onTest_RandomReset();
        };
    }
}
