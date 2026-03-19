#include "helpwindow.h"
#include "./ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::help)
{
    ui->setupUi(this);
    connect(ui->helpClose, &QPushButton::clicked, this, &HelpWindow::close);

    this->setStyleSheet(R"(
        QPushButton#helpClose {
            background-color: #d85a30;
            color: #FFFFFF;
            border-radius: 10px;
            font-size: 12px;
            padding: 8px 12px;
            min-height: 15px
        }

        QPushButton#helpClose:hover {
            background-color: #C04E28;
        }

        QPushButton#helpClose:pressed {
            background-color: #A84420;
        }

        #help {
            background-color: #ffffff;
        }
    )");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
