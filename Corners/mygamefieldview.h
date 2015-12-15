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
    void checkerMoved();

public slots:
    void updateView(QEventLoop *loop);

public:
    QColor green;

    myGameFieldView(int width, int height);
    virtual int heightForWidth(int width) const;
    virtual void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    QGraphicsItem* itemAtCell(int i, int j);

    bool putRect(int i, int j, QVector<QGraphicsRectItem *> &result);
    bool canGoTo(int i, int j);
    bool canMakeMove(QString color);
    QVector<QGraphicsRectItem *> createPossibleMoves(int checkerI, int checkerJ, bool white);
    void createAdditionalMoves(int i, int j, QVector<QGraphicsRectItem *> &result, bool white, bool firstRectangle);
    void erasePossibleMoves();


    double fieldSize;
    double cellSize;
    bool checkerSelected;
    int rowAndColumnCount;

private:
    int transparensy;
    QVector<QGraphicsRectItem *> selectionItems;
    double selectedCheckerX;
    double selectedCheckerY;

};

#endif // MYGAMEFIELDVIEW_H
