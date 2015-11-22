#include "cornersgame.h"
#include <mygamefieldview.h>
#include "ui_cornersgame.h"
#include <QtDebug>

CornersGame::CornersGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CornersGame)
{
    ui->setupUi(this);
    fieldScene = new QGraphicsScene();


    myGameFieldView *gameFieldView = new myGameFieldView(579, 579);
    ui->verticalGameLayout->addWidget(gameFieldView);
    QGraphicsScene *scene = new QGraphicsScene();
    gameFieldView->setScene(scene);
    scene->addPixmap(QPixmap())
}


CornersGame::~CornersGame()
{
    delete ui;
}
