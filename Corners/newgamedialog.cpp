#include "newgamedialog.h"
#include "ui_newgamedialog.h"

NewGameDialog::newGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}
