#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "helpwindow.h"
#include <stdexcept>
#include <QRegularExpression>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalculatorApp)
{
    ui->setupUi(this);

    // Connect number buttons
    for (int i = 0; i < 10; i++) {
        QPushButton *btn = findChild<QPushButton*>("btn" + QString::number(i));
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }

    // Connect operator buttons
    QStringList opButtons = {"btnPlus", "btnMinus", "btnMult", "btnDiv", "btnMod", "btnRoot", "btnSqrt", "btnNSqrt", "btnFact"};
    for (const QString &name : opButtons) {
        QPushButton *btn = findChild<QPushButton*>(name);
        if (btn) connect(btn, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    }

    // Connect other buttons
    connect(ui->btnEqual, &QPushButton::clicked, this, &MainWindow::btnEqual);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::btnClear);
    connect(ui->btnDot, &QPushButton::clicked, this, &MainWindow::btnDot);
    connect(ui->btnHelp, &QPushButton::clicked, this, &MainWindow::btnHelp);

    this->setStyleSheet(R"(
    QPushButton {
        background-color: #faf8f5;
        color: #2C2C2A;
        border: 1px solid #e8e2d9;
        border-radius: 25px;
        font-size: 22px;
        padding: 15px;
    }

    QPushButton:hover {
        background-color: #EDE8E0;
    }

    QPushButton:pressed {
        background-color: #D4CFC7;
    }

    QPushButton#btnClear, QPushButton#btnMod,
    QPushButton#btnSqrt, QPushButton#btnFact,
    QPushButton#btnRoot, QPushButton#btnNSqrt {
        background-color: #f0ebe3;
    }

    QPushButton#btnClear:hover, QPushButton#btnMod:hover,
    QPushButton#btnSqrt:hover, QPushButton#btnFact:hover,
    QPushButton#btnRoot:hover, QPushButton#btnNSqrt:hover {
        background-color: #E3DDD5;
    }

    QPushButton#btnClear:pressed, QPushButton#btnMod:pressed,
    QPushButton#btnSqrt:pressed, QPushButton#btnFact:pressed,
    QPushButton#btnRoot:pressed, QPushButton#btnNSqrt:pressed {
        background-color: #D4CEC6;
    }

    QPushButton#btnHelp {
        background-color: #d85a30;
        color: #FFFFFF;
        border-radius: 14px;
        font-size: 14px;
        min-width: 16px;
        min-height: 16px;
        max-width: 16px;
        max-height: 16px;
        padding: 5px;
    }

    QPushButton#btnPlus, QPushButton#btnMinus,
    QPushButton#btnMult, QPushButton#btnDiv {
        background-color: #d85a30;
        color: #FFFFFF;
    }

    QPushButton#btnPlus:hover, QPushButton#btnMinus:hover,
    QPushButton#btnMult:hover, QPushButton#btnDiv:hover,
    QPushButton#btnHelp:hover {
        background-color: #C04E28;
    }

    QPushButton#btnPlus:pressed, QPushButton#btnMinus:pressed,
    QPushButton#btnMult:pressed, QPushButton#btnDiv:pressed,
    QPushButton#btnHelp:pressed {
        background-color: #A84420;
    }

    QPushButton#btnEqual {
        background-color: #993C1D;
        color: #FFFFFF;
    }

    QPushButton#btnEqual:hover {
        background-color: #7A3018;
    }

    QPushButton#btnEqual:pressed {
        background-color: #5A2010;
    }
    QMainWindow {
        background-color: #ffffff;
}
    #display, #displayHistory {
        border: none;
    }

    #displayHistory {
        color: #e8e2d9
    }
)");
}

// Digit buttons click
void MainWindow::onDigitClicked()
{
    QPushButton *btn = (QPushButton*)sender();
    QString digit = btn->text();

    if (ui->display->text() == "0") {
       ui->display->setText(digit);
    } else {
        ui->display->setText(ui->display->text() + digit);
    }
}

// Operators click
void MainWindow::onOperatorClicked()
{
    QPushButton *btn = (QPushButton*)sender();
    QString name = btn->objectName();

    QMap<QString, QString> operatorMap = {
        {"btnPlus",  "+"},
        {"btnMinus", "-"},
        {"btnMult",  "*"},
        {"btnDiv",   "/"},
        {"btnMod",   "%"},
        {"btnRoot",  "r"},
        {"btnNSqrt", "n"},
        {"btnSqrt",  "s"},
        {"btnFact",  "f"}
    };

    // Only works with + for now
    if (name == "btnPlus") {
        ui->display->setText(ui->display->text() + operatorMap.value(name));
    }
}

// Evaluate expression
void MainWindow::btnEqual()
{
    QString expression = ui->display->text();

    try {
        double result = evaluateExpression(expression);

        ui->displayHistory->setText(expression);

        ui->display->setText(QString::number(result));

    } catch (std::exception &e) {
        ui->display->setText("Error: " + QString(e.what()));
    }
}

// Clear both displays
void MainWindow::btnClear() {
    ui->display->setText("0");
    ui->displayHistory->clear();
}

// Add dot in number
void MainWindow::btnDot() {
    QString currentText = ui->display->text();
    static QRegularExpression re("[\\+\\-\\*/%rnfs]");
    int lastOperator = currentText.lastIndexOf(re);

    QString currentNumber = currentText.mid(lastOperator + 1);

    if (!currentNumber.contains('.')) {
        ui->display->setText(currentText + ".");
    }
}

// Display help
void MainWindow::btnHelp() {
    HelpWindow helpWindow(this);
    helpWindow.exec();
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Simple evaluation of expression
// Only works with +
double MainWindow::evaluateExpression(const QString &expr) {
    double result = 0.0;

    QStringList parts = expr.split('+', Qt::SkipEmptyParts);

    for (const QString &part : parts) {
        bool valid;
        double value = part.toDouble(&valid);

        if (valid) {
            result += value;
        } else {
            throw std::runtime_error("Invalid number");
        }
    }
    return result;
}
