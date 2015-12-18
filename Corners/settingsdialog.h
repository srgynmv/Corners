#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}
/* Class that can get settings from player
 * and save it.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

    friend class CornersGame;
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    QString whitePlayerName();
    QString blackPlayerName();
    int difficulty();

    bool playingWithComputer;

public slots:
    void changeDifficulty();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
