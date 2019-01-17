#ifndef ODBCEXCEL_H
#define ODBCEXCEL_H

#include <QObject>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QDebug>
class SaveExcel
{
public:
    SaveExcel();
    //将QStringList中信息转存至excel
    bool static save(QString filePath,QStringList headers,QList<QStringList> data,QString comment="");
    //将QTableView保存为QStringList
    bool static saveFromTable(QString filePath,QTableWidget *tableWidget,QString comment="");
    //获取错误信息

private:
    QStringList static * tablerow;
    bool static insert(QSqlQuery& query, QString sheetName, QStringList slist);

};

#endif // ODBCEXCEL_H
