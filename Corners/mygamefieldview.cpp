#include "mygamefieldview.h"

myGameFieldView::myGameFieldView(int width, int height) : QGraphicsView(), TRANSPARENSY(50)
{
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);
    this->setMinimumWidth(width + 2);
    this->setMinimumHeight(height);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->rowAndColumnCount = 8;
    fieldSize = height;
    this->cellSize = fieldSize / rowAndColumnCount;
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    checkerSelected = false;
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
    for (int i = 0; i < selectionItems.size(); ++i)
    {
        delete selectionItems[i];
    }
    selectionItems.resize(0);
    selectionItems.squeeze();
}

QVector<QGraphicsRectItem *> myGameFieldView::printPossibleMoves(QMouseEvent *event, bool white)
{
    QPoint clickPoint(event->x(), event->y());
    QPointF clickPointF;
    clickPointF = this->mapToScene(clickPoint);

    int checker_i = clickPointF.y() / cellSize;
    int checker_j = clickPointF.x() / cellSize;

    QColor green = QColor(0, 255, 0, TRANSPARENSY);

    QVector<QGraphicsRectItem *> result;

    QGraphicsItem *item;
    if (white)
    {
        if (canGoTo(checker_j, checker_i - 1))
        {
            item = this->itemAt(mapFromScene(checker_j * cellSize + cellSize / 2, (checker_i - 1) * cellSize + cellSize / 2));

            if (item->type() != WhiteChecker::Type && item->type() != BlackChecker::Type)
            {
                QGraphicsRectItem *up = this->scene()->addRect((checker_j) * cellSize, (checker_i - 1) * cellSize, cellSize, cellSize, QPen(Qt::NoPen), QBrush(green));
                result.push_back(up);
            }
        }
        if (canGoTo(checker_j + 1, checker_i))
        {
            item = this->itemAt(mapFromScene((checker_j + 1) * cellSize + cellSize / 2, checker_i * cellSize + cellSize / 2));

            if (item->type() != WhiteChecker::Type && item->type() != BlackChecker::Type)
            {
                QGraphicsRectItem *right = this->scene()->addRect((checker_j + 1) * cellSize, checker_i * cellSize, cellSize, cellSize, QPen(Qt::NoPen), QBrush(green));
                result.push_back(right);
            }
        }
    }
    else
    {
        if (canGoTo(checker_j - 1, checker_i))
        {
            item = this->itemAt(mapFromScene((checker_j - 1) * cellSize + cellSize / 2, (checker_i) * cellSize + cellSize / 2));

            if (item->type() != WhiteChecker::Type && item->type() != BlackChecker::Type)
            {
               QGraphicsRectItem *left = this->scene()->addRect((checker_j - 1) * cellSize, checker_i * cellSize, cellSize, cellSize, QPen(Qt::NoPen), QBrush(green));
               result.push_back(left);
            }
        }
        if (canGoTo(checker_j, checker_i + 1))
        {
            item = this->itemAt(mapFromScene((checker_j) * cellSize + cellSize / 2, (checker_i + 1) * cellSize + cellSize / 2));

            if (item->type() != WhiteChecker::Type && item->type() != BlackChecker::Type)
            {
                QGraphicsRectItem *down = this->scene()->addRect((checker_j) * cellSize, (checker_i + 1) * cellSize, cellSize, cellSize, QPen(Qt::NoPen), QBrush(green));
                result.push_back(down);
            }
        }
    }

    return result;
}

bool myGameFieldView::canGoTo(int j, int i)
{
   if (j >= 0 && j < rowAndColumnCount && i >= 0 && i < rowAndColumnCount)
   {
       return true;
   }
   else
   {
       return false;
   }
}

void myGameFieldView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (checkerSelected && this->itemAt(event->x(), event->y())->type() == QGraphicsRectItem::Type)
    {
        //Checking clicked cell
        //emit "moving" signal
        checkerSelected = false;
        erasePossibleMoves();

        QPoint clickPoint(event->x(), event->y());
        QPointF clickPointF;
        clickPointF = this->mapToScene(clickPoint);

        int checkerI = clickPointF.y() / cellSize;
        int checkerJ = clickPointF.x() / cellSize;

        this->itemAt(selectedCheckerX, selectedCheckerY)->setPos(checkerJ * cellSize, checkerI * cellSize);
        emit checkerMoved();
        return;
    }

    //Deleting all possible moves from field
    checkerSelected = false;
    erasePossibleMoves();

    if (this->itemAt(event->x(), event->y())->isSelected() &&
            (this->itemAt(event->x(), event->y())->type() == Checker::White || this->itemAt(event->x(), event->y())->type() == Checker::Black))
    {
        checkerSelected = true;

        selectedCheckerX = event->x();
        selectedCheckerY = event->y();

        //Printing new possible moves
        bool white = this->itemAt(event->x(), event->y())->type() == Checker::White ? true : false;
        selectionItems = printPossibleMoves(event, white);
    }
}


