#include "mygamefieldview.h"

myGameFieldView::myGameFieldView(int width, int height) : QGraphicsView()
{
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);
    this->setMinimumWidth(width + 2);
    this->setMinimumHeight(height);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fieldSize = height;
}

int myGameFieldView::heightForWidth(int width) const
{
    return width;
}

void myGameFieldView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    emit resized(event);
}
