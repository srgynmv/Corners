#include "checker.h"

Checker::Checker(QPixmap &pixmap) : QGraphicsPixmapItem(pixmap)
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
}
