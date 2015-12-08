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
    this->playingWithComputer = ui->computerRadioButton->isChecked();
    QObject::connect(ui->playerRadioButton, SIGNAL(clicked(bool)), this, SLOT(changeDifficulty()));
    QObject::connect(ui->computerRadioButton, SIGNAL(clicked(bool)), this, SLOT(changeDifficulty()));

    //Because game with computer not added yet
    //ui->computerRadioButton->setEnabled(false);
}

void SettingsDialog::changeDifficulty()
{
    if (ui->playerRadioButton->isChecked())
    {
        ui->difficultyWidget->setEnabled(false);
        this->playingWithComputer = false;
    }
    else
    {
        ui->difficultyWidget->setEnabled(true);
        this->playingWithComputer = true;
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
