#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>

namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = nullptr);
    ~DisplayWindow();

    void setText(QByteArray);
private slots:
    void on_clear_Btn_clicked();

private:
    Ui::DisplayWindow *ui;
};

#endif // DISPLAYWINDOW_H
