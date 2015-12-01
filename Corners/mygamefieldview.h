#ifndef MYGAMEFIELDVIEW_H
#define MYGAMEFIELDVIEW_H
#include <QtWidgets>
#include <exitdialog.h>
#include <checker.h>
#include <QtDebug>

class myGameFieldView  : public QGraphicsView
{
    Q_OBJECT

    friend class CornersGame;

signals:
    void resized(QResizeEvent *event);
    void checkerMoved();

public:
    myGameFieldView(int width, int height);
    virtual int heightForWidth(int width) const;
    virtual void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    double fieldSize;
    double cellSize;
    bool selection;
    void printPossibleMoves(QMouseEvent *event);
    void erasePossibleMoves();

};

#endif // MYGAMEFIELDVIEW_H
