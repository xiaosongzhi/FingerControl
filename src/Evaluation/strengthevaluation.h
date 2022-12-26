#ifndef STRENGTHEVALUATION_H
#define STRENGTHEVALUATION_H
#include <QShowEvent>
#include <QWidget>
#include "dataFormate.h"
#include <QTimer>
#include "userdataformate.h"

#define PAINTNUM 100
namespace Ui {
class StrengthEvaluation;
}

class StrengthEvaluation : public QWidget
{
    Q_OBJECT
public:
    explicit StrengthEvaluation(QWidget *parent = nullptr);
    ~StrengthEvaluation();

public slots:
    //关闭当前界面的训练
    void slotTurnOffRunning(int pageIndex);

protected:
    void showEvent(QShowEvent *event);
private slots:
    void on_finish_Btn_clicked();

    void receiveCommData(QByteArray receiveData);

    void slotPaintChart();
private:
    //初始化数据
    void initData();
    //填充数据
    void fillChartData(ST_RealTimeStrength &);
    //开始力量评估
    void statStrengthEvaluation();
    //结束力量评估
    void stopStrengthEvaluation();
    //绘制力量域图
    void paintAreaChart(ST_RealTimeStrength&);
    //绘制力量线图
    void paintLineChart();
    //设置伸展力量
    void setStretch(ST_RealTimeStrength&);
    //设置屈曲力
    void setBuckling(ST_RealTimeStrength&);
    //存储力量评估数据到数据库中
    void saveStrengthResult();
    //获取图片名字
    QString getImageName();
    //清理评估数据
    void clearEvluateData();
private:
    Ui::StrengthEvaluation *ui;
    QTimer *m_timer;
    QMap<int,QList<int16_t>> dataListMap; //用于绘制线图，填充的数据
    ST_FringerStrengthResult st_FringerStrengthResult;

    QDateTime m_startTime;

    ST_TrainReport st_trainReport; //训练报告

    ST_RealTimeStrength tempStretchStrength;
    ST_RealTimeStrength tempBucklingStrength;

    QStringList colorList;
};

#endif // STRENGTHEVALUATION_H
