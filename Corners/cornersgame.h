#ifndef CORNERSGAME_H
#define CORNERSGAME_H

#include <QMainWindow>

namespace Ui {
class CornersGame;
}

class CornersGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit CornersGame(QWidget *parent = 0);
    ~CornersGame();

private:
    Ui::CornersGame *ui;
};

#endif // CORNERSGAME_H
