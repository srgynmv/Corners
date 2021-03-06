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
    //Making new gameFieldView
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
    newGameStarted = false;

    //Setup number of checkers and resizing vectors
    numberOfCheckers = 9;
    whiteCheckers.resize(numberOfCheckers);
    blackCheckers.resize(numberOfCheckers);

    for (int i = 0; i < numberOfCheckers; ++i)
    {
        whiteCheckers[i] = blackCheckers[i] = NULL;
    }

    //Setup info label
    ui->infoLabel->setText("");
    ui->infoLabel->setVisible(false);

    newGameDialog = new NewGameDialog;
    settingsDialog = new SettingsDialog;
    rulesDialog = new RulesDialog;
    exitDialog = new ExitDialog;
    gameProcess = NULL;

    QObject::connect(ui->newGameButton, SIGNAL(clicked(bool)), this, SLOT(newGameClicked()));
    QObject::connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(ui->settingsButton, SIGNAL(clicked(bool)), this->settingsDialog, SLOT(exec()));
    QObject::connect(ui->rulesButton, SIGNAL(clicked(bool)), this->rulesDialog, SLOT(exec()));

    //Setup window settings
    setWindowTitle("Corners");
    setWindowIcon(QIcon(":/textures/resources/icon.png"));
}

//If we click on new game button, program will call this slot
void CornersGame::newGameClicked()
{
    ui->infoLabel->setVisible(true);

    if (!gameRunning)
    {
        //Getting and resizing textures
        QPixmap whiteCheckerTexture(":/textures/resources/whiteChess.png");
        QPixmap blackCheckerTexture(":/textures/resources/blackChess.png");

        //Converting from double to int
        int cellSize = gameFieldView->cellSize + 0.5;

        //Creating white and black checkers
        for (int i = 0; i < this->numberOfCheckers; ++i)
        {
            WhiteChecker *whiteChecker = new WhiteChecker(whiteCheckerTexture);
            BlackChecker *blackChecker = new BlackChecker(blackCheckerTexture);
            whiteChecker->setScaledPixmap(cellSize);
            blackChecker->setScaledPixmap(cellSize);

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

        //If game is not started:
        if (gameProcess == NULL)
        {
            gameProcess = new GameProcess(this);
            gameProcess->resetGame();
            gameProcess->game();
        }
        else
        {
            gameProcess->resetGame();
            gameProcess->game();
        }
    }
    else
    {
        //Out asking dialog about new game
        if (newGameDialog->exec())
        {
            newGameStarted = true;
            gameProcess->resetGame();
        }
    }
}

//Constructor of GameProcess class
GameProcess::GameProcess(CornersGame* parent)
{
    this->loop = new QEventLoop;
    this->parent = parent;

    whiteCheckerPlayer = NULL;
    blackCheckerPlayer = NULL;

    QObject::connect(parent->gameFieldView, SIGNAL(checkerMoved()), loop, SLOT(quit()));
    QObject::connect(parent, SIGNAL(updateView(QEventLoop*)), parent->gameFieldView, SLOT(updateView(QEventLoop*)));
}

//Reseting all parameters to the start state
void GameProcess::resetGame()
{
    //Stop QEventLoop if it working
    parent->gameRunning = false;
    loop->exit();
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

    whiteCheckerPlayer = new Player(Player::HUMAN, parent->settingsDialog->whitePlayerName(), "white");

    if (parent->settingsDialog->playingWithComputer)
    {
        blackCheckerPlayer = new Player(Player::COMPUTER, "AI", "black");
    }
    else
    {
        blackCheckerPlayer = new Player(Player::HUMAN, parent->settingsDialog->blackPlayerName(), "black");
    }

    //Making first turn, decide who starts
    currentPlayer = (rand() % 2) ? whiteCheckerPlayer : blackCheckerPlayer;
    qDebug() << "First turn of : " << currentPlayer->name();
    swapSelectionMode();

    if (parent->settingsDialog->playingWithComputer)
    {
        blackCheckerPlayer->setSolutionTree(new SolutionTree(parent->gameFieldView->rowAndColumnCount, SolutionTree::Black, blackCheckerPlayer == currentPlayer, parent->numberOfCheckers, parent->settingsDialog->difficulty()));
    }

    //Reset turn counter
    turnCounter = 1;

    //Print information for player
    QString info = "Started new game!\n Game mode:\n";
    info += parent->settingsDialog->playingWithComputer ? "With computer\n\n" : "Two players\n\n";
    info += "Turn #" + QString::number(turnCounter) + "\n\n";
    info += "Turn of: " + currentPlayer->color() + " (" + currentPlayer->name() + ")";
    parent->ui->infoLabel->setText(info);

    parent->gameRunning = true;

}

//Making new field for current state
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

//Getting move from player or AI solution tree
void GameProcess::getMove()
{
    //Changing selection mode because we need or not need to click on checkers
    swapSelectionMode();
    if (currentPlayer->type() == Player::HUMAN)
    {
        qDebug() << "Getting move from player...";
        loop->exec();
        qDebug() << "Got a move." << endl;
    }
    else
    {
        //Sending current state to the tree and getting the new move of tree

        qDebug() << "Getting move from AI...";
        SolutionTree::State *state = new SolutionTree::State(getStateField());
        SolutionTree::Move move = currentPlayer->getMoveFromAI(state);
        parent->gameFieldView->itemAtCell(move.fromI, move.fromJ)->setPos(move.toJ * parent->gameFieldView->cellSize, move.toI * parent->gameFieldView->cellSize);
        qDebug() << "Got a move." << endl;
    }
}

//Setup selection parameter, we can select the item, if player is a human and it's a player's move
void GameProcess::swapSelectionMode()
{
    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        parent->whiteCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, currentPlayer == whiteCheckerPlayer && whiteCheckerPlayer->type() == Player::HUMAN);
        parent->blackCheckers[i]->setFlag(QGraphicsItem::ItemIsSelectable, currentPlayer == blackCheckerPlayer && blackCheckerPlayer->type() == Player::HUMAN);
    }
}

//Check field for game end
bool GameProcess::canContinueGame()
{
    if (parent->exitDialog->result() == QDialog::Accepted)
    {
        return false;
    }

    //Checking new homes
    int whiteAtNewHomeCount = 0;
    int blackAtNewHomeCount = 0;
    int whiteAtHomeCount = 0;
    int blackAtHomeCount = 0;

    qDebug() << endl << "CONTINUE_GAME" << endl;
    //Checking homes
    for (int i = 0; i < parent->numberOfCheckers; ++i)
    {
        int blackI = i % 3;
        int blackJ = parent->gameFieldView->rowAndColumnCount - (i / 3) - 1;
        int whiteI = parent->gameFieldView->rowAndColumnCount - (i / 3) - 1;
        int whiteJ = i % 3;

        if (parent->gameFieldView->itemAtCell(whiteI, whiteJ)->type() == BlackChecker::Type) blackAtNewHomeCount++;
        if (parent->gameFieldView->itemAtCell(blackI, blackJ)->type() == WhiteChecker::Type) whiteAtNewHomeCount++;

        if (turnCounter > 3 * parent->numberOfCheckers * 2)
        {
            if (parent->gameFieldView->itemAtCell(whiteI, whiteJ)->type() == WhiteChecker::Type) whiteAtHomeCount++;
            if (parent->gameFieldView->itemAtCell(blackI, blackJ)->type() == BlackChecker::Type) blackAtHomeCount++;
        }
    }
    qDebug() << "whiteAtNewHomeCount: " << whiteAtNewHomeCount;
    qDebug() << "blackAtNewHomeCount: " << blackAtNewHomeCount;

    //Check if any player has a checkers at home after turn 3 * numberOfCheckers
    if (blackAtHomeCount != 0 && whiteAtHomeCount != 0)
    {
        winnerIs(SolutionTree::Other);
        return false;
    }
    if (whiteAtNewHomeCount == parent->numberOfCheckers || blackAtHomeCount != 0)
    {
        winnerIs(SolutionTree::White);
        return false;
    }
    if (blackAtNewHomeCount == parent->numberOfCheckers || whiteAtHomeCount != 0)
    {
        winnerIs(SolutionTree::Black);
        return false;
    }

    //Check opportunity to make move
    if (!parent->gameFieldView->canMakeMove(currentPlayer->color()))
    {
        if (currentPlayer->color() == "white")
        {
            winnerIs(SolutionTree::Black); //If white cannot move - black is winner
        }
        else
        {
            winnerIs(SolutionTree::White); //Otherwise, white is winner
        }
        return false;
    }

    return true;
}
//Slot that provides a game loop
void GameProcess::game()
{
    qDebug() << "Started new game() func";

    QString info;

    while (parent->gameRunning)
    {
        parent->repaint();
        getMove();

        emit parent->updateView(loop);

        //If new game started, game need to miss last move
        if (parent->newGameStarted)
        {
            parent->newGameStarted = false;
            continue;
        }

        parent->gameRunning = canContinueGame();
        if (!parent->gameRunning) break;
        //Give turn to other player
        currentPlayer = (currentPlayer == blackCheckerPlayer) ? whiteCheckerPlayer : blackCheckerPlayer;
        turnCounter++;

        info = "Turn #" + QString::number((turnCounter + 1)/ 2) + "\n\n";
        info += "Turn of: " + currentPlayer->color() + " (" + currentPlayer->name() + ")";
        parent->ui->infoLabel->setText(info);
    }

}

//Prints information about winner
void GameProcess::winnerIs(SolutionTree::Color color)
{
    QString info;

    if (color == SolutionTree::White)
    {
        info = "Winner is white!\n";
        info += "(" + whiteCheckerPlayer->name() + ")";

    }
    else if (color == SolutionTree::Black)
    {
        info = "Winner is black!\n";
        info += "(" + blackCheckerPlayer->name() + ")";
    }
    else
    {
        info = "Draw!";
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
