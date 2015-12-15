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

GameProcess::GameProcess(CornersGame* parent) : QEvent()
{
    this->loop = new QEventLoop;
    this->parent = parent;

    whiteCheckerPlayer = NULL;
    blackCheckerPlayer = NULL;

    QObject::connect(parent->gameFieldView, SIGNAL(checkerMoved()), loop, SLOT(quit()));
    //QObject::connect(this, SIGNAL(updateView(QEventLoop*)), parent->gameFieldView, SLOT(updateView(QEventLoop*)));
}

void GameProcess::resetGame()
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

    //Import settings
    if (whiteCheckerPlayer != NULL) delete whiteCheckerPlayer;
    if (blackCheckerPlayer != NULL) delete blackCheckerPlayer;
    whiteCheckerPlayer = new Player(Player::HUMAN, "Sergey", "white");

    if (parent->settingsDialog->playingWithComputer)
    {
        blackCheckerPlayer = new Player(Player::COMPUTER, "AI", "black");
    }
    else
    {
        blackCheckerPlayer = new Player(Player::HUMAN, "Liubov", "black");
    }

    //Making new turn
    currentPlayer = (rand() % 2) ? whiteCheckerPlayer : blackCheckerPlayer;
    qDebug() << "First turn of : " << currentPlayer->name();
    swapSelectionMode();

    if (parent->settingsDialog->playingWithComputer)
    {
        blackCheckerPlayer->setSolutionTree(new SolutionTree(parent->gameFieldView->rowAndColumnCount, SolutionTree::Black, blackCheckerPlayer == currentPlayer, parent->numberOfCheckers));
    }

    //Print information for player
    QString info = "Started new game!\n\n Game mode:\n";
    info += parent->settingsDialog->playingWithComputer ? "With computer\n\n" : "Two players\n\n";
    info += "Turn of: " + currentPlayer->color() + " (" + currentPlayer->name() + ")";
    parent->ui->infoLabel->setText(info);

    parent->gameRunning = true;

}

QVector<QVector<SolutionTree::CellType> > GameProcess::getStateField()
{
    QVector<QVector<SolutionTree::CellType> > result(parent->gameFieldView->rowAndColumnCount, QVector<SolutionTree::CellType> (parent->gameFieldView->rowAndColumnCount, SolutionTree::None));
    for (int i = 0; i < result.size(); ++i)
    {
        for (int j = 0; j < result.size(); ++j)
        {
            if (parent->gameFieldView->itemAtCell(i, j)->type() == WhiteChecker::Type)
            {
                result[i][j] = currentPlayer->color() == "white" ? SolutionTree::Own : SolutionTree::Enemy;
            }
            if (parent->gameFieldView->itemAtCell(i, j)->type() == BlackChecker::Type)
            {
                result[i][j] = currentPlayer->color() == "black" ? SolutionTree::Own : SolutionTree::Enemy;
            }
        }
    }
    return result;
}

void GameProcess::getMove()
{
    swapSelectionMode();
    //qDebug() << "In getMove()";
    if (currentPlayer->type() == Player::HUMAN)
    {
        qDebug() << "Getting move from player...";
        loop->exec();
        qDebug() << "Got a move." << endl;
    }
    else
    {
        qDebug() << "Getting move from AI...";
        SolutionTree::State *state = new SolutionTree::State(getStateField());
        SolutionTree::Move move = currentPlayer->getMoveFromAI(state);
        parent->gameFieldView->itemAtCell(move.fromI, move.fromJ)->setPos(move.toJ * parent->gameFieldView->cellSize, move.toI * parent->gameFieldView->cellSize);
        qDebug() << "Got a move." << endl;
    }
    //TODO: Update view
    emit updateView(loop);
}

void GameProcess::swapSelectionMode()
{
    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        //Setup selection

        parent->whiteCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, currentPlayer == whiteCheckerPlayer && whiteCheckerPlayer->type() == Player::HUMAN);
        parent->blackCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, currentPlayer == blackCheckerPlayer && blackCheckerPlayer->type() == Player::HUMAN);
    }
}

bool GameProcess::checkHomes()
{
    //TODO...
    //Check checker at it's home

    //qDebug() << "In checkHomes()";
    if (parent->exitDialog->result() == QDialog::Accepted)
    {
        return false;
    }

    //Checking new homes
    int whiteInNewHomeCount = 0;
    int blackInNewHomeCount = 0;

    qDebug() << endl << "CHECK_HOMES" << endl;

    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        int blackI = i % 3;
        int blackJ = parent->gameFieldView->rowAndColumnCount - (i / 3) - 1;
        int whiteI = parent->gameFieldView->rowAndColumnCount - (i / 3) - 1;
        int whiteJ = i % 3;

        //qDebug() << parent->gameFieldView->itemAtCell(whiteI, whiteJ)->type();
        if (parent->gameFieldView->itemAtCell(whiteI, whiteJ)->type() == BlackChecker::Type)
        {
            blackInNewHomeCount++;
        }
        //qDebug() << parent->gameFieldView->itemAtCell(blackI, blackJ)->type() << endl;
        if (parent->gameFieldView->itemAtCell(blackI, blackJ)->type() == WhiteChecker::Type)
        {
            whiteInNewHomeCount++;
        }
    }

    qDebug() << "WhiteInNewHomeCount: " << whiteInNewHomeCount;
    qDebug() << "BlackInNewHomeCount: " << blackInNewHomeCount;

    if (whiteInNewHomeCount == parent->numberOfCheckers)
    {
        winnerIs(true);
        return false;
    }
    if (blackInNewHomeCount == parent->numberOfCheckers)
    {
        winnerIs(false);
        return false;
    }
    return true;
}

void GameProcess::game()
{
    qDebug() << "Started new game() func";

    QString info;

    while (parent->gameRunning)
    {
        parent->repaint();
        getMove();

        currentPlayer = (currentPlayer == blackCheckerPlayer) ? whiteCheckerPlayer : blackCheckerPlayer;

        info = "Turn of: " + currentPlayer->color() + " (" + currentPlayer->name() + ")";
        parent->ui->infoLabel->setText(info);

        parent->gameRunning = checkHomes();
    }

}

void GameProcess::winnerIs(bool whiteWin)
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

GameProcess::~GameProcess()
{
    loop->quit();
}

//Reimplementing closeEvent() for message box
void CornersGame::closeEvent(QCloseEvent *event)
{
    if (exitDialog->exec())
    {
        //Closing game loop event
        QMainWindow::closeEvent(event);
    }
    else
    {
        event->ignore();
    }
}

CornersGame::~CornersGame()
{
    qDebug() << "Destructor called";
    delete newGameDialog;
    delete settingsDialog;
    delete rulesDialog;
    delete exitDialog;
    delete scene;
    delete gameProcess;
    delete ui;
}
