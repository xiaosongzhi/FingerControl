#ifndef TRAININGGAME_H
#define TRAININGGAME_H

#include <QWidget>

namespace Ui {
class TrainingGame;
}

class TrainingGame : public QWidget
{
    Q_OBJECT

public:
    explicit TrainingGame(QWidget *parent = nullptr);
    ~TrainingGame();

private slots:
    void on_memoryTrain_Btn_clicked();

private:
    Ui::TrainingGame *ui;
};

#endif // TRAININGGAME_H
