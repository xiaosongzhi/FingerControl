#ifndef USERDATAFORMATE_H
#define USERDATAFORMATE_H

//此处存放与数据库有关的内容

#include <QWidget>
#include <QDateTime>
#include "dataFormate.h"
#define PERCOUNT 8  //每页放8个处方

typedef struct PatientMsg
{
    uint32_t ID;                    //自增性ID，由系统分配控制
    QString name;                   //患者姓名 必填
    int age;                        //患者年龄
    int sex;                        //0-男性 1-女性
    int height;                     //患者身高
    int weight;                     //患者体重
    int hands;                      //0-左 1-右 2-双
    QString hospitalDepartment;     //主治科室
    QString hospitalNumber;         //住院号
    int healthInsuranceType;        //医保类型
    QString healthInsuranceNumber;  //医保账号
    QString phone;                  //患者电话
    QString address;                //家庭住址
    int certificateType;            //0-身份证 1-护照  3-军官证
    QString certificateNum;         //证件号码
    QString remarkMessage;          //备注信息

}ST_PatientMsg;

//此处该做优化，优化成满编状态
typedef struct HandData
{
    int hand;                   //0-左手 1-右手
    QList<int> fingers;         //0-4 分别代表从拇指到小指，举例1-2-3代表下标为1,2,3的手指被选中
    int8_t fingerSelected[5];   //手指是否被选中0-未被选中 1-被选中
    HandData()
    {
       hand = -1;
       fingers.clear();
    }
}ST_HandData;

//手指关节活动度
typedef struct JointRange
{
    int ID;                                 //患者ID
    int hand;                               //0-左手 1-右手
    QString fingers;                        //0-4 分别代表从拇指到小指，举例1-2-3代表下标为1,2,3的手指被选中
    QMap<int,QPair<int,int>>fingersRange;   //手指的活动范围0~4代表拇指到小指
}ST_JointRange;


//力量评定结果
typedef struct FringerStrengthResult
{
    int ID;                     //患者ID
    int hand;                   //0-左手 1-右手
    QDateTime dateTime;         //测试时间
    QMap<int,QPair<float,float>> mapFingerStrength;//QMap<手指序号，QPair<屈曲，伸展>>
    float grip;                 //握力
    float tension;              //张力
    QString resultImagePath;    //结果图片路径
}ST_FringerStrengthResult;

//关节活动度评定结果
typedef struct JointRangeResult
{
    int ID;                     //患者ID
    int hand;                   //0-左手 1-右手
    QDateTime startDateTime;    //测试时间
    QDateTime endDateTime;      //测试时间
    QMap<int,int>mapMovePercent;//QMap<手指序号，占到活动范围的百分比>
    int totalPercent;           //关节总活动度百分比
    QString resultImagePath;    //图片结果路径
}ST_JointRangeResult;

typedef struct TrainReport
{
    int ID;                 //系统ID
    QString name;           //患者姓名
    int level;              //当前等级
    QDateTime startTime;    //开始时间
    QDateTime endTime;      //结束时间
    E_TRAINMODE trainMode;  //训练模式  0-被动训练 1-主动训练 2-助力训练 3-记忆训练 4-力量评定 5-关节活动度评定 6-游戏
    int duration;           //训练时长
    QString markMsg;        //备注
    QString resultImagePath;//图片结果路径
    TrainReport()
    {
        level = 1;
    }
}ST_TrainReport;

extern ST_PatientMsg variantMapToPatientMsg(QVariantMap vMap);
extern ST_JointRange variantMapToJointRange(QVariantMap vMap);
extern ST_TrainReport variantMapToTrainReport(QVariantMap vMap);
extern ST_FringerStrengthResult variantMapToFringerStrengthResult(QVariantMap vMap);
extern ST_JointRangeResult variantMapToJointRangeResult(QVariantMap vMap);

extern QVariantMap patientMsgToVariantMap(ST_PatientMsg);
extern QVariantMap jointRangeToVariantMap(ST_JointRange);
extern QVariantMap trainReportToVariantMap(ST_TrainReport);
extern QVariantMap fringerStrengthResultToVariantMap(ST_FringerStrengthResult);
extern QVariantMap jointRangeResultToVariantMap(ST_JointRangeResult);

#endif // USERDATAFORMATE_H
