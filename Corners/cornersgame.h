#ifndef CORNERSGAME_H
#define CORNERSGAME_H

#include <QMainWindow>
#include <QtWidgets>
#include <mygamefieldview.h>
#include <checker.h>
#include <newgamedialog.h>
#include <exitdialog.h>
#include <QtDebug>

namespace Ui {
class CornersGame;
}

class CornersGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit CornersGame(QWidget *parent = 0);
    ~CornersGame();

public slots:
    void newGameClicked();
    void resizeView(QResizeEvent *event);
    void askingClose();
private:
    Ui::CornersGame *ui;
    myGameFieldView *gameFieldView;
    QGraphicsScene *scene;
    QGraphicsScene *fieldScene;
    QVector <Checker *> whiteCheckers, blackCheckers;
    bool gameRunning;
    int numberOfCheckers;
    NewGameDialog newGameDialog;
    ExitDialog exitDialog;
};

#endif // CORNERSGAME_H
