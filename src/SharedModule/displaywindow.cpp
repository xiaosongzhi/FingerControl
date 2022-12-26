#include "displaywindow.h"
#include "ui_displaywindow.h"

DisplayWindow::DisplayWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}

void DisplayWindow::on_clear_Btn_clicked()
{
    ui->textEdit->clear();
}

void DisplayWindow::setText(QByteArray array)
{
    ui->textEdit->append(array.toHex() + "\r\n");
    ui->textEdit->moveCursor(QTextCursor::End);
}
