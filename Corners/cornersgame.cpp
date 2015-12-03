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
                scene->addItem(blackChecker);
                blackCheckers[i] = blackChecker;
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
    //Deleting old selecting rectangles

    parent->gameFieldView->erasePossibleMoves();
    parent->gameFieldView->checkerSelected = false;

    //Making new turn
    whiteTurn = rand() % 2;
    qDebug() << "New player: " << whiteTurn;
    swapSelectionMode();

    QString info = "Started new game!\n\n";
    info += whiteTurn ? "Turn of white" : "Turn of black";
    parent->ui->infoLabel->setText(info);

    parent->gameRunning = true;

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
    //Check checker at it's home

    qDebug() << "In checkHomes()";
    if (parent->exitDialog->result() == QDialog::Accepted)
    {
        return false;
    }

    //Checking new homes
    int whiteInNewHomeCount = 0;
    int blackInNewHomeCount = 0;

    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        int blackI = i % 3;
        int blackJ = parent->gameFieldView->rowAndColumnCount - (i / 3) - 1;
        int whiteI = parent->gameFieldView->rowAndColumnCount - (i / 3) - 1;
        int whiteJ = i % 3;

        if (parent->gameFieldView->itemAtCell(whiteI, whiteJ)->type() == BlackChecker::Type)
        {
            blackInNewHomeCount++;
        }
        if (parent->gameFieldView->itemAtCell(blackI, blackJ)->type() == WhiteChecker::Type)
        {
            whiteInNewHomeCount++;
        }
    }

    qDebug() << "WhiteInNewHomeCount: " << whiteInNewHomeCount;
    qDebug() << "BlackInNewHomeCount: " << blackInNewHomeCount;

    if (whiteInNewHomeCount == parent->numberOfCheckers)
    {
        winnerIsWhite(true);
        return false;
    }
    if (blackInNewHomeCount == parent->numberOfCheckers)
    {
        winnerIsWhite(false);
        return false;
    }
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

void CornersGame::GameProcess::winnerIsWhite(bool whiteWin)
{
    QString info;

    if (whiteWin)
    {
        info = "Winner is white!";
    }
    else
    {
        info = "Winner is black!";
    }

    parent->ui->infoLabel->setText(info);

    //Disabling selection
    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        parent->blackCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, false);
        parent->whiteCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, false);
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
