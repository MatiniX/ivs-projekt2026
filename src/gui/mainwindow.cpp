#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "helpwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < 10; i++) {
        QPushButton *btn = findChild<QPushButton*>("btn" + QString::number(i));
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }

    QStringList opButtons = {"btnPlus", "btnMinus", "btnMult", "btnDiv", "btnMod", "btnRoot", "btnSqrt", "btnNSqrt", "btnFact"};
    for (const QString &name : opButtons) {
        QPushButton *btn = findChild<QPushButton*>(name);
        if (btn) connect(btn, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    }

    connect(ui->btnEqual, &QPushButton::clicked, this, &MainWindow::btnEqual);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::btnClear);
    connect(ui->btnDot, &QPushButton::clicked, this, &MainWindow::btnDot);
}

void MainWindow::onDigitClicked()
{
    QPushButton *btn = (QPushButton*)sender();
    QString digit = btn->text();

    if (waitingForInput) {
        ui->display->setText(digit);
        waitingForInput = false;
    } else {
        if (ui->display->text() == "0") {
            ui->display->setText(digit);
        }
        else {
            ui->display->setText(ui->display->text() + digit);
        }
    }
}

void MainWindow::onOperatorClicked()
{
    QPushButton *btn = (QPushButton*)sender();
    QString name = btn->objectName();

    QMap<QString, char> operatorMap = {
        {"btnPlus", '+'},
        {"btnMinus", '-'},
        {"btnMult", '*'},
        {"btnDiv", '/'},
        {"btnMod", '%'},
        {"btnRoot", 'r'},
        {"btnNSqrt", 'n'},
        {"btnSqrt", 's'},
        {"btnFact", 'f'}
    };

    storedValue = ui->display->text().toDouble();
    waitingForInput = true;
    currentOperator = operatorMap.value(name, 0);
}

void MainWindow::btnEqual() {
    //result = ;
}

void MainWindow::btnClear() {
    ui->display->setText("0");
    storedValue = 0;
    currentOperator = 0;
    waitingForInput = false;
}

void MainWindow::btnDot() {
    if (!ui->display->text().contains('.')) {
        ui->display->setText(ui->display->text() + ".");
    }
}

void MainWindow::btnHelp() {
    HelpWindow helpWindow;
    helpWindow.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
