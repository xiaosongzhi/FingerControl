#ifndef DATAFORMATE_H
#define DATAFORMATE_H
#include <QWidget>
#include <QMap>
#define WIDTH 630
#define HEIGHT 450
#define MAXSTRENGTH 100
#define MINSTRENGTH (-100)
#define PACKHEAD 0xFE
typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QPair<DataList,QColor> DataPair;
typedef QList<QPair<DataList,QColor>> DataTable;

//用于生成报表数据
typedef  QMap<QDateTime,int> ReportData; //其中的int代表力量 或者是 关节活动度
typedef  QList<ReportData> ReportDataList;

//绘图类型
typedef enum
{
    AreaChart_E = 0,
    BarChart_E,
    PieChart_E,
    LineChart_E,
    SplineChart_E,
    ScatterChart_E
}E_ChartType;
typedef enum
{
    X_Axis = 0,
    Y_Axis
}E_AxisType;

typedef enum
{
    EMERGENCY_E,    //急停
    SPASM_E,        //痉挛
    CONNECTLOST_E,  //连接断开
    CHECKING_E,     //校准界面
    MOVETIPS_E,     //移动提示界面
    TOUCHWARNING_E  //接触风险
}E_WarningType;

//通信状态
typedef enum
{
    UnconnectedState_E = 0,
    HostLookupState_E,
    ConnectingState_E,
    ConnectedState_E,
    BoundState_E,
    ClosingState_E,
    ListeningState_E

}E_DeviceState;

//左右手定义
typedef enum
{
    LeftHand_E = 0,
    RightHand_E
}E_HandType;

typedef enum
{
    Buckling_E = -1,
    Stretch_E = 1
}E_BuckStretchState;

//手指定义
typedef enum
{
    Thumb_E = 0,
    ForeFinger_E,
    MiddleFinger_E,
    RingFinger_E,
    LittleFinger_E
}E_FingerIndex;

//手的颜色状态
typedef enum
{
    NORMAL_E = 0,
    EvaluateRed_E,
    EvaluateGreen_E,
    EvaluateYellow_E,
    TipsPurple_E,
    TipsGreen_E,
    DisableGray_E
}E_FingerState;

typedef enum
{
    Grey_E = 0,
    Green_E,
    Red_E,
    Blue_E
}E_Color;

//页面切换
typedef enum
{
    JointMotionScopePage_E = 0,
    TreatModePage_E,
    PassiveTrainingPage_E,
    BoostTrainingPage_E,
    ActiveTrainingPage_E,
    MemoryTrainingPage_E,
    TrainingGamePage_E,
    EvaluationPage_E,
    StrengthEvaluationPage_E,
    StrengthEvaluationResultPage_E,
    MotionScopeEvaluationPage_E,
    MotionScopeEvaluationResultPage_E,
    TrainingRecordPage_E,
    SettingPage_E,
    EmptyPage_E
}E_PAGENAME;

//协议命令
typedef enum
{
    HEARTBEAT = 0X0000,           	//心跳
    CHECKRESULT = 0X0001,         	//自检结果
    EMERGENCY = 0X0002,           	//急停报警
    SPASMWARNING = 0X0003,        	//痉挛报警
    SETTRAINMODE = 0x0100,			//设置训练模式
    CONTROLPARAMSET = 0x0101,       //主动训练和记忆训练中的力量设定
    MACHINEZEROCHECK = 0x1000,      //机械零点校准
    HANDFINGERSELECTED = 0x1001,    //左右手以及手指选择
    GETFINGERPOSITION = 0X1002,		//获取手指位置 int8_t  1-开始获取  0-停止获取
    RESPONDPOSITION = 0X1003,		//回复手指位置信息
    GETFINGERFORCE = 0X1004,        //获取手指力量大小
    RESPONDFORCE = 0X1005,			//回复手指力量大小
    CONTROLSIGLEFINGER = 0X1006,	//控制单个手指上下运动 int8_t  -1向下  1向上
    TESTFINGERSPOSITION = 0X1007,   //验证当前范围，(1五指朝外 2四指朝内 3拇指朝内)
    LOCKFINGERS  = 0X1008,			//锁定手指  0-解锁  1-锁定
    FINGERSRANGE = 0x1009,          //五指运动范围
    RESET_TRAIN_TEST = 0x100A,      //重新测试

    ASSISTANCE_MODE_FINGER_SELECT = 0x2000, //是四指还是拇指
    ASSISTANCE_MODE_HELP_TYPE = 0X2001,     //助力的等级 1简单、2中等、3困难
    START_ASSISTANCE_MODE = 0X2002,         //开始助力训练
    CLOSE_ASSISTANCE_MODE = 0X2003,         //结束助力训练
    ASSISTANCE_MOVE_DEST_POSITION = 0X2004, //助力模式告知运动到目标位置
    ASSISTANCE_REACH_TO_DEST = 0X2005,      //下位机告知到达目标位置

    START_ACTIVE_TRANNING_MODE = 0X2101,		//开始主动训练模式
    CLOSE_ACTIVE_TRANNING_MODE = 0X2102,		//结束主动训练模式
    ACTIVE_TRANNING_MODE_CTRL_TYPE = 0X2103,	//1位置模式，2力量模式、
    MOVE_FINGER_INDEX = 0x2104,                 //动哪个手指选择哪个，并返回。
    ACTIVE_TRAINNING_MODE_HELP_TYPE = 0X2105,	//主动训练模式的难易等级 1简单2中等3困难
    ACTIVE_TRAINNING_MODE_TIPS = 0X2106,          //告知下位机给出运动提示
    ACTIVE_TRAINNING_MODE_COMPLETE_TIPS = 0X2107,//下位机完成运动提示
    ACTIVE_TRAINNING_MODE_REBACK = 0x2108,      //回到初始位置

    SELECT_PASSIVE_TRANNING_FINGER = 0X2200,	//选择
    PASSIVE_TRANNING_MODE_PARAMETER = 0X2201,	//被动训练参数
    START_PASSIVE_TRANNING_MODE = 0X2202,		//开始被动训练
    CLOSE_PASSIVE_TRANNING_MODE = 0X2203,		//结束被动训练
    PASSIVE_CONTROL_TO_DESTPOSITION = 0X2204,   //控制单个手指到给定目标位置
    PASSIVE_CONTROL_ALL_FINGERS = 0X2205,       //控制五个手指同时运动

    START_MEMORY_TRANNING_MODE = 0X2301,	//开始记忆训练
    CLOSE_MEMORY_TRANNING_MODE = 0X2302,	//结束记忆训练
    MEMORY_MODE_CTRL_MODE = 0X2303,			//1位置模式，2力的模式
    MEMORY_MODE_HELP_TYPE = 0X2304,			//训练的难易程度
    MEMORY_MODE_FORCE_TYPE = 0X2305,		//记忆训练力的类型，1伸展力，2屈曲力    
    MEMORY_MODE_TRANNING_CYCLE = 0X2306,	//当力超限时，是否继续运行。
    MEMORY_MODE_MOVE_TIPS = 0X2307,         //记忆训练运动提示
    MEMORY_MODE_REACH_TO_DEST = 0X2308,     //回到初始位置

    START_FORCE_ASSESS_MODE = 0X3001,			//力量评估模式开始
    CLOSE_FORCE_ASSESS_MODE = 0X3002,			//力量评估模式结束

    START_MOTION_RANG_JUDGE = 0X3101,			//开始关节活动度评定
    CLOSE_MOTION_RANG_JUDGE = 0X3102,			//结束关节活动度评定。
    MOTION_RANG_JUDGE_FINGER_TYPE = 0x3103,     //手指类型，四指或者拇指
    MOTION_RANG_JUDGE_EXTEND_FORCE = 0x3104,	//伸展力数值
    MOTION_RANG_JUDGE_FLEXION_FORCE =0x3105,    //屈曲力数值

}E_CMDID;

//训练模式
typedef enum
{
    TRAINMODE_PASSIVE_E = 0,    //被动
    TRAINMODE_ACTIVE_E,         //主动
    TRAINMODE_BOOST_E,          //助力
    TRAINMODE_MEMORY_E,         //记忆
    TRAINMODE_STRENGTH_E,       //力量评定
    TRAINMODE_SCOPE_E,          //关节活动度评定
    TRAINMODE_GAME_E            //游戏
}E_TRAINMODE;

//串口配置
typedef struct ST_SerialPortConfig
{
    QString portName;   //端口名
    int32_t baud;       //波特率
    int8_t dataBits;    //数据位
    int8_t parity;      //奇偶校验
    int8_t stopBit;     //停止位
    int8_t flowControl; //流控
    ST_SerialPortConfig()
    {
        portName = "";
        baud = 0;
        dataBits = 0;
        parity = 0;
        stopBit = 0;
        flowControl = 0;
    }

}ST_SerialPortConfig;

//数据库配置
typedef struct ST_DataBaseConfig
{
    QString IP;
    int16_t port;
    QString userName;   //用户名
    QString password;   //密码
    ST_DataBaseConfig()
    {
        IP = "";
        port = 0;
        userName = "";
        password = "";
    }

}ST_DataBaseConfig;

#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐

//发送左右手以及手指被选中的状态
typedef struct
{
    int8_t handType;    //0-左手 1-右手
    int8_t thumb;       //0-未被选中  1-被选中
    int8_t forfinger;
    int8_t middlefinger;
    int8_t ringfinger;
    int8_t littlefinger;
}ST_HandTypeFingers;

//控制单个手指上下运动
typedef struct
{
    int8_t fingerIndex; //手指下标 0-4 拇指到小指
    int8_t direction;   //控制方向 -1向下  1向上
}ST_FingerUpDownControl;

//控制单个手指到目标位置
typedef struct
{
    int8_t fingerIndex; //手指下标 0-4 拇指到小指
    uint8_t  position;//目标位置
}ST_FingerDestPosition;

//手指目标位置
typedef struct
{
    uint8_t  	thumb;
    uint8_t  	forefinger;
    uint8_t 	middlefinger;
    uint8_t 	ringfinger;
    uint8_t 	littlefinger;
    uint8_t array[5];
}ST_DestPosition;

//关节活动度测试 下位机<--->上位机
typedef struct
{
    int8_t  thumb;	 //range 0~100 -1为不操控当前手指
    int8_t  forefinger;
    int8_t 	middlefinger;
    int8_t 	ringfinger;
    int8_t 	littlefinger;
}ST_FingersPosition;

//手指位置（不分左右手）
typedef struct
{
    int8_t  thumb;
    int8_t  forefinger;
    int8_t 	middlefinger;
    int8_t 	ringfinger;
    int8_t 	littlefinger;
}ST_HandFingersPosition;

//实时位置采集
typedef struct
{
    uint8_t  	Lthumb;
    uint8_t  	Rthumb;
    uint8_t  	forefinger;
    uint8_t 	middlefinger;
    uint8_t 	ringfinger;
    uint8_t 	littlefinger;
}ST_RealTimeFingersPosition;

//实时力量采集
typedef struct
{
    int16_t  	thumb;
    int16_t  	forefinger;
    int16_t 	middlefinger;
    int16_t 	ringfinger;
    int16_t 	littlefinger;
}ST_RealTimeStrength;

//单个手指关节位置调整  上位机---->下位机
typedef struct
{
    uint8_t  fingerIndex; //0~5 依次为左拇指、右拇指、食指、中指、无名指、小指
    int8_t   flag;			//-1向下    1向上
}ST_SetFingerPosiotion;

//关节活动设定-锁定按钮
typedef struct
{
    int8_t  lock; // 0-锁定  1-解锁
}ST_FingerLock;

//各手指的运动范围
typedef struct
{
    int8_t fingersRange[10];//[0]拇指min [1]拇指max [2]食指min [3]食指max ...
}ST_FingersRange;

//被动训练参数
typedef struct
{
    int8_t  speed;//速度
    uint8_t  strength;//力量极限
}ST_PassiveParam;

//被动模式目标位置
typedef struct
{
    int8_t flag;     //-1-向下 1-向上
    int8_t array[5]; //array[0]~array[4]代表拇指到小指
}ST_PassiveDestPosition;

//主动训练和记忆训练中力量设定
typedef struct
{
    int8_t 	buckling;   //屈曲力等级 1~3
    int8_t 	stretch;    //伸展力等级 1~3
    int8_t 	controlType;//控制方式 1~趋势  2~位移
}ST_ControlSet;

#pragma pack(pop)//恢复对齐状态

//主动模式游戏参数
typedef struct
{
    int gameType;                   //游戏类型
    QString gameTrack;              //运动轨迹
    QString scorpPath;              //运动范围
    int scoreSize;                  //运动范围大小 1-小 2-中 3-大
    int blockLength;                //障碍物长度 1-短 2-中 3-长
    QMap<int,int> trainProcessMap;  //训练流程 QMap<训练方式，时间>
    int level;                      //游戏关卡
    E_TRAINMODE trainMode;          //训练模式
}ST_ActiveParam;

/***********与游戏交互**************/
//被动模式游戏参数
//typedef struct
//{
//    int gameType;                   //游戏类型
//    QString gameTrack;              //运动轨迹
//    QString scorp;                  //运动范围
//    QMap<int,int> trainProcessMap;  //训练流程
//    int speed;                      //速递大小 1-低速 2-中速 3-高速
//    int level;                      //游戏关卡
//    E_TRAINMODE trainMode;          //训练模式
//}ST_PassiveParam;

//助力模式游戏参数
typedef struct
{
    int gameType;                   //游戏类型
    QString gameTrack;              //运动轨迹
    QString scorp;                  //运动范围
    QMap<int,int> trainProcessMap;  //训练流程
    int level;                      //游戏关卡
    E_TRAINMODE trainMode;          //训练模式
}ST_BoostParam;

//阻抗模式游戏参数
typedef struct
{
    int gameType;                   //游戏类型
    QString gameTrack;              //运动轨迹
    QString scorp;                  //运动范围
    int resistance;                 //阻力
    QMap<int,int> trainProcessMap;  //训练流程
    int level;                      //游戏关卡
    E_TRAINMODE trainMode;          //训练模式
}ST_AgainstParam;

extern void  Pressure_CheckCRC(uint8_t*buf,int len,uint8_t* CRC_H,uint8_t* CRC_L);
extern void Sleep(int msec);
#endif // DATAFORMATE_H
