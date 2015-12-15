#ifndef SOLUTIONTREE_H
#define SOLUTIONTREE_H

#include <QtWidgets>
#include <QtDebug>

class SolutionTree
{
    friend class GameProcess;

public:
    enum Color
    {
        Black, White
    };

    enum CellType
    {
        Enemy, Own, None
    };

    class Move
    {
    public:
        int fromI;
        int fromJ;
        int toI;
        int toJ;
        Move(int fi, int fj, int ti, int tj) : fromI(fi), fromJ(fj), toI(ti), toJ(tj) {}
        Move() {}
        inline bool operator==(const Move a) const
        {
            return (a.fromI == fromI && a.fromJ == fromJ && a.toI == toI && a.toJ == toJ);
        }
    };

    class State
    {
    public:
        QVector<State*> child;
        QVector<QVector<CellType> > field;
        bool moveOfAI;
        Move move;
        State(QVector< QVector< CellType > > newField, bool moveOf = false);
        State(QVector< QVector< CellType > > newField, bool moveOf, Move newMove);
    };

    static const int TREE_SIZE = 5;
    bool canGoTo(State* state, int i, int j);
    QSet<Move> getPossibleMoves(State *state, int i, int j);
    void makeSolutionTree(State *state, int count);
    Color color;
    SolutionTree(int size, SolutionTree::Color color, bool moveOfAI, int numberOfCheckers);
    Move getMove(State *state);
    State* root;
};

#endif // SOLUTIONTREE_H
