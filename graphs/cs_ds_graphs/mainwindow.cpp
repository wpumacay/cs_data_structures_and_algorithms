#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <iostream>

#include "gldrawingarea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    QHBoxLayout* _layout_main = new QHBoxLayout;

    m_glDrawingArea = new qt::GLdrawingArea();

    _layout_main->addWidget( m_glDrawingArea );

    QWidget* _centralWidget = new QWidget( this );
    setCentralWidget( _centralWidget );
    _centralWidget->setLayout( _layout_main );

}

void MainWindow::createActions()
{
    m_act_init_nodes = new QAction( tr( "Init &Nodes" ), this );
    connect( m_act_init_nodes, &QAction::triggered, this, &MainWindow::onInitNodes );

    m_act_init_graph = new QAction( tr( "Init &Graph" ), this );
    connect( m_act_init_graph, &QAction::triggered, this, &MainWindow::onInitGraph );

    m_act_place_obstacle = new QAction( tr( "Place &Obstacle" ), this );
    connect( m_act_place_obstacle, &QAction::triggered, this, &MainWindow::onPlaceObstacle );
}

void MainWindow::createMenus()
{
    m_optsMenu = menuBar()->addMenu( tr( "&Options" ) );
    m_optsMenu->addAction( m_act_init_nodes );
    m_optsMenu->addAction( m_act_init_graph );
    m_optsMenu->addAction( m_act_place_obstacle );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onInitNodes()
{
    std::cout << "init nodes" << std::endl;
    m_glDrawingArea->initGraphNodes();
}

void MainWindow::onInitGraph()
{
    std::cout << "init graph" << std::endl;
    m_glDrawingArea->initGraphConnections();
}

void MainWindow::onPlaceObstacle()
{
    std::cout << "place obstacle" << std::endl;
    m_glDrawingArea->placeObstacle();
}

