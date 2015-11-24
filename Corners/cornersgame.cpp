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
    QPoint point = gameFieldView->mapFromScene(gameFieldView->scene()->width(), gameFieldView->scene()->height());
    if (point.x() - gameFieldView->scene()->width() != 0)
    {
        double newWidth = 2 * point.x() - event->size().width();
        qDebug() << newWidth;
        gameFieldView->scale(gameFieldView->fieldSize / newWidth, 1);
    }
    if (point.y() - gameFieldView->scene()->height() != 0)
    {
        double newHeight = 2 * point.y() - event->size().height();
        qDebug() << newHeight;
        gameFieldView->scale(1, gameFieldView->fieldSize / newHeight);
    }
    qDebug() << "scene x: " << gameFieldView->scene()->width() << " scene y: " << gameFieldView->scene()->height();
    qDebug() << "window size x: " << event->size().width() << " window size y: " << event->size().height();
    qDebug() << "x: " << point.x() << " y: " << point.y();
    qDebug() << "minSize == " << gameFieldView->fieldSize << endl;

    gameFieldView->fieldSize = qMin(event->size().width(), event->size().height());
}

CornersGame::~CornersGame()
{
    delete ui;
}
