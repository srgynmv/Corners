#ifndef CORNERSGAME_H
#define CORNERSGAME_H

#include <QMainWindow>
#include <QtWidgets>
#include <mygamefieldview.h>
#include <checker.h>
#include <newgamedialog.h>

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

private:
    Ui::CornersGame *ui;
    myGameFieldView *gameFieldView;
    QGraphicsScene *scene;
    QGraphicsScene *fieldScene;
    QVector <Checker *> whiteCheckers, blackCheckers;
    bool gameRunning;
    int numberOfCheckers;
};

#endif // CORNERSGAME_H
