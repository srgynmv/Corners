#include "newgamedialog.h"
#include "ui_newgamedialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle(" ");
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}
