#include "checker.h"

Checker::Checker(QPixmap &pixmap) : QGraphicsPixmapItem(pixmap)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    originalTexture = pixmap;
}

void Checker::setScaledPixmap(int size)
{
    this->setPixmap(originalTexture.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

WhiteChecker::WhiteChecker(QPixmap &pixmap) : Checker(pixmap)
{
}

BlackChecker::BlackChecker(QPixmap &pixmap) : Checker(pixmap)
{
}
