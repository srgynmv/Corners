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
        Black, White, Other
    };

    enum CellType
    {
        Enemy, Own, None
    };

    enum Difficulty
    {
        Easy, Medium, Hard
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
        int cost;
        int turnNumber;

        State(QVector< QVector< CellType > > newField, bool moveOf = false, int newTurnNumber = 1);
        State(QVector< QVector< CellType > > newField, bool moveOf, Move newMove, int newTurnNumber);
    };

    bool canGoTo(State* state, int i, int j);
    QSet<Move> getPossibleMoves(State *state, int i, int j);
    QSet<Move> getAdditionalMoves(State *state, int i, int j, int ni, int nj, Color &checkerColor, bool firstMove);
    void makeSolutionTree(State *state, int count);
    void deleteBranch(State *state);
    SolutionTree(int size, SolutionTree::Color color, bool moveOfAI, int numberOfCheckers, int difficulty);
    Move getMove(State *state);
    int moveCost(State *state);

    static const int TREE_SIZE = 5;
    static const int MAX_COST = 20;
    Color color;
    Difficulty difficulty;
    State* root;

    ~SolutionTree();
};

#endif // SOLUTIONTREE_H
