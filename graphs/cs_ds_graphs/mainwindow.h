#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gldrawingarea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    QAction* m_act_init_nodes;
    QAction* m_act_init_graph;
    QAction* m_act_place_obstacle;
    QAction* m_act_save;
    QAction* m_act_open;
    QAction* m_act_precalculate;

    QMenu* m_optsMenu;

    void createActions();
    void createMenus();

    Ui::MainWindow *ui;

    qt::GLdrawingArea* m_glDrawingArea;

public slots:

    void onInitNodes();
    void onInitGraph();
    void onPlaceObstacle();
    void onSave();
    void onOpen();
    void onPrecalculate();
};

#endif // MAINWINDOW_H
