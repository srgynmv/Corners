#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("Settings");
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
