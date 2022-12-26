#ifndef PASSIVERESUTEXPRESSION_H
#define PASSIVERESUTEXPRESSION_H

#include <QWidget>

namespace Ui {
class PassiveResutExpression;
}

class PassiveResutExpression : public QWidget
{
    Q_OBJECT

public:
    explicit PassiveResutExpression(QWidget *parent = nullptr);
    ~PassiveResutExpression();
    //设置笑脸内容 @int level 0~4 从哭到笑
    void setExpressionResult(int level);
private:
    Ui::PassiveResutExpression *ui;
};

#endif // PASSIVERESUTEXPRESSION_H
