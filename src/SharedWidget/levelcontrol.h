#ifndef LEVELCONTROL_H
#define LEVELCONTROL_H

#include <QWidget>

namespace Ui {
class LevelControl;
}

class LevelControl : public QWidget
{
    Q_OBJECT

public:
    explicit LevelControl(QWidget *parent = nullptr);
    ~LevelControl();

private:
    Ui::LevelControl *ui;
};

#endif // LEVELCONTROL_H
