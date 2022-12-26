#include "chandstateshow.h"
#include "ui_chandstateshow.h"
#include "dataFormate.h"
#include <QDebug>

CHandStateShow::CHandStateShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHandStateShow),
    m_handType (0),
    m_maxValue(0)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    initWidget();
}

CHandStateShow::~CHandStateShow()
{
    delete ui;
}

void CHandStateShow::initWidget()
{
    ui->thumbL_Label->setStyleSheet("");
    ui->thumbR_Label->setStyleSheet("");
    ui->forefingerL_Label->setStyleSheet("");
    ui->forefingerR_Label->setStyleSheet("");
    ui->middlefingerL_Label->setStyleSheet("");
    ui->middlefingerR_Label->setStyleSheet("");
    ui->ringfingerL_Label->setStyleSheet("");
    ui->ringfingerR_Label->setStyleSheet("");
    ui->littlefingerL_Label->setStyleSheet("");
    ui->littlefingerR_Label->setStyleSheet("");

    ui->thumbL_Label->setLabelName("L0");
    ui->thumbR_Label->setLabelName("R0");
    ui->forefingerL_Label->setLabelName("L1");
    ui->forefingerR_Label->setLabelName("R1");
    ui->middlefingerL_Label->setLabelName("L2");
    ui->middlefingerR_Label->setLabelName("R2");
    ui->ringfingerL_Label->setLabelName("L3");
    ui->ringfingerR_Label->setLabelName("R3");
    ui->littlefingerL_Label->setLabelName("L4");
    ui->littlefingerR_Label->setLabelName("R4");

    foreach(QObject*object,ui->left_GroupBox->children())
    {
        CLabel *lable = static_cast<CLabel*>(object);
        connect(lable,&CLabel::signalOnPressed,this,&CHandStateShow::signalLabelOnPressed);
    }

    foreach(QObject*object,ui->right_GroupBox->children())
    {
        CLabel *lable = static_cast<CLabel*>(object);
        connect(lable,&CLabel::signalOnPressed,this,&CHandStateShow::signalLabelOnPressed);
    }
}

void CHandStateShow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
#if 1
    float xScale = this->width()/560.f;
    float yScale = this->height()/560.f;

    //左手
    ui->littlefingerL_Label->resize(115*xScale,194*yScale);
    ui->littlefingerL_Label->move(73*xScale,123*yScale);

    ui->ringfingerL_Label->resize(81*xScale,223*yScale);
    ui->ringfingerL_Label->move(172*xScale,43*yScale);

    ui->middlefingerL_Label->resize(96*xScale,225*yScale);
    ui->middlefingerL_Label->move(228*xScale,19*yScale);

    ui->forefingerL_Label->resize(133*xScale,268*yScale);
    ui->forefingerL_Label->move(283*xScale,61*yScale);

    ui->thumbL_Label->resize(150*xScale,219*yScale);
    ui->thumbL_Label->move(335*xScale,248*yScale);

    //右手
    ui->littlefingerR_Label->resize(115*xScale,194*yScale);
    ui->littlefingerR_Label->move(372*xScale,123*yScale);

    ui->ringfingerR_Label->resize(81*xScale,223*yScale);
    ui->ringfingerR_Label->move(307*xScale,43*yScale);

    ui->middlefingerR_Label->resize(96*xScale,225*yScale);
    ui->middlefingerR_Label->move(236*xScale,19*yScale);

    ui->forefingerR_Label->resize(133*xScale,268*yScale);
    ui->forefingerR_Label->move(144*xScale,61*yScale);

    ui->thumbR_Label->resize(150*xScale,219*yScale);
    ui->thumbR_Label->move(75*xScale,248*yScale);
#endif
}

void CHandStateShow::setFingerState(int fingerIndex,int state)
{
    QString normalStyle("");
    //左手
    if(0 == m_handType)
    {
        switch(fingerIndex)
        {
        case 0: //拇指
            switch(state)
            {
            case NORMAL_E :
                ui->thumbL_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->thumbL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/thumbR.png);");
                break;
            case EvaluateGreen_E:
                ui->thumbL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/thumbG.png);");
                break;
            case EvaluateYellow_E:
                ui->thumbL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/thumbY.png);");
                break;
            case TipsPurple_E:
                ui->thumbL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/thumbP.png);");
                break;
            case TipsGreen_E:
                ui->thumbL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/thumbG.png);");
                break;
            case DisableGray_E:
                // "border-image: url(:/new/prefix1/source/RHandState/strengthEvaluate/forefingerR.png);"
                ui->thumbL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/lock/thumb.png);");
                break;
            }
            break;
        case 1: //食指
            switch(state)
            {
            case NORMAL_E :
                ui->forefingerL_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->forefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/forefingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->forefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/forefingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->forefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/forefingerY.png);");
                break;
            case TipsPurple_E:
                ui->forefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/forefingerP.png);");
                break;
            case TipsGreen_E:
                ui->forefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/forefingerG.png);");
                break;
            case DisableGray_E:
                ui->forefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/lock/forefinger.png);");
                break;
            }
            break;
        case 2: //中指
            switch(state)
            {
            case NORMAL_E :
                ui->middlefingerL_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->middlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/middlefingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->middlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/middlefingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->middlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/middlefingerY.png);");
                break;
            case TipsPurple_E:
                ui->middlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/middlefingerP.png);");
                break;
            case TipsGreen_E:
                ui->middlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/middlefingerG.png);");
                break;
            case DisableGray_E:
                ui->middlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/lock/middlefinger.png);");
                break;
            }
            break;
        case 3: //无名指
            switch(state)
            {
            case NORMAL_E :
                ui->ringfingerL_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->ringfingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/ringfingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->ringfingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/ringfingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->ringfingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/ringfingerY.png);");
                break;
            case TipsPurple_E:
                ui->ringfingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/ringfingerP.png);");
                break;
            case TipsGreen_E:
                ui->ringfingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/ringfingerG.png);");
                break;
            case DisableGray_E:
                ui->ringfingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/lock/ringfinger.png);");
                break;
            }
            break;
        case 4: //小指
            switch(state)
            {
            case NORMAL_E :
                ui->littlefingerL_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->littlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/littlefingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->littlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/littlefingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->littlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/strengthEvaluate/littlefingerY.png);");
                break;
            case TipsPurple_E:
                ui->littlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/littlefingerP.png);");
                break;
            case TipsGreen_E:
                ui->littlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/visionTips/littlefingerG.png);");
                break;
            case DisableGray_E:
                ui->littlefingerL_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/LHandState/lock/littlefinger.png);");
                break;
            }
            break;
        }

    }
    else if(1 == m_handType)
    {
        switch(fingerIndex)
        {
        case 0: //拇指
            switch(state)
            {
            case NORMAL_E :
                ui->thumbR_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->thumbR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/thumbR.png);");
                break;
            case EvaluateGreen_E:
                ui->thumbR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/thumbG.png);");
                break;
            case EvaluateYellow_E:
                ui->thumbR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/thumbY.png);");
                break;
            case TipsPurple_E:
                ui->thumbR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/thumbP.png);");
                break;
            case TipsGreen_E:
                ui->thumbR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/thumbG.png);");
                break;
            case DisableGray_E:
                ui->thumbR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/lock/thumb.png);");
                break;
            }
            break;
        case 1: //食指
            switch(state)
            {
            case NORMAL_E :
                ui->forefingerR_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->forefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/forefingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->forefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/forefingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->forefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/forefingerY.png);");
                break;
            case TipsPurple_E:
                ui->forefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/forefingerP.png);");
                break;
            case TipsGreen_E:
                ui->forefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/forefingerG.png);");
                break;
            case DisableGray_E:
                ui->forefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/lock/forefinger.png);");
                break;
            }
            break;
        case 2: //中指
            switch(state)
            {
            case NORMAL_E :
                ui->middlefingerR_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->middlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/middlefingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->middlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/middlefingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->middlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/middlefingerY.png);");
                break;
            case TipsPurple_E:
                ui->middlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/middlefingerP.png);");
                break;
            case TipsGreen_E:
                ui->middlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/middlefingerG.png);");
                break;
            case DisableGray_E:
                ui->middlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/lock/middlefinger.png);");
                break;
            }
            break;
        case 3: //无名指
            switch(state)
            {
            case NORMAL_E :
                ui->ringfingerR_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->ringfingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/ringfingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->ringfingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/ringfingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->ringfingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/ringfingerY.png);");
                break;
            case TipsPurple_E:
                ui->ringfingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/ringfingerP.png);");
                break;
            case TipsGreen_E:
                ui->ringfingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/ringfingerG.png);");
                break;
            case DisableGray_E:
                ui->ringfingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/lock/ringfinger.png);");
                break;
            }
            break;
        case 4: //小指
            switch(state)
            {
            case NORMAL_E :
                ui->littlefingerR_Label->setMyStyleSheet(normalStyle);
                break;
            case EvaluateRed_E:
                ui->littlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/littlefingerR.png);");
                break;
            case EvaluateGreen_E:
                ui->littlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/littlefingerG.png);");
                break;
            case EvaluateYellow_E:
                ui->littlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/strengthEvaluate/littlefingerY.png);");
                break;
            case TipsPurple_E:
                ui->littlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/littlefingerP.png);");
                break;
            case TipsGreen_E:
                ui->littlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/visionTips/littlefingerG.png);");
                break;
            case DisableGray_E:
                ui->littlefingerR_Label->setMyStyleSheet("border-image:url(:/new/prefix1/source/RHandState/lock/littlefinger.png);");
                break;
            }
            break;
        }
    }
}

void CHandStateShow::setFingerEnabled(int index,bool isEnabled)
{

    if(isEnabled)
        setFingerState(index,NORMAL_E);
    else
        setFingerState(index,DisableGray_E);
    switch(m_handType)
    {
    case LeftHand_E:
    {
        switch(index)
        {
        case Thumb_E:
            ui->thumbL_Label->setEnabled(isEnabled);
            break;
        case ForeFinger_E:
            ui->forefingerL_Label->setEnabled(isEnabled);
            break;
        case MiddleFinger_E:
            ui->middlefingerL_Label->setEnabled(isEnabled);
            break;
        case RingFinger_E:
            ui->ringfingerL_Label->setEnabled(isEnabled);
            break;
        case LittleFinger_E:
            ui->littlefingerL_Label->setEnabled(isEnabled);
            break;
        }
    }
        break;
    case RightHand_E:
    {
        switch(index)
        {
        case Thumb_E:
            ui->thumbR_Label->setEnabled(isEnabled);
            break;
        case ForeFinger_E:
            ui->forefingerR_Label->setEnabled(isEnabled);
            break;
        case MiddleFinger_E:
            ui->middlefingerR_Label->setEnabled(isEnabled);
            break;
        case RingFinger_E:
            ui->ringfingerR_Label->setEnabled(isEnabled);
            break;
        case LittleFinger_E:
            ui->littlefingerR_Label->setEnabled(isEnabled);
            break;
        }
    }
        break;
    }
}

//设置左右手
void CHandStateShow::setHandType(int type)
{
    m_handType = type;
    ui->stackedWidget->setCurrentIndex(type);
}

void CHandStateShow::switchFingerDelayShow(bool on)
{
    foreach(QObject*object,ui->left_GroupBox->children())
    {
        CLabel *lable = static_cast<CLabel*>(object);
        lable->switchDelayShow(on);
    }

    foreach(QObject*object,ui->right_GroupBox->children())
    {
        CLabel *lable = static_cast<CLabel*>(object);
        lable->switchDelayShow(on);
    }
}

void CHandStateShow::switchPressedEvent(bool on)
{
    foreach(QObject*object,ui->left_GroupBox->children())
    {
        CLabel *lable = static_cast<CLabel*>(object);
        lable->switchPressend(on);
    }

    foreach(QObject*object,ui->right_GroupBox->children())
    {
        CLabel *lable = static_cast<CLabel*>(object);
        lable->switchPressend(on);
    }
}

//根据力量设置颜色
void CHandStateShow::setFingerColorByStrength(ST_RealTimeStrength st_realStrength)
{
//    设置原则
//    (1)找出各手指的最大值
//    (2)(0%~50%)*MaxValue 红色
//    (3)(51%~80%)*MaxValue 黄色
//    (4)(81%~100%)*MaxValue 绿色
    int16_t array[5] = {0};
    array[0] = qAbs(st_realStrength.thumb);
    array[1] = qAbs(st_realStrength.forefinger);
    array[2] = qAbs(st_realStrength.middlefinger);
    array[3] = qAbs(st_realStrength.ringfinger);
    array[4] = qAbs(st_realStrength.littlefinger);


    for(int i = 0;i < 5;i++)
    {
        if(m_maxValue <array[i] )
            m_maxValue = array[i];
    }
    //1-red 2-green 3-yellow
    if(m_maxValue > 10)
    for(int i = 0;i < 5;i++)
    {
        if(array[i] > m_maxValue*0.8)
            setFingerState(i,EvaluateGreen_E);
        else if((array[i] > m_maxValue*0.5) && (array[i] <= m_maxValue*0.8))
            setFingerState(i,EvaluateYellow_E);
        else if((array[i] > 0) && (array[i] <= m_maxValue*0.5))
            setFingerState(i,EvaluateRed_E);
    }
}

void CHandStateShow::clearFingerColorEffect()
{
    m_maxValue = 0;
    for(int i = 0;i < 5;i++)
    {
        setFingerState(i,NORMAL_E);
    }
}
