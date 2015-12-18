#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class NewGameDialog;
}
//Dialog that asking about starting new game
class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = 0);
    ~NewGameDialog();

private:
    Ui::NewGameDialog *ui;
};

#endif // NEWGAMEDIALOG_H
