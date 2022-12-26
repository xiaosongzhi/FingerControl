#ifndef VIRTUALHANDLE_H
#define VIRTUALHANDLE_H

#include <QWidget>

namespace Ui {
class VirtualHandle;
}

class VirtualHandle : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualHandle(QWidget *parent = nullptr);
    ~VirtualHandle();

private:
    Ui::VirtualHandle *ui;
};

#endif // VIRTUALHANDLE_H
