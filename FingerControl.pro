QT       += core gui network serialport sql printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

UI_DIR = ./UI
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src/MainWindow \
               $$PWD/src/SharedModule \
               $$PWD/src/UserManager \
               $$PWD/src/communication \
               $$PWD/src/DataFormate \
               $$PWD/src/DataBaseControl \
               $$PWD/src/TreatMode \
               $$PWD/src/SharedWidget \
               $$PWD/src/Evaluation \
               $$PWD/src/TrainingRecord \
               $$PWD/src/TrainingGame \
               $$PWD/src/MotionScope \
               $$PWD/src/ReportForms

INCLUDEPATH += $$PWD/src/log4j/include \
               $$PWD/src/log4j/src \
               $$PWD/src/log4j/src/log4qt


SOURCES += \
    src/DataBaseControl/cdatabaseinterface.cpp \
    src/DataBaseControl/mysqldatabase.cpp \
    src/DataFormate/dataFormate.cpp \
    src/DataFormate/userdataformate.cpp \
    src/Evaluation/evaluationmanager.cpp \
    src/Evaluation/motionscopeevaluation.cpp \
    src/Evaluation/motionscopeevaluationresult.cpp \
    src/Evaluation/strengthevaluation.cpp \
    src/Evaluation/strengthevaluationresult.cpp \
    src/MainWindow/main.cpp \
    src/MainWindow/mainwindow.cpp \
    src/MainWindow/mainwindowpagecontrol.cpp \
    src/MotionScope/jointmotionscope.cpp \
    src/MotionScope/machinecheckdialog.cpp \
    src/ReportForms/cprinter.cpp \
    src/ReportForms/reportforms.cpp \
    src/ReportForms/tablecontent.cpp \
    src/ReportForms/tabletitle.cpp \
    src/SharedModule/cbasedialog.cpp \
    src/SharedModule/cmaskdialog.cpp \
    src/SharedModule/controlmaskdialog.cpp \
    src/SharedModule/cpassworddialog.cpp \
    src/SharedModule/displaywindow.cpp \
    src/SharedModule/warningdialog.cpp \
    src/SharedWidget/areawidget.cpp \
    src/SharedWidget/chandscopewidget.cpp \
    src/SharedWidget/chandstateshow.cpp \
    src/SharedWidget/clabel.cpp \
    src/SharedWidget/cprocessbar.cpp \
    src/SharedWidget/cpushbutton.cpp \
    src/SharedWidget/ctraininghand.cpp \
    src/SharedWidget/levelcontrol.cpp \
    src/SharedWidget/linechart.cpp \
    src/SharedWidget/movescopeprogressbar.cpp \
    src/SharedWidget/newareawidget.cpp \
    src/SharedWidget/passiveresutexpression.cpp \
    src/SharedWidget/verticalprogressbar.cpp \
    src/SharedWidget/virtualhandle.cpp \
    src/TrainingGame/traininggame.cpp \
    src/TrainingRecord/traininghistory.cpp \
    src/TreatMode/activetraining.cpp \
    src/TreatMode/boosttraining.cpp \
    src/TreatMode/controlset.cpp \
    src/TreatMode/memorytraining.cpp \
    src/TreatMode/passivetraining.cpp \
    src/TreatMode/treatmodemanager.cpp \
    src/UserManager/currentuserdata.cpp \
    src/UserManager/edituser.cpp \
    src/UserManager/userfingerselect.cpp \
    src/UserManager/userhandselect.cpp \
    src/UserManager/usermanager.cpp \
    src/communication/ccommunicateapi.cpp \
    src/communication/ccommunicationinterface.cpp \
    src/communication/cserialportinterface.cpp \
    src/communication/ctcpsocketinterface.cpp \
    src/communication/cudpinterface.cpp \
    src/communication/globalqueue.cpp \
    src/communication/readconfig.cpp

HEADERS += \
    src/DataBaseControl/cdatabaseinterface.h \
    src/DataBaseControl/mysqldatabase.h \
    src/DataFormate/dataFormate.h \
    src/DataFormate/userdataformate.h \
    src/Evaluation/evaluationmanager.h \
    src/Evaluation/motionscopeevaluation.h \
    src/Evaluation/motionscopeevaluationresult.h \
    src/Evaluation/strengthevaluation.h \
    src/Evaluation/strengthevaluationresult.h \
    src/MainWindow/mainwindow.h \
    src/MainWindow/mainwindowpagecontrol.h \
    src/MotionScope/jointmotionscope.h \
    src/MotionScope/machinecheckdialog.h \
    src/ReportForms/cprinter.h \
    src/ReportForms/reportforms.h \
    src/ReportForms/tablecontent.h \
    src/ReportForms/tabletitle.h \
    src/SharedModule/cbasedialog.h \
    src/SharedModule/cmaskdialog.h \
    src/SharedModule/controlmaskdialog.h \
    src/SharedModule/cpassworddialog.h \
    src/SharedModule/displaywindow.h \
    src/SharedModule/warningdialog.h \
    src/SharedWidget/areawidget.h \
    src/SharedWidget/chandscopewidget.h \
    src/SharedWidget/chandstateshow.h \
    src/SharedWidget/clabel.h \
    src/SharedWidget/cprocessbar.h \
    src/SharedWidget/cpushbutton.h \
    src/SharedWidget/ctraininghand.h \
    src/SharedWidget/levelcontrol.h \
    src/SharedWidget/linechart.h \
    src/SharedWidget/movescopeprogressbar.h \
    src/SharedWidget/newareawidget.h \
    src/SharedWidget/passiveresutexpression.h \
    src/SharedWidget/verticalprogressbar.h \
    src/SharedWidget/virtualhandle.h \
    src/TrainingGame/traininggame.h \
    src/TrainingRecord/traininghistory.h \
    src/TreatMode/activetraining.h \
    src/TreatMode/boosttraining.h \
    src/TreatMode/controlset.h \
    src/TreatMode/memorytraining.h \
    src/TreatMode/passivetraining.h \
    src/TreatMode/treatmodemanager.h \
    src/UserManager/currentuserdata.h \
    src/UserManager/edituser.h \
    src/UserManager/userfingerselect.h \
    src/UserManager/userhandselect.h \
    src/UserManager/usermanager.h \
    src/communication/ccommunicateapi.h \
    src/communication/ccommunicationinterface.h \
    src/communication/cserialportinterface.h \
    src/communication/ctcpsocketinterface.h \
    src/communication/cudpinterface.h \
    src/communication/globalqueue.h \
    src/communication/readconfig.h

FORMS += \
    src/Evaluation/evaluationmanager.ui \
    src/Evaluation/motionscopeevaluation.ui \
    src/Evaluation/motionscopeevaluationresult.ui \
    src/Evaluation/strengthevaluation.ui \
    src/Evaluation/strengthevaluationresult.ui \
    src/MainWindow/mainwindow.ui \
    src/MotionScope/jointmotionscope.ui \
    src/MotionScope/machinecheckdialog.ui \
    src/ReportForms/reportforms.ui \
    src/ReportForms/tablecontent.ui \
    src/ReportForms/tabletitle.ui \
    src/SharedModule/cbasedialog.ui \
    src/SharedModule/cpassworddialog.ui \
    src/SharedModule/displaywindow.ui \
    src/SharedModule/warningdialog.ui \
    src/SharedWidget/chandscopewidget.ui \
    src/SharedWidget/chandstateshow.ui \
    src/SharedWidget/ctraininghand.ui \
    src/SharedWidget/levelcontrol.ui \
    src/SharedWidget/linechart.ui \
    src/SharedWidget/movescopeprogressbar.ui \
    src/SharedWidget/newareawidget.ui \
    src/SharedWidget/passiveresutexpression.ui \
    src/SharedWidget/verticalprogressbar.ui \
    src/SharedWidget/virtualhandle.ui \
    src/TrainingGame/traininggame.ui \
    src/TrainingRecord/traininghistory.ui \
    src/TreatMode/activetraining.ui \
    src/TreatMode/boosttraining.ui \
    src/TreatMode/controlset.ui \
    src/TreatMode/memorytraining.ui \
    src/TreatMode/passivetraining.ui \
    src/TreatMode/treatmodemanager.ui \
    src/UserManager/edituser.ui \
    src/UserManager/userfingerselect.ui \
    src/UserManager/userhandselect.ui \
    src/UserManager/usermanager.ui

TRANSLATIONS += FingerControl_EN.ts \
                FingerControl_CN.ts

#LIBS += -L$$PWD/src/log4j/bin -llog4qt
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#使用中文的编码配置
#QMAKE_CXXFLAGS += /execution-charset:utf-8   /source-charset:utf-8

RESOURCES += \
    source.qrc

RC_ICONS = 128_128.ico
