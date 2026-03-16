#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
QT_BEGIN_NAMESPACE
namespace Ui {
class HelpWindow;
}
QT_END_NAMESPACE

class HelpWindow : public QDialog
{
    Q_OBJECT
public:
    HelpWindow(QWidget *parent = nullptr);
    ~HelpWindow();

private:
    Ui::HelpWindow *ui;
};

#endif // HELPWINDOW_H
