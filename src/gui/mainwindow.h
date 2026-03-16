#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
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


private:
    Ui::MainWindow *ui;
    double storedValue = 0;
    char currentOperator = 0;
    bool waitingForInput = false;
};
#endif // MAINWINDOW_H
