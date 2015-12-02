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
    bool checkerSelected;
    bool canGoTo(int j, int i);
    QVector<QGraphicsRectItem *> printPossibleMoves(QMouseEvent *event, bool white);
    void erasePossibleMoves();
    int rowAndColumnCount;
private:
    const int TRANSPARENSY;
    QVector<QGraphicsRectItem *> selectionItems;
    double selectedCheckerX;
    double selectedCheckerY;

};

#endif // MYGAMEFIELDVIEW_H
