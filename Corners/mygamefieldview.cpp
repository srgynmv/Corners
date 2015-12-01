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

void myGameFieldView::erasePossibleMoves()
{

}

void myGameFieldView::printPossibleMoves(QMouseEvent *event)
{

}

void myGameFieldView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

//        qDebug() << "clicked on: ";
//        if (this->itemAt(event->x(), event->y())->type() == Checker::White)
//        {
//            qDebug() << "white checker";
//        }
//        else if (this->itemAt(event->x(), event->y())->type() == Checker::Black)
//        {
//            qDebug() << "black checker";
//        }
//        else
//        {
//            qDebug() << "field";
//        }

    if (selection)
    {
        //Checking clicked cell
        //emit signal

        //return;
    }

    //Deleting all possible moves from field
    selection = false;
    erasePossibleMoves();

    if (this->itemAt(event->x(), event->y())->isSelected() &&
            (this->itemAt(event->x(), event->y())->type() == Checker::White || this->itemAt(event->x(), event->y())->type() == Checker::Black))
    {
        selection = true;
        //Printing new possible moves
        printPossibleMoves(event);
    }
}


