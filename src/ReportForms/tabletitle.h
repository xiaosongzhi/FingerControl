#ifndef TABLETITLE_H
#define TABLETITLE_H

/********报表表头类*********/

#include <QWidget>
#include "userdataformate.h"

namespace Ui {
class TableTitle;
}

class TableTitle : public QWidget
{
    Q_OBJECT

public:
    explicit TableTitle(QWidget *parent = nullptr);
    ~TableTitle();

    //清除报表头
    void clearTitle();

    //设置报表头信息
    void setTitleMessage(const ST_PatientMsg &st_patientMsg);


private:
    Ui::TableTitle *ui;
};

#endif // TABLETITLE_H
