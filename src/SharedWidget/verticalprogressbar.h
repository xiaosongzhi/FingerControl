#ifndef VERTICALPROGRESSBAR_H
#define VERTICALPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class VerticalProgressBar;
}

class VerticalProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit VerticalProgressBar(QWidget *parent = nullptr);
    ~VerticalProgressBar();

private:
    Ui::VerticalProgressBar *ui;
};

#endif // VERTICALPROGRESSBAR_H
