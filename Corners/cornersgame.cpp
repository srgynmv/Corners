#include "cornersgame.h"
#include <mygamefieldview.h>
#include "ui_cornersgame.h"
#include <QtDebug>

QGraphicsScene *scene;

CornersGame::CornersGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CornersGame)
{
    ui->setupUi(this);
    fieldScene = new QGraphicsScene();


    myGameFieldView *gameFieldView = new myGameFieldView(579, 579);
    ui->verticalGameLayout->addWidget(gameFieldView);
    scene = new QGraphicsScene();
    gameFieldView->setScene(scene);

    QObject::connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    scene->addPixmap(QPixmap(":/textures/resources/fieldTexture.jpg"));
    QObject::connect(ui->newGameButton, SIGNAL(clicked(bool)), this, SLOT(test()));
}

void CornersGame::test()
{
    qDebug() << "Scene height: " << scene->height() << " Scene width: " << scene->width();
}

CornersGame::~CornersGame()
{
    delete ui;
}
