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
    ui->infoLabel->setText("");

    newGameDialog = new NewGameDialog;
    settingsDialog = new SettingsDialog;
    rulesDialog = new RulesDialog;
    exitDialog = new ExitDialog;
    gameProcess = NULL;

    QObject::connect(ui->newGameButton, SIGNAL(clicked(bool)), this, SLOT(newGameClicked()));
    QObject::connect(gameFieldView, SIGNAL(resized(QResizeEvent *)), this, SLOT(resizeView(QResizeEvent *)));
    QObject::connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(ui->settingsButton, SIGNAL(clicked(bool)), this->settingsDialog, SLOT(exec()));
    QObject::connect(ui->rulesButton, SIGNAL(clicked(bool)), this->rulesDialog, SLOT(exec()));
}

void CornersGame::newGameClicked()
{
    if (!gameRunning)
    {
        //Getting and resizing textures
        QPixmap whiteCheckerTexture(":/textures/resources/whiteChess.png");
        QPixmap blackCheckerTexture(":/textures/resources/blackChess.png");

        //Converting from double to int
        int cellSize = gameFieldView->cellSize + 0.5;

        whiteCheckerTexture = whiteCheckerTexture.scaled(cellSize, cellSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        blackCheckerTexture = blackCheckerTexture.scaled(cellSize, cellSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        //Creating white and black checkers
        for (int i = 0; i < this->numberOfCheckers; ++i)
        {
            WhiteChecker *whiteChecker = new WhiteChecker(whiteCheckerTexture);
            BlackChecker *blackChecker = new BlackChecker(blackCheckerTexture);

            if (whiteCheckers[i] == NULL)
            {
                scene->addItem(whiteChecker);
                whiteCheckers[i] = whiteChecker;
            }
            if (blackCheckers[i] == NULL)
            {
                blackCheckers[i] = blackChecker;
                scene->addItem(blackChecker);
            }
        }

        if (gameProcess == NULL)
        {
            gameProcess = new GameProcess(this);
            gameProcess->resetGame();
            gameProcess->game();
        }
        else
        {
            gameProcess->resetGame();
        }
    }
    else
    {
        //Out asking dialog about new game
        if (newGameDialog->exec())
        {
            //TODO...:
            //Update scene, matrix, etc.
            gameProcess->resetGame();
        }
    }
}

CornersGame::GameProcess::GameProcess(CornersGame* parent)
{
    this->loop = new QEventLoop;
    this->parent = parent;

    QObject::connect(parent->gameFieldView, SIGNAL(checkerMoved()), loop, SLOT(quit()));
}

void CornersGame::GameProcess::resetGame()
{
    //Reseting position
    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        parent->whiteCheckers[i]->setPos((i % 3) *  parent->gameFieldView->cellSize , parent->scene->height() - (i / 3 + 1) *  parent->gameFieldView->cellSize - 2);
        parent->blackCheckers[i]->setPos(parent->scene->width() - (i % 3 + 1) *  parent->gameFieldView->cellSize - 2, (i / 3) *  parent->gameFieldView->cellSize );
    }
    //Making new turn
    whiteTurn = rand() % 2;
    qDebug() << "New player: " << whiteTurn;
    swapSelectionMode();

    QString info = "Started new game!\n\n";
    info += whiteTurn ? "Turn of white" : "Turn of black";
    parent->ui->infoLabel->setText(info);

}

void CornersGame::GameProcess::getMove()
{
    swapSelectionMode();
    qDebug() << "In getMove()";
    loop->exec();
    qDebug() << "Loop quit";
}

void CornersGame::GameProcess::swapSelectionMode()
{
    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        //Setup selection
        parent->blackCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, !whiteTurn);
        parent->whiteCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, whiteTurn);
    }
}

bool CornersGame::GameProcess::checkHomes()
{
    //TODO...
    qDebug() << "In checkHomes()";
    if (parent->exitDialog->result() == QDialog::Accepted || !parent->gameRunning)
    {
        return false;
    }
    qDebug() << "gameRunning: " << parent->gameRunning;
    return true;
}

void CornersGame::GameProcess::game()
{
    parent->gameRunning = true;
    qDebug() << "Started new game() func";
    whiteTurn = rand() % 2;
    QString info = "Started new game!\n\n";
    info += whiteTurn ? "Turn of white" : "Turn of black";

    while (parent->gameRunning)
    {
        parent->ui->infoLabel->setText(info);

        getMove();

        whiteTurn = !whiteTurn;
        info = whiteTurn? "Turn of white" : "Turn of black";
        parent->gameRunning = checkHomes();
    }

}

CornersGame::GameProcess::~GameProcess()
{
    loop->quit();
}

//WTF How it works
void CornersGame::resizeView(QResizeEvent *event)
{
    double newFieldSize = qMin(event->size().width(), event->size().height());
    //Scales the all view to the new size:
    gameFieldView->scale(newFieldSize / gameFieldView->fieldSize, newFieldSize / gameFieldView->fieldSize);
    gameFieldView->fieldSize = newFieldSize;
}

//Reimplementing closeEvent() for message box
void CornersGame::closeEvent(QCloseEvent *event)
{
    if (exitDialog->exec())
    {
        //Closing game loop event
        if (gameProcess != NULL)
        {
            delete gameProcess;
        }
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

CornersGame::~CornersGame()
{
    qDebug() << "Destructor called";
    delete ui;
}
