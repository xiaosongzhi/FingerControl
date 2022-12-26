#ifndef CPRINTER_H
#define CPRINTER_H

#include <QWidget>
class QPrinter;

class CPrinter : public QWidget
{
    Q_OBJECT

public:
    explicit CPrinter(QWidget *parent = nullptr);
    ~CPrinter();

    //打印图片
    void printPixmap(QPixmap& pixmap);
    //打印页面
    void printPage( const QString& title,const QString& body);
    //打印预览
    void printPreview(QPixmap &pixmap);

private slots:
    void slotPrintPreview(QPrinter*);

private:

    QPixmap m_pixmap;
};

#endif // CPRINTER_H
