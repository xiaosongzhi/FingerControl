#include "cpushbutton.h"
#include <QMovie>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
#include <QDebug>
CPushButton::CPushButton(QWidget *parent)
    :QPushButton(parent),
      m_totalFrames(0),
      m_isSelected(false)
{
    resize(60,60);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    QLabel* label = new QLabel();
    label->setAlignment(Qt::AlignHCenter);
    m_movie = new QMovie();
    //此处设置为全帧缓存，这个是在各帧之间跳转的前提
    m_movie->setCacheMode(QMovie::CacheAll);
    m_movie->setScaledSize(size());
    label->setMovie(m_movie);
    layout->addWidget(label);
    connect(m_movie,SIGNAL(frameChanged(int)),this,SLOT(slotFrameChanged(int)));
    connect(this,SIGNAL(clicked()),this,SLOT(onClicked()));
    connect(m_movie,SIGNAL(finished()),this,SLOT(slotFinished()));
}

void CPushButton::setMovieFilePathAndFrames(QString filePath,int frames)
{
    m_totalFrames = frames;
    m_movie->setFileName(filePath);
    m_movie->jumpToFrame(0);
}

void CPushButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    //未被选中时，开始播放gif
    if(!m_isSelected)
    {
        //跳到第1帧
        m_movie->jumpToNextFrame();

        m_movie->start();
    }
}

void CPushButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(!m_isSelected)
    {
        m_movie->stop();
        m_movie->jumpToFrame(0);
    }
}

void CPushButton::slotFrameChanged(int frameNumber)
{
    //未被选中,播放gif时要跳过最后一帧和第一帧
    if((!m_isSelected) && (frameNumber == (m_totalFrames-1)))
    {
        m_movie->jumpToFrame(1);
        m_movie->start();
        m_movie->jumpToFrame(1);
    }
}

void CPushButton::onClicked()
{
    m_isSelected = !m_isSelected;

    if(m_isSelected)
    {
        m_movie->setPaused(true);
        m_movie->jumpToFrame(m_totalFrames-1);
        emit signalSelected(true);
    }
    else
    {
        m_movie->jumpToFrame(0);
        m_movie->start();
        emit signalSelected(false);
    }
}

void CPushButton::setSelected(bool isSelected)
{
    //被选中
    if(isSelected)
    {
        m_isSelected = true;
    }
    else//失去焦点状态
    {
        m_movie->jumpToFrame(0);
        m_isSelected = false;
    }
}

bool CPushButton::getSelectedState()
{
    return m_isSelected;
}

void CPushButton::slotFinished()
{
//    qDebug()<<"finished";
}

