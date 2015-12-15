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

    int index = rand() % root->child.size();

    Move move = root->child[index]->move;

    qDebug() << "Making move from " << move.fromI << "," << move.fromJ << "  to  " << move.toI << "," << move.toJ;

    newState = root->child[index];

    //Delete other
    for (int i = 0; i < root->child.size(); ++i)
    {
        if (root->child[i] != newState) deleteBranch(root->child[i]);
    }
    delete root;

    root = newState;

    return move;

}

SolutionTree::~SolutionTree()
{
    deleteBranch(root);
}

SolutionTree::State::State(QVector< QVector< SolutionTree::CellType > > newField, bool moveOf) : field(newField), moveOfAI(moveOf) {}
SolutionTree::State::State(QVector< QVector< SolutionTree::CellType > > newField, bool moveOf, Move newMove) : field(newField), moveOfAI(moveOf), move(newMove) {}

