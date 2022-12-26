#include "passiveresutexpression.h"
#include "ui_passiveresutexpression.h"

PassiveResutExpression::PassiveResutExpression(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassiveResutExpression)
{
    ui->setupUi(this);
}

PassiveResutExpression::~PassiveResutExpression()
{
    delete ui;
}

void PassiveResutExpression::setExpressionResult(int level)
{
    QString pixPath = QString("border-image: url(:/new/prefix1/source/passiveResult/result%1.png);").arg(level);
    ui->expression_Label->setStyleSheet(pixPath);
}

