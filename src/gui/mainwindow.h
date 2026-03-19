#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class CalculatorApp;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDigitClicked();

    void onOperatorClicked();

    void btnEqual();

    void btnClear();

    void btnDot();

    void btnHelp();

    double evaluateExpression(const QString &expr);

private:
    Ui::CalculatorApp *ui;
};
#endif // MAINWINDOW_H
