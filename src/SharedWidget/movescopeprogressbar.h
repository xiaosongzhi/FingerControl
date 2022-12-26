#ifndef MOVESCOPEPROGRESSBAR_H
#define MOVESCOPEPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class MoveScopeProgressBar;
}

class MoveScopeProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit MoveScopeProgressBar(QWidget *parent = nullptr);
    ~MoveScopeProgressBar();

private:
    Ui::MoveScopeProgressBar *ui;
};

#endif // MOVESCOPEPROGRESSBAR_H
