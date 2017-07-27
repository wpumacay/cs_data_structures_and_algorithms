#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>
#include <iostream>

#include "gldrawingarea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    setMinimumHeight( 900 );
    setMaximumHeight( 900 );
    setMinimumWidth( 825 );
    setMaximumWidth( 825 );

    QVBoxLayout* _layout_main = new QVBoxLayout;

    m_glDrawingArea = new qt::GLdrawingArea();

    QHBoxLayout* _layout_btns = new QHBoxLayout();

    QPushButton* _btn_init_nodes = new QPushButton( "Init nodes" );
    QPushButton* _btn_init_connections = new QPushButton( "Triangulate" );
    QPushButton* _btn_place_obstacle = new QPushButton( "Obstacle" );
    QPushButton* _btn_save = new QPushButton( "Save graph" );
    QPushButton* _btn_open = new QPushButton( "Open graph" );

    connect( _btn_init_nodes, &QPushButton::clicked, this, &MainWindow::onInitNodes );
    connect( _btn_init_connections, &QPushButton::clicked, this, &MainWindow::onInitGraph );
    connect( _btn_place_obstacle, &QPushButton::clicked, this, &MainWindow::onPlaceObstacle );
    connect( _btn_save, &QPushButton::clicked, this, &MainWindow::onSave );
    connect( _btn_open, &QPushButton::clicked, this, &MainWindow::onOpen );

    _layout_btns->addWidget( _btn_init_nodes );
    _layout_btns->addWidget( _btn_init_connections );
    _layout_btns->addWidget( _btn_place_obstacle );
    _layout_btns->addWidget( _btn_save );
    _layout_btns->addWidget( _btn_open );

    _layout_main->addLayout( _layout_btns );
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

    m_act_save = new QAction( tr( "&Save" ), this );
    connect( m_act_save, &QAction::triggered, this, &MainWindow::onSave );

    m_act_open = new QAction( tr( "&Open" ), this );
    connect( m_act_open, &QAction::triggered, this, &MainWindow::onOpen );

    m_act_precalculate = new QAction( tr( "&Precalculate" ), this );
    connect( m_act_precalculate, &QAction::triggered, this, &MainWindow::onPrecalculate );
}

void MainWindow::createMenus()
{
    m_optsMenu = menuBar()->addMenu( tr( "&Options" ) );
    m_optsMenu->addAction( m_act_init_nodes );
    m_optsMenu->addAction( m_act_init_graph );
    m_optsMenu->addAction( m_act_place_obstacle );
    m_optsMenu->addAction( m_act_save );
    m_optsMenu->addAction( m_act_open );
    m_optsMenu->addAction( m_act_precalculate );
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

void MainWindow::onSave()
{
    QString _fileName = QFileDialog::getSaveFileName( this,
                                                      tr( "Save graph" ),
                                                      "./graph.txt",
                                                      tr( "Text files (*.txt)" ) );
    m_glDrawingArea->saveGraph( _fileName );
}


void MainWindow::onOpen()
{
    QString _fileName = QFileDialog::getOpenFileName( this,
                                                      tr( "Open graph" ),
                                                      "./",
                                                      tr( "Text files (*.txt)" ) );
    m_glDrawingArea->openGraph( _fileName );
}


void MainWindow::onPrecalculate()
{
    m_glDrawingArea->precalculate();
}



