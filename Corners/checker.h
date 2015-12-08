#ifndef CHECKER_H
#define CHECKER_H
#include <QtWidgets>
#include <QtDebug>

class Checker : public QGraphicsPixmapItem
{
public:
    enum { Black = UserType + 1, White = UserType + 2 };
    Checker(QPixmap &pixmap);

};

class WhiteChecker : public Checker
{
public:
    enum { Type = Checker::White };
    WhiteChecker(QPixmap &pixmap);
    int type() const
    {
        return Type;
    }
};

class BlackChecker : public Checker
{
public:
    enum { Type = Checker::Black };
    BlackChecker(QPixmap &pixmap);
    int type() const
    {
        return Type;
    }
};

#endif // CHECKER_H
