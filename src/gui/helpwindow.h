#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
QT_BEGIN_NAMESPACE
namespace Ui {
class help;
}
QT_END_NAMESPACE

/**
 * @class HelpWindow
 * @brief Application usage help window
 */
class HelpWindow : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Helpwindow constructor
     * @param parent Pointer to the parent widget
     */
    HelpWindow(QWidget *parent = nullptr);

    /**
     * @brief Helpwindow destructor
     */
    ~HelpWindow();

private:
    Ui::help *ui;
};

#endif // HELPWINDOW_H
