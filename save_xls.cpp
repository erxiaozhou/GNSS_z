#include "save_xls.h"


SaveExcel::SaveExcel()
{
    *tablerow <<"频谱主搬距频带中心的频偏（MHz）"<<"90%功率的带宽(dBW/Hz)"<<"RMS带宽（MHz）"
            <<"等效矩形带宽（MHz）"<<"与自身的频谱隔离系数(dB/Hz)"<<"与1.023MHzBPSK的频谱隔离系数(dB/Hz)"
            <<"与BOC（10,5）的频谱隔离系数(dB/Hz)"<<"与XXX的频谱隔离系数(dB/Hz)";
}
QStringList * SaveExcel::tablerow=new QStringList;
//将QStringList中信息转存至excel
bool SaveExcel::save(QString filePath, QStringList headers, QList<QStringList> data,QString comment)
{
    QString sheetName = "Sheet";

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","excelexport");//定义一个QDBC数据库
    if( !db.isValid())
    {

        return false;   //! type error
    }

    //数据库名称设置
    QString dsn = "DRIVER={Microsoft Excel Driver (*.xls)};"
            "DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\""+filePath+"\";DBQ="+filePath;
    db.setDatabaseName( dsn);

    //打开db，否则退出
    if( !db.open())
    {
        return false;  //! db error
    }

    QSqlQuery query(db);
    QString sql;

    //drop旧版本
    sql = QString("DROP TABLE [%1]").arg(sheetName);
    query.exec( sql);
    //create新版本
    sql = QString("CREATE TABLE [%1] (").arg(sheetName);
    //将表头加入数据库
    foreach (QString name, headers) {
        sql +=QString("[%1] varchar(200)").arg(name);
        if(name!=headers.last())
            sql +=",";
    }
    //qDebug()<<headers.size()<<"headers.size()";
    sql += ")";
    //qDebug()<<sql;
    query.prepare( sql);

    if( !query.exec()) {
        db.close();
        //qDebug()<<"！ERROR in exec headers ";
        return false;
    }
    //添加数据
    foreach (QStringList slist, data) {
        insert(query,sheetName,slist);
    }

    //添加备注
    if(!comment.isEmpty())
        {
            QStringList slist;
            slist<<comment;
            for(int i=0,n=headers.size()-1;i<n;i++)
            {
                slist<<"";
            }
            insert(query,sheetName,slist);
        }

    //关闭数据库
    db.close();
    return true;
}
//将QTableView保存为QStringList
bool SaveExcel::saveFromTable(QString filePath,QTableWidget *tableWidget, QString comment)
{
    QAbstractItemModel* model=tableWidget->model();//用model读取表格数据
    const int column=model->columnCount();
    const int row=model->rowCount();
    //qDebug()<<"col:"<<column<<"  row:"<<row;//col列 1  row行 8

    //header
    QStringList headers;
    headers<<"参数";
    for(int i=0;i<column;i++)
    {
        headers<<model->headerData(i,Qt::Horizontal).toString();
    }

    //data
    QStringList list;
    QList<QStringList> data;
    //用model遍历读取表格数据
    for(int i=0;i<row;i++)
    {
        if(model->index(i,0).data().isNull())
            continue;
        list.clear();
        list<<tablerow->at(i);
        for(int j=0;j<column;j++)
        {

            list<<model->index(i,j).data().toString();
        }
        data<<list;
    }
     //Debug信息
    //qDebug()<<data<<"data";
    //qDebug()<<headers<<"headers";

    return SaveExcel::save(filePath,headers,data,comment);
}
bool SaveExcel::insert(QSqlQuery &query, QString sheetName, QStringList slist)
{
    QString sSql = QString("INSERT INTO [%1] VALUES(").arg( sheetName);//数据库操作，插入新数据
        for(int i=0,n=slist.size();i<n;i++)
        {
            sSql+=QString(":%1").arg(i);
            if(i!=n-1)
                sSql+=",";
            else
                sSql+=")";
        }
        query.prepare( sSql);
        for(int i=0,n=slist.size();i<n;i++)
        {
            query.bindValue(QString(":%1").arg(i),slist.at(i));
           // qDebug()<<sSql<<" "<<QString(":%1").arg(i)<<"   "<<slist.at(i);
        }

        if( !query.exec()) {
         //qDebug()<<"!！ERROR in exec insert";
            return false;
        }
        return true;
}
