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

SolutionTree::SolutionTree(int size, Color color, bool moveOfAI, int numberOfCheckers, int difficulty)
{
    qDebug() << endl << "------PLAYING WITH COMPUTER------" << endl;

    this->color = color;
    this->difficulty = (Difficulty)difficulty;

    QVector< QVector < SolutionTree::CellType > > startField(size, QVector< SolutionTree::CellType > (size, None));

    for (int i = 0; i < numberOfCheckers; ++i)
    {
        startField[size - i / 3 - 1][i % 3] = color == White ? Own : Enemy;
        startField[i / 3][size - i % 3 - 1] = color == White ? Enemy : Own;
    }

    root = new State(startField, moveOfAI);

    srand(QTime::currentTime().msec());
    makeSolutionTree(root, this->TREE_SIZE);
}

bool SolutionTree::canGoTo(State* state, int i, int j)
{
   if (i >= 0 && i < state->field.size() && j >= 0 && j < state->field.size())
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
        if (canGoTo(state, i - 1, j) && state->field[i - 1][j] == None) result.insert(Move(i, j, i - 1, j));
        if (canGoTo(state, i, j + 1) && state->field[i][j + 1] == None) result.insert(Move(i, j, i, j + 1));
    }
    else
    {
        if (canGoTo(state, i, j - 1) && state->field[i][j - 1] == None) result.insert(Move(i, j, i, j - 1));
        if (canGoTo(state, i + 1, j) && state->field[i + 1][j] == None) result.insert(Move(i, j, i + 1, j));
    }

    result += getAdditionalMoves(state, i, j, i, j, checkerColor, true);
    return result;
}

QSet<SolutionTree::Move> SolutionTree::getAdditionalMoves(State *state, int i, int j, int ni, int nj, Color &checkerColor, bool firstMove)
{
    QSet<Move> result;

    if ((canGoTo(state, ni, nj) && state->field[ni][nj] == None) || firstMove)
    {
        if (!firstMove) result.insert(Move(i, j, ni, nj));
    }
    else
    {
        return result;
    }

    CellType type = checkerColor == this->color ? Enemy : Own;

    if (checkerColor == White)
    {
        if (canGoTo(state, ni - 1, nj) && state->field[ni - 1][nj] == type) result += getAdditionalMoves(state, i, j, ni - 2, nj, checkerColor, false);
        if (canGoTo(state, ni, nj + 1) && state->field[ni][nj + 1] == type) result += getAdditionalMoves(state, i, j, ni, nj + 2, checkerColor, false);
    }
    else
    {
        if (canGoTo(state, ni, nj - 1) && state->field[ni][nj - 1] == type) result += getAdditionalMoves(state, i, j, ni, nj - 2, checkerColor, false);
        if (canGoTo(state, ni + 1, nj) && state->field[ni + 1][nj] == type) result += getAdditionalMoves(state, i, j, ni + 2, nj, checkerColor, false);
    }

    return result;
}

int SolutionTree::moveCost(State* state)
{
    CellType type = !state->moveOfAI ? Own : Enemy;
    Color color = this->color;
    if (type == Enemy) color = color == Black ? White : Black;
    //Check losing variants
    int deltaRow = 0;
    int deltaCol = 0;
    for (int k = 0; k < 3; ++k)
    {
        int checkerAtRowCount = 0;
        int checkerAtColCount = 0;
        int allCount = 0;
        if ((color == White && state->field[k][state->field.size() - k - 1] == type) || (color == Black && state->field[state->field.size() - k - 1][k] == type)) allCount--;

        for (int i = k; i < state->field.size(); ++i)
        {
            if ((color == White && state->field[k][state->field.size() - 1 - i] == type) || (color == Black && state->field[state->field.size() - 1 - k][i] == type)) checkerAtRowCount++;
            if ((color == White && state->field[i][state->field.size() - 1 - k] == type) || (color == Black && state->field[state->field.size() - 1 - i][k] == type)) checkerAtColCount++;
        }

        allCount += checkerAtColCount + checkerAtRowCount;
        if (checkerAtColCount <= 3 - k && checkerAtRowCount <= 3 - k && allCount <= 5 - 2 * k)
        {
                deltaRow += 3 - k - checkerAtRowCount;
                deltaCol += 3 - k - checkerAtColCount;
        }
        else
        {
            return 0;
        }
    }

    int cost = 0;

    //Making a non-zero cost
    if (difficulty == Easy)
    {
        cost = rand() % state->field.size() + 1;
    }
    if (difficulty == Medium)
    {
        if (!(rand() % 4))
        {
            cost = (int)(sqrt((state->move.fromI - state->move.toI) * (state->move.fromI - state->move.toI) + (state->move.fromJ - state->move.toJ) * (state->move.fromJ - state->move.toJ)) + 0.5);
            cost = cost * 4;
        }
        else
        {
            cost = rand() % (MAX_COST / 4) + 1;
        }
    }
    if (difficulty == Hard)
    {
        cost = (int)(sqrt((state->move.fromI - state->move.toI) * (state->move.fromI - state->move.toI) + (state->move.fromJ - state->move.toJ) * (state->move.fromJ - state->move.toJ)) + 0.5);
        cost *= 4;
    }

    //Make tree to leave home
    if (state->turnNumber <= 55)
    {
        if (color == White)
        {
            //If checker at home
            if (state->field.size() - 3 <= state->move.fromI && state->move.fromI < state->field.size() && 0 <= state->move.fromJ && state->move.fromJ < 3)
            {
                cost = MAX_COST - 1;
                if (state->move.toI < state->field.size() - 3 || state->move.toJ >= 3) cost++;
            }
            //If one checker blocks other
            if ((state->move.fromI == state->field.size() - 4 && state->field[state->move.fromI + 1][state->move.fromJ] == type) || (state->move.fromJ == 3 && state->field[state->move.fromI][state->move.fromJ - 1] == type))
            {
                cost = MAX_COST;
            }
        }
        else
        {
            //If checker at home
            if (state->field.size() - 3 <= state->move.fromJ && state->move.fromJ < state->field.size() && 0 <= state->move.fromI && state->move.fromI < 3)
            {
                cost = MAX_COST - 1;
                if (state->move.toI >= 3 || state->move.toJ < state->field.size() - 3) cost++;
            }
            //If one checker blocks other
            if ((state->move.fromI == 3 && state->field[state->move.fromI - 1][state->move.fromJ] == type) || (state->move.fromJ == state->field.size() - 4 && state->field[state->move.fromI][state->move.fromJ + 1] == type))
            {
                cost = MAX_COST;
            }
        }
    }

    return cost;
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

        state->child.push_back(new State(state->field, !state->moveOfAI, *it, state->turnNumber + 1));
        state->child.last()->cost = moveCost(state->child.last());      //Making cost of move
        makeSolutionTree(state->child.last(), count - 1);

        swap(state->field[it->fromI][it->fromJ], state->field[it->toI][it->toJ]);
    }

}

void SolutionTree::deleteBranch(State *state)
{
    if (state == NULL) return;

    for (int i = 0; i < state->child.size(); ++i)
    {
        deleteBranch(state->child[i]);
    }
    delete state;
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
    State* newState = root;

    //Find state from field in tree and delete other
    if (!root->moveOfAI)
    {
        for (int i = 0; i < root->child.size(); ++i)
        {
            if (state->field == root->child[i]->field)
            {
                newState = root->child[i];
            }
            else
            {
                deleteBranch(root->child[i]);
            }
        }
        delete root;
    }

    root = newState;

    if (root->child.size() == 0)
    {
        makeSolutionTree(root, TREE_SIZE);
    }

    if (!root->moveOfAI)
    {
        throw QUnhandledException();
    }

    //Make move with biggest cost (depends on difficulty)
    int cost = - 1;
    int index = 0;
    for (int i = 0; i < root->child.size(); ++i)
    {
        if (root->child[i]->cost > cost || (root->child[i]->cost == cost && rand() % 2))
        {
            cost = root->child[i]->cost;
            index = i;
        }
    }

    Move move = root->child[index]->move;

    qDebug() << "Making move from " << move.fromI << "," << move.fromJ << "  to  " << move.toI << "," << move.toJ;
    qDebug() << "Move cost: " << root->child[index]->cost;
    newState = root->child[index];

    //Delete other
    for (int i = 0; i < root->child.size(); ++i)
    {
        if (root->child[i] != newState) deleteBranch(root->child[i]);
    }
    delete root;

    root = newState;

    CellType type = Own;
    int deltaRow = 0;
    int deltaCol = 0;
    for (int k = 0; k < 3; ++k)
    {
        int checkerAtRowCount = 0;
        int checkerAtColCount = 0;
        int allCount = 0;
        if ((color == White && state->field[k][state->field.size() - k - 1] == type) || (color == Black && state->field[state->field.size() - k - 1][k] == type)) allCount--;

        for (int i = k; i < state->field.size(); ++i)
        {
            if ((color == White && state->field[k][state->field.size() - 1 - i] == type) || (color == Black && state->field[state->field.size() - 1 - k][i] == type)) checkerAtRowCount++;
            if ((color == White && state->field[i][state->field.size() - 1 - k] == type) || (color == Black && state->field[state->field.size() - 1 - i][k] == type)) checkerAtColCount++;
        }

        allCount += checkerAtColCount + checkerAtRowCount + deltaCol + deltaRow;
        if (checkerAtColCount <= 3 - k + deltaCol && checkerAtRowCount <= 3 - k + deltaRow && allCount <= 5 - 2 * k + deltaCol + deltaRow)
        {
                deltaRow += 3 - k - checkerAtRowCount;
                deltaCol += 3 - k - checkerAtColCount;
        }
        else
        {
            qDebug() << "bad move";
            qDebug() << endl;
            return move;
        }
    }
    qDebug() << "good move";
    qDebug() << endl;

    return move;

}

SolutionTree::~SolutionTree()
{
    deleteBranch(root);
}

SolutionTree::State::State(QVector< QVector< SolutionTree::CellType > > newField, bool moveOf, int newTurnNumber) : field(newField), moveOfAI(moveOf) , turnNumber(newTurnNumber) {}
SolutionTree::State::State(QVector< QVector< SolutionTree::CellType > > newField, bool moveOf, Move newMove, int newTurnNumber) :
    field(newField),
    moveOfAI(moveOf),
    move(newMove),
    turnNumber(newTurnNumber)
{}

