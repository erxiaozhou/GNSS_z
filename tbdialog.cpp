#include "tbdialog.h"
#include "ui_tbdialog.h"
#include "qdebug.h"
#include"save_xls.h"
#include <QDesktopWidget>

TbDialog::TbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TbDialog)
{
    ui->setupUi(this);
    qDebug()<<"before ini";
    AllTable_ini();
    qDebug()<<"before ini";

    QRect deskRect = QApplication::desktop()->availableGeometry();
    WT=deskRect.width();
    HT=deskRect.height();//-25

    QRect wsize=this->geometry();
    setPartPos(wsize,3,3,7,5.5);//wsize.setRect(0,30,WT,HT);
    this->setGeometry(wsize);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(false);
    QHeaderView* headerView2=ui->tableWidget->horizontalHeader();
    headerView2->setHidden(false);

}

TbDialog::~TbDialog()
{
    delete ui;
}

void TbDialog::on_btn_close_clicked()
{
    this->close();
}
void TbDialog::AllTable_ini()
{
    //设置表头
    TableDataIni();
    QTableWidgetItem *headerItem;
    QStringList headerText;
    headerText<<"1.023MHz BPSK"<<"10.23MHz BPSK"<<"BOC(5,2)"<<"BOC(8,4)"<<"BOC(10,5)";
   //ui->tableWidget->setHorizontalHeaderLabels(headerText);
    ui->tableWidget->setColumnCount(headerText.count());
    for (int i=0;i<ui->tableWidget->columnCount();i++)
    {
        headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();
        font.setBold(true);

        ui->tableWidget->setHorizontalHeaderItem(i,headerItem);
    }
    //初始化数据
    ui->tableWidget->clearContents();//工作区清理
    int Rows=ui->tableWidget->rowCount();
    for (int i =0;i<Rows;i++)
    {
        QTableWidgetItem *item;
        QString str;

        str.setNum(tabledata[i][0]);
        item=new QTableWidgetItem(str,TbDialog::cta);
        item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i,TbDialog::cola,item);

        str.setNum(tabledata[i][1]);
        item=new QTableWidgetItem(str,TbDialog::ctb);
        item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i,TbDialog::colb,item);

        str.setNum(tabledata[i][2]);
        item=new QTableWidgetItem(str,TbDialog::ctc);
        item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i,TbDialog::colc,item);

        str.setNum(tabledata[i][3]);
        item=new QTableWidgetItem(str,TbDialog::ctd);
        item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i,TbDialog::cold,item);

        str.setNum(tabledata[i][4]);
        item=new QTableWidgetItem(str,TbDialog::cole);
        item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i,TbDialog::cole,item);
    }
}
void TbDialog::TableDataIni()
{
     for(int i=0;i<tabledata_hei;i++)
     {
         for(int j=0;j<tabledata_wei;j++)
         {
             tabledata[i][j]=i*10+j;
         }
     }
     tabledata[0][0]=0;
     tabledata[1][0]=-60.0988;
     tabledata[2][0]=1.6;
     tabledata[3][0]=1.12756;
     tabledata[4][0]=1.01428;
     tabledata[5][0]=-61.8225;
     tabledata[6][0]=-61.8225;
     tabledata[7][0]=-86.7686;
     tabledata[8][0]=0.00733025;

     tabledata[0][1]=0;
     tabledata[1][1]=-70.0988;
     tabledata[2][1]=12.1;
     tabledata[3][1]=3.47676;
     tabledata[4][1]=9.26208;
     tabledata[5][1]=-71.4406;
     tabledata[6][1]=-70.2094;
     tabledata[7][1]=-79.8416;
     tabledata[8][1]=0.107132;

     tabledata[0][2]=4.8753;
     tabledata[1][2]=-66.8273;
     tabledata[2][2]=119.04;
     tabledata[3][2]=4.80265;
     tabledata[4][2]=3.99217;
     tabledata[5][2]=-68.549;
     tabledata[6][2]=-77.8489;
     tabledata[7][2]=-83.8036;
     tabledata[8][2]=0.148756;

     tabledata[0][3]=7.596;
     tabledata[1][3]=-69.7282;
     tabledata[2][3]=18.94;
     tabledata[3][3]=7.53079;
     tabledata[4][3]=7.80853;
     tabledata[5][3]=-71.3915;
     tabledata[6][3]=-85.9201;
     tabledata[7][3]=-73.0815;
     tabledata[8][3]=0.00964393;

     tabledata[0][4]=9.495;
     tabledata[1][4]=-70.6973;
     tabledata[2][4]=23.84;
     tabledata[3][4]=9.05641;
     tabledata[4][4]=8.95838;
     tabledata[5][4]=-72.1217;
     tabledata[6][4]=-87.9064;
     tabledata[7][4]=-72.1217;
     tabledata[8][4]=0.372497;

}
void TbDialog::on_btn_updata_clicked()
{
    TableDataIni();  //先暂时这么写
}
void TbDialog::setPartPos(QRect &rect,qreal x,qreal y,qreal wt,qreal ht)
{
    rect.setRect(0+qRound(WT*x/11.5),30+qRound(HT*y/8.5),qRound(WT*wt/11.5),qRound(HT*ht/8.5));
}
void TbDialog::on_btn_save_clicked()   //保存表格的函数里，目前表格名称是钉死的，这个要改   下面注释掉的代码是弹窗保存
{
    //获取tablewidget的像素分布
    QScreen * screen = QGuiApplication::primaryScreen();
    QPixmap p = screen->grabWindow(ui->tableWidget->winId());//ui->chartView->winId()
    QImage image = p.toImage();

    QStringList tail;
    tail<<"*.xls"<<"*.png"<<"*.jpg"<<"*.bmp";

    QString filePath=get_save_dir(tail);//获取保存的路径
    int pot_index=filePath.indexOf(".");
    QString user_tail=filePath.mid(pot_index);//用户输入的获取后缀名

    //由后缀定保存方式
    if (user_tail==".png")
        MessageOfSave(image.save(filePath,"PNG"));
    else if (user_tail==".jpg")
        MessageOfSave(image.save(filePath,"JPG"));
    else if (user_tail==".bmp")
        MessageOfSave(image.save(filePath,"BMP"));
    else if (user_tail==".xls")
        MessageOfSave(saveFromTable(filePath,ui->tableWidget,"注释：无"));
    else
        MessageOfSave(false);//后缀不合规范则保存失败                   //这里也可以细划分
}
void TbDialog::MessageOfSave(bool saveOK)
{
    if(saveOK)
    {
        QMessageBox::information(this,tr("提示"),tr("保存成功"));
    }
    else
        QMessageBox::information(this,tr("提示"),tr("保存失败"));
}
QString TbDialog::get_save_dir(QStringList tail)//图形化地获取保存的路径
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    dlg.setNameFilters(tail);
    dlg.selectFile(QDate::currentDate().toString("yyyy-MM-dd-useful"+tail.at(0).mid(1)));//加参数保存吗
    if(dlg.exec()!= QDialog::Accepted)
        return nullptr;
    return dlg.selectedFiles()[0];
}

void TbDialog::setBackG()
{
    this->setAutoFillBackground(true);//设置自动填充
    this->update();//更新窗口，调动重写的paintEvent
}
void TbDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap("D:\\ch_9_4\\bkground.jpeg",Q_NULLPTR,Qt::ColorOnly).scaled(this->size()));
}
