#include "rulesdialog.h"
#include "ui_rulesdialog.h"

RulesDialog::RulesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RulesDialog)
{
    ui->setupUi(this);
    ui->textBrowser->setSource(QUrl("qrc:/rules.html"));
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("Rules");
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

RulesDialog::~RulesDialog()
{
    delete ui;
}
