
#include "lwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include "../LSolver.h"

#ifndef TEST_MAT_LIB
#include <armadillo>
#endif

namespace engine
{
    namespace qtgp
    {
        LWindow::LWindow( QWidget *parent )
            : QWidget(parent)
        {

            QHBoxLayout* _layout_main = new QHBoxLayout;
            QVBoxLayout* _layout_test_options = new QVBoxLayout;

            // Solver stuff ******************************************************
            QLabel* _lb_Solver = new QLabel( "Solver" );
            _layout_test_options->addWidget( _lb_Solver );

            m_solver_optimizer_type = new QComboBox();
            m_solver_optimizer_type->addItem( "BS_Gradient_Descent" );
            m_solver_optimizer_type->addItem( "BS_LBFGS" );
            _layout_test_options->addWidget( m_solver_optimizer_type );

            m_solver_intensifier_type = new QComboBox();
            m_solver_intensifier_type->addItem( "Local_Search" );
            m_solver_intensifier_type->addItem( "VND" );
            m_solver_intensifier_type->addItem( "TS_VND" );
            _layout_test_options->addWidget( m_solver_intensifier_type );

            QPushButton* _btn_initialize_solver = new QPushButton( "InitializeSolver" );
            connect( _btn_initialize_solver, SIGNAL( clicked() ), this, SLOT( onInitializeSolver() ) );
            _layout_test_options->addWidget( _btn_initialize_solver );
            // *******************************************************************

            // Instance stuff ****************************************************
            QLabel* _lb_Instance = new QLabel( "Instance" );
            _layout_test_options->addWidget( _lb_Instance );

            m_cbox_instance_type = new QComboBox();
            m_cbox_instance_type->addItem( "r = i" );
            m_cbox_instance_type->addItem( "r = i^0.5" );
            m_cbox_instance_type->addItem( "r = i^-0.5" );
            _layout_test_options->addWidget( m_cbox_instance_type );

            m_sbox_instance_size = new QSpinBox();
            m_sbox_instance_size->setRange( 5, 50 );
            _layout_test_options->addWidget( m_sbox_instance_size );

            QPushButton* _btn_initialize_instance = new QPushButton( "InitializeInstance" );
            connect( _btn_initialize_instance, SIGNAL( clicked() ), this, SLOT( onInitializeInstance() ) );
            _layout_test_options->addWidget( _btn_initialize_instance );
            // *******************************************************************

            QPushButton* _btn_test_solver_step = new QPushButton( "Test Solver step" );

            QPushButton* _btn_test_potential = new QPushButton( "Test Potential U" );
            QPushButton* _btn_test_start = new QPushButton( "Start" );
            QPushButton* _btn_test_pause = new QPushButton( "Pause" );
            QPushButton* _btn_test_reset = new QPushButton( "Reset" );
            QPushButton* _btn_test_general = new QPushButton( "Just a test" );
            QPushButton* _btn_test_swap_neighborhood = new QPushButton( "Swap Neighborhood" );
            QPushButton* _btn_test_random_reset = new QPushButton( "Random Reset" );

            connect( _btn_test_solver_step, SIGNAL( clicked() ), this, SLOT( onTest_Solver() ) );
            connect( _btn_test_potential, SIGNAL( clicked() ), this, SLOT( onTest_Potential_U() ) );
            connect( _btn_test_start, SIGNAL( clicked() ), this, SLOT( onTest_Start() ) );
            connect( _btn_test_pause, SIGNAL( clicked() ), this, SLOT( onTest_Pause() ) );
            connect( _btn_test_reset, SIGNAL( clicked() ), this, SLOT( onTest_Reset() ) );
            connect( _btn_test_general, SIGNAL( clicked() ), this, SLOT( onTest_General() ) );
            connect( _btn_test_swap_neighborhood, SIGNAL( clicked() ), this, SLOT( onTest_SwapNeighborhood() ) );
            connect( _btn_test_random_reset, SIGNAL( clicked() ), this, SLOT( onTest_RandomReset() ) );

            _layout_test_options->addWidget( _btn_test_solver_step );
            _layout_test_options->addWidget( _btn_test_potential );
            _layout_test_options->addWidget( _btn_test_start );
            _layout_test_options->addWidget( _btn_test_pause );
            _layout_test_options->addWidget( _btn_test_reset );
            _layout_test_options->addWidget( _btn_test_general );
            _layout_test_options->addWidget( _btn_test_swap_neighborhood );
            _layout_test_options->addWidget( _btn_test_random_reset );

            m_sbox_test_swap_1 = new QSpinBox();
            m_sbox_test_swap_1->setRange( 1, 20 );
            _layout_test_options->addWidget( m_sbox_test_swap_1 );
            m_sbox_test_swap_2 = new QSpinBox();
            m_sbox_test_swap_2->setRange( 1, 20 );
            _layout_test_options->addWidget( m_sbox_test_swap_2 );

            m_lbl_result = new QLabel( "res: 0.0" );
            _layout_test_options->addWidget( m_lbl_result );

            QTimer* _timer = new QTimer( this );

            m_circleRenderer = new engine::qtgp::LCircleRenderer();
                
            connect( _timer, SIGNAL( timeout() ),
                     m_circleRenderer, SLOT( nextAnimationFrame() ) );

            _layout_main->addWidget( m_circleRenderer );
            _layout_main->addLayout( _layout_test_options );

            _timer->start( 20 );
            setLayout( _layout_main );

            setWindowTitle( tr( "Circle Packing" ) );
        }

        void LWindow::onInitializeSolver()
        {
            LSolver::createSolver( ( options::optimizer::_optimizer ) m_solver_optimizer_type->currentIndex(),
                                   ( options::intensifier::_intensifier ) m_solver_intensifier_type->currentIndex() );
        }

        void LWindow::onInitializeInstance()
        {
            LSolver::instance->init( ( circleInstance::_circleInstance ) m_cbox_instance_type->currentIndex(),
                                     m_sbox_instance_size->value() );
        }

        void LWindow::onTest_Solver()
        {
            LSolver::instance->step();

            m_lbl_result->setText( tr( "res: " ) + QString::number( LSolver::instance->configuration()->getContainer().r ) );
        }

        void LWindow::onTest_Potential_U()
        {
        }

        void LWindow::onTest_Start()
        {
        }

        void LWindow::onTest_Pause()
        {
        }

        void LWindow::onTest_Reset()
        {
        }

        void LWindow::onTest_General()
        {
            LSolver::instance->test_optimization();
            m_lbl_result->setText( tr( "res: " ) + QString::number( LSolver::instance->configuration()->getContainer().r ) );
        }

        void LWindow::onTest_SwapNeighborhood()
        {
            LSolver::instance->test_swap( m_sbox_test_swap_1->value() - 1,
                                          m_sbox_test_swap_2->value() - 1 );
            m_lbl_result->setText( tr( "res: " ) + QString::number( LSolver::instance->configuration()->getContainer().r ) );
        }

        void LWindow::onTest_RandomReset()
        {
            LSolver::instance->test_random_reset();
            m_lbl_result->setText( tr( "res: " ) + QString::number( LSolver::instance->configuration()->getContainer().r ) );
        }
    }


}
