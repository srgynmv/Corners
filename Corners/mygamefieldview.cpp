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
    qDebug() << "x: " << this->x() << " y: " << this->y();
    int newY = this->parentWidget()->height() / 2 - minSize / 2;
    int newX = this->parentWidget()->width() > 1000 ? ((this->parentWidget()->width() - 1000 + 9) - (this->parentWidget()->height() - 600) / 2 - (this->parentWidget()->width() - 1000 + 9) / 3) : this->x();

    newX = newX < 9 ? 9 : newX;

    qDebug() << "NewX: " << newX << " NewY: " << newY;
    this->setGeometry(newX, newY, minSize, minSize);
    QGraphicsView::resizeEvent(event);
}
