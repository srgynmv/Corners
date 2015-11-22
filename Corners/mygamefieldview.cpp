#include "mygamefieldview.h"

myGameFieldView::myGameFieldView(int width, int height) : QGraphicsView()
{
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);
    this->setMinimumWidth(width);
    this->setMinimumHeight(height);
}

int myGameFieldView::heightForWidth(int width) const
{
    return width;
}

void myGameFieldView::resizeEvent(QResizeEvent *event)
{
    int minSize = qMin(event->size().height(), event->size().width());
    qDebug() << "In resizeEvent() with size " << minSize;
    qDebug() << "x: " << this->x() << " y: " << this->y();
    qDebug() << "parent-x: " << this->parentWidget()->x() << " parent-y: " << this->parentWidget()->y();
    this->setGeometry(this->x(), this->y(), minSize, minSize);
}
