#include "cornersgame.h"
#include "ui_cornersgame.h"

CornersGame::CornersGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CornersGame)
{
    ui->setupUi(this);
}

CornersGame::~CornersGame()
{
    delete ui;
}
