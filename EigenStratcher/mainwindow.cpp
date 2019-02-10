#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qpainter.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->runTests();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    d.setScale(150.0);
    //d.drawModel(&qp, t);
//    c.test0();
//    c.test(&qp, &d);
    EsCalculatorHelp::testRandom2TriangleWithPainting(&qp, &d);
    qp.end();
}

void MainWindow::runTests() const
{
    EsCalculatorTests esCTests;
    QTest::qExec(&esCTests);
}
