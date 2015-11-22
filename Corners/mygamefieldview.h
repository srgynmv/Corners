#ifndef MYGAMEFIELDVIEW_H
#define MYGAMEFIELDVIEW_H
#include <QtWidgets>

class myGameFieldView  : public QGraphicsView
{
    friend class CornersGame;
public:
    myGameFieldView(int width, int height);

    virtual int heightForWidth(int width) const;

    virtual void resizeEvent(QResizeEvent *event);
};

#endif // MYGAMEFIELDVIEW_H
