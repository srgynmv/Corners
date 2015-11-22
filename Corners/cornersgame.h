#ifndef CORNERSGAME_H
#define CORNERSGAME_H

#include <QMainWindow>
#include <QtWidgets>

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
    void test();

private:
    Ui::CornersGame *ui;
    QGraphicsScene *fieldScene;
};

#endif // CORNERSGAME_H
