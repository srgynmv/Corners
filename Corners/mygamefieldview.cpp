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
    this->cellSize = 72.5;
    fieldSize = height;
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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
