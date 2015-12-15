#include "solutiontree.h"

inline uint qHash(const SolutionTree::Move& t)
{
    return qHash(t.fromI + t.fromJ * 3 + t.toI * 9 + t.toJ * 27);
}

void swap(SolutionTree::CellType &a, SolutionTree::CellType &b)
{
    SolutionTree::CellType tmp;
    tmp = a;
    a = b;
    b = tmp;
}

SolutionTree::SolutionTree(int size, Color color, bool moveOfAI, int numberOfCheckers)
{
    qDebug() << endl << "------PLAYING WITH COMPUTER------" << endl;
    this->color = color;

    QVector< QVector < SolutionTree::CellType > > startField(size, QVector< SolutionTree::CellType > (size, None));

    for (int i = 0; i < numberOfCheckers; ++i)
    {
        startField[size - i / 3 - 1][i % 3] = color == White ? Own : Enemy;
        startField[i / 3][size - i % 3 - 1] = color == White ? Enemy : Own;
    }

    root = new State(startField, moveOfAI);

    makeSolutionTree(root, this->TREE_SIZE);
}

bool SolutionTree::canGoTo(State* state, int i, int j)
{
   if (i >= 0 && i < state->field.size() && j >= 0 && j < state->field.size() && state->field[i][j] == None)
   {
       return true;
   }
   else
   {
       return false;
   }
}

QSet<SolutionTree::Move> SolutionTree::getPossibleMoves(State *state, int i, int j)
{
    QSet<Move> result;

    Color checkerColor = this->color;

    if (state->field[i][j] ==  Enemy)
    {
        checkerColor = this->color == White ? Black : White;
    }

    if (checkerColor == White)
    {
        if (canGoTo(state, i - 1, j)) result.insert(Move(i, j, i - 1, j));
        if (canGoTo(state, i, j + 1)) result.insert(Move(i, j, i, j + 1));
    }
    else
    {
        if (canGoTo(state, i, j - 1)) result.insert(Move(i, j, i, j - 1));
        if (canGoTo(state, i + 1, j)) result.insert(Move(i, j, i + 1, j));
    }

    return result;
//    QVector<QGraphicsRectItem *> myGameFieldView::createPossibleMoves(QMouseEvent *event, bool white)
//    {
//        QPoint clickPoint(event->x(), event->y());
//        QPointF clickPointF;
//        clickPointF = this->mapToScene(clickPoint);

//        int checkerI = clickPointF.y() / cellSize;
//        int checkerJ = clickPointF.x() / cellSize;

//        QVector<QGraphicsRectItem *> result;

//        if (white)
//        {
//            putRect(checkerI - 1, checkerJ, result);
//            putRect(checkerI, checkerJ + 1, result);
//        }
//        else
//        {
//            putRect(checkerI, checkerJ - 1, result);
//            putRect(checkerI + 1, checkerJ, result);
//        }

//        createAdditionalMoves(checkerI, checkerJ, result, white, true);
//        return result;
//    }

//    void myGameFieldView::createAdditionalMoves(int i, int j, QVector<QGraphicsRectItem *> &result, bool white, bool firstRectangle)
//    {
//        bool rectangleAdded = putRect(i, j, result);
//        if (!rectangleAdded && !firstRectangle)
//        {
//            return;
//        }

//        QGraphicsItem *item;
//        if (white)
//        {
//            item = itemAtCell(i - 1, j);
//            if (item != NULL && item->type() == BlackChecker::Type) createAdditionalMoves(i - 2, j, result, white, false);
//            item = itemAtCell(i, j + 1);
//            if (item != NULL && item->type() == BlackChecker::Type) createAdditionalMoves(i, j + 2, result, white, false);
//        }
//        else
//        {
//            item = itemAtCell(i, j - 1);
//            if (item != NULL && item->type() == WhiteChecker::Type) createAdditionalMoves(i, j - 2, result, white, false);
//            item = itemAtCell(i + 1, j);
//            if (item != NULL && item->type() == WhiteChecker::Type) createAdditionalMoves(i + 2, j, result, white, false);
//        }
//    }
}


void SolutionTree::makeSolutionTree(State *state, int count)
{
    if (count == 0) return;

    QSet<Move> moves;

    for (int i = 0; i < state->field.size(); ++i)
    {
        for (int j = 0; j < state->field.size(); ++j)
        {
            if ((state->moveOfAI && state->field[i][j] == Own) || (!state->moveOfAI && state->field[i][j] == Enemy))
            {
                moves += getPossibleMoves(state, i, j);
            }
        }
    }

    for (QSet<Move>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
        swap(state->field[it->fromI][it->fromJ], state->field[it->toI][it->toJ]);

        state->child.push_back(new State(state->field, !state->moveOfAI, *it));
        makeSolutionTree(state->child.last(), count - 1);

        swap(state->field[it->fromI][it->fromJ], state->field[it->toI][it->toJ]);
    }

}

SolutionTree::Move SolutionTree::getMove(State *state)
{
    if (root == NULL)
    {
        root = state;
        root->moveOfAI = true;
        makeSolutionTree(root, TREE_SIZE);
    }

    if (root->child.size() == 0)
    {
        makeSolutionTree(root, TREE_SIZE);
    }

    //Moving to state that on screen
    if (!root->moveOfAI)
    {
        for (int i = 0; i < root->child.size(); ++i)
        {
            if (state->field == root->child[i]->field)
            {
                root = root->child[i];
            }
        }
    }

    if (root->child.size() == 0)
    {
        makeSolutionTree(root, TREE_SIZE);
    }

//    if (root->child.size() == 0)
//    {
//        return Move();
//    }

    if (!root->moveOfAI)
    {
        throw QUnhandledException();
    }

    int index = rand() % root->child.size();

    Move move = root->child[index]->move;

    qDebug() << "Making move from " << move.fromI << "," << move.fromJ << "  to  " << move.toI << "," << move.toJ;
    root = root->child[index];
    return move;

}

SolutionTree::State::State(QVector< QVector< SolutionTree::CellType > > newField, bool moveOf) : field(newField), moveOfAI(moveOf) {}
SolutionTree::State::State(QVector< QVector< SolutionTree::CellType > > newField, bool moveOf, Move newMove) : field(newField), moveOfAI(moveOf), move(newMove) {}

