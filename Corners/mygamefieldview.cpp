#include "mygamefieldview.h"

//Constructor with width and height
myGameFieldView::myGameFieldView(int width, int height) : QGraphicsView(), transparensy(50)
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

    green = QColor(0, 255, 0, transparensy);

    checkerSelected = false;
}

//Reimplemented method that make height equal to width
int myGameFieldView::heightForWidth(int width) const
{
    return width;
}
//Make resize with keeping aspect ratio
void myGameFieldView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    double newFieldSize = qMin(event->size().width(), event->size().height());
    //Scales the all view to the new size:
    this->scale(newFieldSize / this->fieldSize, newFieldSize / this->fieldSize);
    this->fieldSize = newFieldSize;
}
//Clean field from moves
void myGameFieldView::erasePossibleMoves()
{
    for (int i = 0; i < selectionItems.size(); ++i)
    {
        delete selectionItems[i];
    }
    selectionItems.resize(0);
}
//Return pointer to item at cell(i, j)
QGraphicsItem* myGameFieldView::itemAtCell(int i, int j)
{
    return this->itemAt(mapFromScene(j * cellSize + cellSize / 2, i * cellSize + cellSize / 2));
}
//Put rectangle at [i,j], true - if successfully, otherwise - false
bool myGameFieldView::putRect(int i, int j, QVector<QGraphicsRectItem *> &result)
{
    if (!canGoTo(i, j))
    {
        return false;
    }

    QGraphicsItem *item = this->itemAtCell(i, j);

    if (item->type() != WhiteChecker::Type && item->type() != BlackChecker::Type && item->type() != QGraphicsRectItem::Type)
    {
        QGraphicsRectItem *rect = this->scene()->addRect(j * cellSize, i * cellSize, cellSize, cellSize, QPen(Qt::NoPen), QBrush(green));
        result.push_back(rect);
        return true;
    }
    else
    {
        return false;
    }
}

//Making a green rects at field
QVector<QGraphicsRectItem *> myGameFieldView::createPossibleMoves(int checkerI, int checkerJ, bool white)
{
    QVector<QGraphicsRectItem *> result;

    if (white)
    {
        putRect(checkerI - 1, checkerJ, result);
        putRect(checkerI, checkerJ + 1, result);
    }
    else
    {
        putRect(checkerI, checkerJ - 1, result);
        putRect(checkerI + 1, checkerJ, result);
    }

    createAdditionalMoves(checkerI, checkerJ, result, white, true);
    return result;
}
//Allows jumping throught checkers
void myGameFieldView::createAdditionalMoves(int i, int j, QVector<QGraphicsRectItem *> &result, bool white, bool firstRectangle)
{
    bool rectangleAdded = putRect(i, j, result);
    if (!rectangleAdded && !firstRectangle)
    {
        return;
    }

    QGraphicsItem *item;
    if (white)
    {
        item = itemAtCell(i - 1, j);
        if (item != NULL && item->type() == BlackChecker::Type) createAdditionalMoves(i - 2, j, result, white, false);
        item = itemAtCell(i, j + 1);
        if (item != NULL && item->type() == BlackChecker::Type) createAdditionalMoves(i, j + 2, result, white, false);
    }
    else
    {
        item = itemAtCell(i, j - 1);
        if (item != NULL && item->type() == WhiteChecker::Type) createAdditionalMoves(i, j - 2, result, white, false);
        item = itemAtCell(i + 1, j);
        if (item != NULL && item->type() == WhiteChecker::Type) createAdditionalMoves(i + 2, j, result, white, false);
    }
}
//Checks can we go to i, j
bool myGameFieldView::canGoTo(int i, int j)
{
   if (i >= 0 && i < rowAndColumnCount && j >= 0 && j < rowAndColumnCount)
   {
       return true;
   }
   else
   {
       return false;
   }
}

//Returns true, if any checker of this color can make move
bool myGameFieldView::canMakeMove(QString color)
{
    //Making invisible green rects to make move count
    transparensy = 0;

    for (int i = 0; i < rowAndColumnCount; ++i)
    {
        for (int j = 0; j < rowAndColumnCount; ++j)
        {
            if (color == "white" && itemAtCell(i, j)->type() == WhiteChecker::Type) selectionItems += createPossibleMoves(i, j, true);
            if (color == "black" && itemAtCell(i, j)->type() == BlackChecker::Type) selectionItems += createPossibleMoves(i, j, false);
        }
    }
    transparensy = 50;
    int count = selectionItems.size();
    erasePossibleMoves();

    if (count != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//Making doubleclick like once click
void myGameFieldView::mouseDoubleClickEvent(QMouseEvent *event)
{
    mousePressEvent(event);
}

//Reimplementing once click event:
void myGameFieldView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    //If player clicked on green rectangle
    if (checkerSelected && this->itemAt(event->x(), event->y())->type() == QGraphicsRectItem::Type)
    {
        //Checking clicked cell
        //emit "moving" signal
        checkerSelected = false;
        erasePossibleMoves();

        QPoint clickPoint(event->x(), event->y());
        QPointF clickPointF;
        clickPointF = this->mapToScene(clickPoint);

        int checkerI = (clickPointF.y() + 1) / cellSize;
        int checkerJ = (clickPointF.x() + 1) / cellSize;

        if (itemAtCell(checkerI, checkerJ)->type() != WhiteChecker::Type && itemAtCell(checkerI, checkerJ)->type() != BlackChecker::Type)
        {
            this->itemAt(selectedCheckerX, selectedCheckerY)->setPos(checkerJ * cellSize, checkerI * cellSize);
            qDebug() << "GAMEVIEW: moving checker.";

            emit checkerMoved();
        }
    }
    else
    {
        //Deleting all possible moves from field
        checkerSelected = false;
        erasePossibleMoves();
        //If player clicked on his checker, print rectangles
        if (this->itemAt(event->x(), event->y())->flags() == QGraphicsItem::ItemIsSelectable &&
                (this->itemAt(event->x(), event->y())->type() == Checker::White || this->itemAt(event->x(), event->y())->type() == Checker::Black))
        {
            checkerSelected = true;

            selectedCheckerX = event->x();
            selectedCheckerY = event->y();

            //Printing new possible moves
            bool white = this->itemAt(event->x(), event->y())->type() == Checker::White ? true : false;

            //Convert click point to cell coordinates
            QPoint clickPoint(event->x(), event->y());
            QPointF clickPointF;
            clickPointF = this->mapToScene(clickPoint);

            int checkerI = clickPointF.y() / cellSize;
            int checkerJ = clickPointF.x() / cellSize;

            selectionItems = createPossibleMoves(checkerI, checkerJ, white);
        }
    }
}

//Update view on screen
void myGameFieldView::updateView(QEventLoop *loop)
{
    //Start timer 50ms and call loop->quit() after timeout
    QTimer::singleShot(50, loop, SLOT(quit()));
    //Force main thread to sleep and wake up a gameFieldView
    loop->exec();
}
