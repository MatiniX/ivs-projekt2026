#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "helpwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

void MainWindow::btnPlus() {
    //result = mathAdd();
}

void MainWindow::btnMinus() {
    //result = mathMinus();
}

void MainWindow::btnMultiply() {
    //result = mathMultiply();
}

void MainWindow::btnDivision() {
    //result = mathDiv();
}

void MainWindow::btnClear() {
    ui->display->setText("0");
}

void MainWindow::btnHelp() {
    HelpWindow *helpWindow = new HelpWindow(this);
    //helpWindow->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
