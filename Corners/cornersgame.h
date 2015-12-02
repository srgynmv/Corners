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

namespace Ui {
class CornersGame;
}

class CornersGame : public QMainWindow
{

    Q_OBJECT

    class GameProcess
    {
        friend class CornersGame;
    public:
        GameProcess(CornersGame* parent);
        ~GameProcess();
        void game();
        void getMove();
        bool checkHomes();
        QEventLoop *loop;
        void resetGame();
        void swapSelectionMode();
    private:
        bool whiteTurn;
        CornersGame* parent;
    };

public:
    explicit CornersGame(QWidget *parent = 0);
    ~CornersGame();
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void newGameClicked();
    void resizeView(QResizeEvent *event);

signals:
    void newGameStarted();

private:
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

#endif // CORNERSGAME_H
