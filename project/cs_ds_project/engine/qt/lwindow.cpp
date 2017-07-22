
#include "lwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QMessageBox>

#include <armadillo>

#include "../../itsvnd/optimization/BFGS_optimizer.h"

namespace engine
{
    namespace qtgp
    {
        LWindow::LWindow( QWidget *parent )
            : QWidget(parent)
        {

            QHBoxLayout* _layout_main = new QHBoxLayout;
            QVBoxLayout* _layout_test_options = new QVBoxLayout;

            QPushButton* _btn_test_bfgs = new QPushButton( "Test BFGS" );
            QPushButton* _btn_test_potential = new QPushButton( "Test Potential U" );
            QComboBox* _cb_test_instances = new QComboBox();
            _cb_test_instances->addItem( "r = i" );
            _cb_test_instances->addItem( "r = i^0.5" );
            _cb_test_instances->addItem( "r = i^-0.5" );
            QPushButton* _btn_test_start = new QPushButton( "Start" );
            QPushButton* _btn_test_pause = new QPushButton( "Pause" );
            QPushButton* _btn_test_reset = new QPushButton( "Reset" );
            QPushButton* _btn_test_general = new QPushButton( "Just a test" );
            QPushButton* _btn_test_swap_neighborhood = new QPushButton( "Swap Neighborhood" );

            connect( _btn_test_bfgs, SIGNAL( clicked() ), this, SLOT( onTest_BFGS() ) );
            connect( _btn_test_potential, SIGNAL( clicked() ), this, SLOT( onTest_Potential_U() ) );
            connect( _cb_test_instances, SIGNAL( currentIndexChanged(int) ), this, SLOT( onTest_InstanceSelected( int ) ) );
            connect( _btn_test_start, SIGNAL( clicked() ), this, SLOT( onTest_Start() ) );
            connect( _btn_test_pause, SIGNAL( clicked() ), this, SLOT( onTest_Pause() ) );
            connect( _btn_test_reset, SIGNAL( clicked() ), this, SLOT( onTest_Reset() ) );
            connect( _btn_test_general, SIGNAL( clicked() ), this, SLOT( onTest_General() ) );
            connect( _btn_test_swap_neighborhood, SIGNAL( clicked() ), this, SLOT( onTest_SwapNeighborhood() ) );

            _layout_test_options->addWidget( _btn_test_bfgs );
            _layout_test_options->addWidget( _btn_test_potential );
            _layout_test_options->addWidget( _cb_test_instances );
            _layout_test_options->addWidget( _btn_test_start );
            _layout_test_options->addWidget( _btn_test_pause );
            _layout_test_options->addWidget( _btn_test_reset );
            _layout_test_options->addWidget( _btn_test_general );
            _layout_test_options->addWidget( _btn_test_swap_neighborhood );

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

        void LWindow::onTest_BFGS()
        {

        }

        void LWindow::onTest_Potential_U()
        {
            //double _pot = itsvnd::optimizers::evalGradientDescent( m_circleRenderer->circleConfiguration );
            //if ( _pot <= 0 )
            //if ( true )
            //{
            //    m_circleRenderer->circleConfiguration->getContainer().r -= 0.1;
            //}
            itsvnd::optimizers::BS_GradientDescent( m_circleRenderer->circleConfiguration );
        }

        void LWindow::onTest_InstanceSelected( int pIndxSelected )
        {
            m_circleRenderer->initializeFromInstance( ( engine::circleInstance::_circleInstance ) pIndxSelected, 7 );
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
            arma::mat _x = arma::zeros<arma::mat>( 2, 1 );
            arma::mat _res = arma::zeros<arma::mat>( 2, 1 );

            _x( 0, 0 ) = 10;
            _x( 1, 0 ) = 10;

            itsvnd::optimizers::testGradientDescent( _x, _res );

            int _ret = QMessageBox::information( this, tr( "Gradient calculation" ),
                                                 tr( "Result: " ) + 
                                                    QString::number( _x( 0, 0 ) ) + 
                                                    tr( " " ) + 
                                                    QString::number( _x( 1, 0 ) ) );
        }

        void LWindow::onTest_SwapNeighborhood()
        {
            
        }
    }


}
