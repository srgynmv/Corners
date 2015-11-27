#include "checker.h"

Checker::Checker(QPixmap &pixmap) : QGraphicsPixmapItem(pixmap)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

WhiteChecker::WhiteChecker(QPixmap &pixmap) : Checker(pixmap)
{
}

BlackChecker::BlackChecker(QPixmap &pixmap) : Checker(pixmap)
{
}
