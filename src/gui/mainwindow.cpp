#include "mainwindow.h"

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

void MainWindow::onDigitClicked()
{
    QPushButton *btn = (QPushButton*)sender();
    QString digit = btn->text();

    if (ui->display->text() == "0") {
       ui->display->setText(digit);
    } else {
        ui->display->insert(btn->text());
    }
}

void MainWindow::onOperatorClicked()
{
    QPushButton *btn = (QPushButton*)sender();
    QString name = btn->objectName();
    QString currentText = ui->display->text();

    if (name == "btnSqrt" || name == "btnFact") {
        static QRegularExpression binaryOperatorRe("[\\+\\-\\*/%\\^√]$");
        if (currentText.contains(binaryOperatorRe) || currentText.endsWith('.')) {
            return;
        }

        try {
            double val = currentText.toDouble();
            double result = 0;

            if (name == "btnSqrt") {
                result = calculator::math::squareRoot(val);
                ui->displayHistory->setText("√" + currentText);
            } else {
                result = calculator::math::factorial(static_cast<long long>(val));
                ui->displayHistory->setText(currentText + "!");
            }

            ui->display->setText(QString::number(result));

        } catch (const std::exception&) {
            ui->display->setText("Error");
        }
        return;
    }

    QMap<QString, QString> operatorMap = {
        {"btnPlus",  "+"},
        {"btnMinus", "-"},
        {"btnMult",  "*"},
        {"btnDiv",   "/"},
        {"btnMod",   "%"},
        {"btnRoot",  "^"},
        {"btnNSqrt", "√"}
    };

    if (!operatorMap.contains(name)) {
        return;
    }

    const QString token = operatorMap.value(name);

    if (name == "btnMinus") {
        if (currentText == "0") {
            ui->display->setText(token);
        } else {
            ui->display->insert(token);
        }

        return;
    }

    if (currentText == "0" || currentText.endsWith('+') ||
        currentText.endsWith('-') || currentText.endsWith('*') ||
        currentText.endsWith('/') || currentText.endsWith('.') ||
        currentText.endsWith('%') || currentText.endsWith('^') ||
        currentText.endsWith("√")) {
        return;
    }

    ui->display->insert(token);
}

void MainWindow::btnEqual()
{
    QString expression = ui->display->text();

    static QRegularExpression invalidRe("[\\+\\-\\*/%\\^√]$");
    if (expression.contains(invalidRe)) {
        return;
    }

    try {
        double result = evaluateExpression(expression);

        ui->displayHistory->setText(expression);

        ui->display->setText(QString::number(result));

    } catch (std::exception &e) {
        ui->display->setText("Error: " + QString(e.what()));
    }
}

void MainWindow::btnClear() {
    ui->display->setText("0");
    ui->displayHistory->clear();
}

void MainWindow::btnDot() {
    QString currentText = ui->display->text();
    static QRegularExpression re("[\\+\\-\\*/%\\^√]");
    int lastOperator = currentText.lastIndexOf(re);

    QString currentNumber = currentText.mid(lastOperator + 1);

    if (!currentNumber.contains('.') && !currentText.endsWith('!')) {
        ui->display->insert(".");
    }
}

void MainWindow::btnHelp() {
    HelpWindow helpWindow(this);
    helpWindow.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::evaluateExpression(const QString &expr) {
    QString internalExpr = expr;
    internalExpr.replace("√", "n");
    return calculator::math::evaluateExpression(internalExpr.toStdString());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        int number = event->key() - Qt::Key_0;
        QString btnName = "btn" + QString::number(number);
        QPushButton *btn = findChild<QPushButton*>(btnName);
        if (btn) {
            btn->animateClick();
        }
    }

    else {
        switch (event->key()) {
            case Qt::Key_Plus:
                ui->btnPlus->animateClick();
                break;
            case Qt::Key_Minus:
                ui->btnMinus->animateClick();
                break;
            case Qt::Key_Asterisk:
                ui->btnMult->animateClick();
                break;
            case Qt::Key_Slash:
                ui->btnDiv->animateClick();
                break;
            case Qt::Key_Equal:
                ui->btnEqual->animateClick();
                break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                ui->btnEqual->animateClick();
                break;
            case Qt::Key_Backspace:
                ui->display->backspace();
                break;
            case Qt::Key_Escape:
                ui->btnClear->animateClick();
                break;
            case Qt::Key_Period:
            case Qt::Key_Comma:
                ui->btnDot->animateClick();
                break;
            case Qt::Key_F:
                ui->btnFact->animateClick();
                break;
            case Qt::Key_P:
                ui->btnRoot->animateClick();
                break;
            case Qt::Key_S:
                ui->btnSqrt->animateClick();
                break;
            case Qt::Key_N:
                ui->btnNSqrt->animateClick();
                break;
            case Qt::Key_M:
                ui->btnMod->animateClick();
                break;
            default:
                QMainWindow::keyPressEvent(event);
                return;
        }
    }
}
