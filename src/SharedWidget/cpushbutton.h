#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>
class QMovie;
class QLabel;

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
    CPushButton(QWidget*parent = 0);

    /***函数 该button在使用之前必须要使用该函数，并且最好是在构造函数中调用
    ****参数 @filePath--gif路径   @frames--总帧数
    ****/
    void setMovieFilePathAndFrames(QString filePath,int frames);
    //设置是否选中
    void setSelected(bool isSelected);
    //获取是否被选中状态
    bool getSelectedState();
protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
public slots:
    void slotFrameChanged(int);
    void onClicked();
    void slotFinished();

signals:
    void signalSelected(bool);

private:
    QMovie *m_movie;
    int m_totalFrames;
    bool m_isSelected;

};

#endif // CPUSHBUTTON_H
