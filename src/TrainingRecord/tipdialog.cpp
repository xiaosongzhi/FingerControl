#include "tipdialog.h"
#include "ui_tipdialog.h"

TipDialog::TipDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TipDialog)
{
    ui->setupUi(this);
}

TipDialog::~TipDialog()
{
    delete ui;
}
