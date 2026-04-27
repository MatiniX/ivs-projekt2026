#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./ui_mainwindow.h"
#include "helpwindow.h"
#include "calculator/math.hpp"
#include <QMap>
#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>
#include <stdexcept>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class CalculatorApp;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Main window of calculator app
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief Filters events for the display to capture keyboard input
     * @param obj The object that sent the event
     * @param event The event being processed
     * @return True if the event was handled, false otherwise
     */
    bool eventFilter(QObject *obj, QEvent *event);

    /**
     * @brief Handles clicking on number buttons (0-9)
     */
    void onDigitClicked();

    /**
     * @brief Handles clicks on operators (+, -, *, etc.)
     */
    void onOperatorClicked();

    /**
     * @brief Calculates the result of the displayed expression
     */
    void btnEqual();

    /**
     * @brief Clears the display and history
     */
    void btnClear();

    /**
     * @brief Adds a decimal point to a number
     */
    void btnDot();

    /**
     * @brief Opens a help window
     */
    void btnHelp();

    /**
     * @brief Converts a string to a mathematical result
     * @param expr A mathematical expression to evaluate
     * @return Expression result
     */
    double evaluateExpression(const QString &expr);

    /**
     * @brief Captures and processes key press on the keyboard
     * @param event Keyboard key
     */
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::CalculatorApp *ui;
};
#endif // MAINWINDOW_H
