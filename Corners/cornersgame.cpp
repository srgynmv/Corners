#include "cornersgame.h"
#include "ui_cornersgame.h"
#include <QtDebug>
#include <QtWidgets>

CornersGame::CornersGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CornersGame)
{
    //Setup background and background brush
    QPixmap fieldTexture(":/textures/resources/fieldTexture.jpg");
    QGraphicsPixmapItem *fieldTextureItem = new QGraphicsPixmapItem(fieldTexture);
    QBrush backgroundBrush(QImage(":/textures/resources/backgroundTexture.jpg"));

    gameFieldView = new myGameFieldView(fieldTexture.width(), fieldTexture.height());
    gameFieldView->setBackgroundBrush(backgroundBrush);

    //Use form and add our graphicsView
    ui->setupUi(this);
    ui->verticalGameLayout->addWidget(gameFieldView);

    //Add scene to view
    scene = new QGraphicsScene();
    gameFieldView->setScene(scene);
    scene->addItem(fieldTextureItem);

    QObject::connect(ui->newGameButton, SIGNAL(clicked(bool)), this, SLOT(test()));
    QObject::connect(gameFieldView, SIGNAL(resized(QResizeEvent *)), this, SLOT(resizeView(QResizeEvent *)));
    QObject::connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void CornersGame::test()
{
    qDebug() << "Scene height: " << scene->height() << " Scene width: " << scene->width();
}

//WTF How it works
void CornersGame::resizeView(QResizeEvent *event)
{
    double newFieldSize = qMin(event->size().width(), event->size().height());

    //Scales the all view to the new size:
    gameFieldView->scale(newFieldSize / gameFieldView->fieldSize, 1);
    gameFieldView->scale(1, newFieldSize / gameFieldView->fieldSize);

    gameFieldView->fieldSize = newFieldSize;
}

CornersGame::~CornersGame()
{
    delete ui;
}
