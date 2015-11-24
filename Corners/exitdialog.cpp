#include "exitdialog.h"
#include "ui_exitdialog.h"

ExitDialog::ExitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExitDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("Exit");
}

ExitDialog::~ExitDialog()
{
    delete ui;
}
