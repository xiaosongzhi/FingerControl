#include "cprinter.h"
#include <QPrinterInfo>
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QTextDocument>

CPrinter::CPrinter(QWidget *parent) :
    QWidget(parent)
{

}

CPrinter::~CPrinter()
{


}

//打印图片
void CPrinter::printPixmap(QPixmap& pixmap)
{
    //将打印机的分辨率设置为屏幕的分辨率
    QPrinter printer(QPrinter::ScreenResolution);
    //开启一个打印窗口
    QPrintDialog printDialog(&printer);
    //是否为模态对话框
    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter;
        painter.begin(&printer);
        QRect viewRect = painter.viewport();
        QSize size = pixmap.size();
        size.scale(viewRect.size(),Qt::KeepAspectRatio);
        painter.drawPixmap(60,50,pixmap);
        painter.end();
    }
}
//打印页面（将数据转化为HTML，使用Qt的富文本引擎QTextDocument来进行显示）
void CPrinter::printPage(const QString& title,const QString& body)
{
    QString html;
    html += "<table width=\"100%\" border=1 cellspacing=0>\n"
            "<tr><td bgcolor=\"lightgray\"><font size=\"+1\">"
            "<b><i>" + title +"</i></b><\font>\n<tr><td>" + body +"\n</table>\n<br>\n";
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec())
    {
        QTextDocument textDocument;
        textDocument.setHtml(html);
        textDocument.print(&printer);
    }

}
//打印预览
void CPrinter::printPreview(QPixmap &pixmap)
{
    m_pixmap = pixmap;
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintPreviewDialog previewDialog(&printer,this);
    connect(&previewDialog,SIGNAL(paintRequested(QPrinter*)),this,SLOT(slotPrintPreview(QPrinter*)));
    previewDialog.exec();
}

void CPrinter::slotPrintPreview(QPrinter* printer)
{
    printer->setOrientation(QPrinter::Portrait);
    QPainter painter(printer);

    painter.begin(printer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QRect rect = painter.viewport();
    float x = (float)rect.width()/m_pixmap.width();
    float y = (float)rect.height()/m_pixmap.height();

    painter.scale(x,y);
    painter.drawPixmap(0,0,m_pixmap);
    painter.end();
}
