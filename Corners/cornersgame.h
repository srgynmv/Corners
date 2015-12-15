#ifndef CORNERSGAME_H
#define CORNERSGAME_H

#include <QMainWindow>
#include <QtWidgets>
#include <mygamefieldview.h>
#include <checker.h>
#include <newgamedialog.h>
#include <exitdialog.h>
#include <QtDebug>
#include <settingsdialog.h>
#include <rulesdialog.h>
#include <solutiontree.h>

namespace Ui {
class CornersGame;
}

class GameProcess;

class CornersGame : public QMainWindow
{

    Q_OBJECT

    friend class GameProcess;

public:
    explicit CornersGame(QWidget *parent = 0);
    ~CornersGame();
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void newGameClicked();

signals:
    void updateView(QEventLoop*);

private:
    bool newGameStarted;

    GameProcess* gameProcess;
    Ui::CornersGame *ui;
    myGameFieldView *gameFieldView;
    QGraphicsScene *scene;
    QGraphicsScene *fieldScene;
    QVector <Checker *> whiteCheckers, blackCheckers;
    bool gameRunning;
    int numberOfCheckers;

    NewGameDialog *newGameDialog;
    SettingsDialog *settingsDialog;
    RulesDialog *rulesDialog;
    ExitDialog *exitDialog;
};

class Player
{
public:
    enum Type
    {
        HUMAN, COMPUTER
    };

    Player(Type newType, QString name, QString color) :  playerName(name), playerType(newType), checkerColor(color) {}

private:

    QString playerName;
    Type playerType;
    QString checkerColor;
    SolutionTree *AI;

public:

    void setSolutionTree(SolutionTree *NewAI)
    {
        AI = NewAI;
    }

    Type type()
    {
        return playerType;
    }

    QString name()
    {
        return playerName;
    }

    QString color()
    {
        return checkerColor;
    }

    SolutionTree::Move getMoveFromAI(SolutionTree::State* state)
    {
        return AI->getMove(state);
    }
};


class GameProcess
{

    friend class CornersGame;

public:
    GameProcess(CornersGame* parent);
    ~GameProcess();

    void game();
    void getMove();
    bool canContinueGame();
    void resetGame();
    void swapSelectionMode();
    void winnerIs(bool whiteWin);
    QVector<QVector<SolutionTree::CellType> > getStateField();

    Player *whiteCheckerPlayer;
    Player *blackCheckerPlayer;
    Player *currentPlayer;
    QEventLoop *loop;
private:
    bool whiteTurn;
    CornersGame* parent;
};

#endif // CORNERSGAME_H
