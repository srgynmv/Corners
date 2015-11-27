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
    this->cellSize = fieldSize / 8;
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

void myGameFieldView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    qDebug() << "clicked on: ";
    if (this->itemAt(event->x(), event->y())->type() == Checker::White)
    {
        qDebug() << "white checker";
        //this->itemAt(event->x(), event->y())->setSelected(true);
    }
    else if (this->itemAt(event->x(), event->y())->type() == Checker::Black)
    {
        qDebug() << "black checker";
    }
    else
    {
        qDebug() << "field";
    }
}


