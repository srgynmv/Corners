#include "cornersgame.h"
#include "ui_cornersgame.h"
#include <QtDebug>
#include <QtWidgets>


CornersGame::CornersGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CornersGame)
{
    //Setup background and background brush
    QPixmap fieldTexture(":/textures/resources/fieldTexture.png");
    QGraphicsPixmapItem *fieldTextureItem = new QGraphicsPixmapItem(fieldTexture);
    QBrush backgroundBrush(QImage(":/textures/resources/backgroundTexture.png"));

    gameFieldView = new myGameFieldView(fieldTexture.width(), fieldTexture.height());
    gameFieldView->setBackgroundBrush(backgroundBrush);

    //Use form and add our graphicsView
    ui->setupUi(this);
    ui->verticalGameLayout->addWidget(gameFieldView);

    //Add scene to view
    scene = new QGraphicsScene();
    gameFieldView->setScene(scene);
    scene->addItem(fieldTextureItem);

    //Because on a start game doesn't running
    gameRunning = false;

    //Setup number of checkers and resizing vectors
    numberOfCheckers = 9;
    whiteCheckers.resize(numberOfCheckers);
    blackCheckers.resize(numberOfCheckers);

    for (int i = 0; i < numberOfCheckers; ++i)
    {
        whiteCheckers[i] = blackCheckers[i] = NULL;
    }

    QObject::connect(ui->newGameButton, SIGNAL(clicked(bool)), this, SLOT(newGameClicked()));
    QObject::connect(gameFieldView, SIGNAL(resized(QResizeEvent *)), this, SLOT(resizeView(QResizeEvent *)));
    QObject::connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void CornersGame::newGameClicked()
{
    if (!gameRunning)
    {
        //Getting and resizing textures
        QPixmap whiteCheckerTexture(":/textures/resources/whiteChess.png");
        QPixmap blackCheckerTexture(":/textures/resources/blackChess.png");
        whiteCheckerTexture = whiteCheckerTexture.scaled(73, 73, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        blackCheckerTexture = blackCheckerTexture.scaled(73, 73, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        //Creating white checkers
        for (int i = 0; i < this->numberOfCheckers; ++i)
        {
            Checker *checker = new Checker(whiteCheckerTexture);
            if (whiteCheckers[i] != NULL) whiteCheckers[i] = checker;
            scene->addItem(checker);
            checker->setPos((i % 3) * gameFieldView->cellSize, scene->height() - (i / 3 + 1) * gameFieldView->cellSize - 1);
        }

        //Creating black checkers
        for (int i = 0; i < this->numberOfCheckers; ++i)
        {
            Checker *checker = new Checker(blackCheckerTexture);
            if (blackCheckers[i] != NULL) blackCheckers[i] = checker;
            scene->addItem(checker);
            checker->setPos(scene->width() - (i % 3 + 1) * gameFieldView->cellSize - 1, (i / 3) * gameFieldView->cellSize);
        }

        gameRunning = true;
    }
    else
    {
        NewGameDialog newGameDialog;

        if (newGameDialog.exec())
        {
            qDebug() << "cout";
            for (int i = 0; i < this->numberOfCheckers; ++i)
            {
                whiteCheckers[i]->setPos((i % 3) * gameFieldView->cellSize, scene->height() - (i / 3 + 1) * gameFieldView->cellSize - 1);
                blackCheckers[i]->setPos(scene->width() - (i % 3 + 1) * gameFieldView->cellSize - 1, (i / 3) * gameFieldView->cellSize);
            }
        }
    }
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
