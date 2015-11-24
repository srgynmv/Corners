#ifndef MYGAMEFIELDVIEW_H
#define MYGAMEFIELDVIEW_H
#include <QtWidgets>

class myGameFieldView  : public QGraphicsView
{
    Q_OBJECT

    friend class CornersGame;

signals:
    void resized(QResizeEvent *event);
public:
    myGameFieldView(int width, int height);
    virtual int heightForWidth(int width) const;
    virtual void resizeEvent(QResizeEvent *event);
    double fieldSize;
    double cellSize;
};

#endif // MYGAMEFIELDVIEW_H
