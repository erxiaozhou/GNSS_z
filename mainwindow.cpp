#include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    chart=new QChart();
    series0= new QLineSeries();
    series1= new QLineSeries();
    IniMap();  //初始化函数地址列表
    iniFea();  //功能状态及其选择栏初始化

    labXYValue=new QLabel("当前图片坐标X=, Y=");//状态栏显示点的坐标
    labXYValue->setMinimumWidth(250);
    ui->statusBar->addWidget(labXYValue); //添加label于状态栏,显示坐标

    //series0=(QLineSeries *)ui->chartView->chart()->series().at(0);

    Iniconnect(); //槽函数连接
    prepareData();
    initable();

    //设置窗口位置
    //获取桌面大小
    QRect deskRect = QApplication::desktop()->availableGeometry();
    WT=deskRect.width();
    HT=deskRect.height();//-25
    setScreenPos();
    setBackG();
           //setDragMode(QGraphicsView::RubberBandDrag);
    this->setMouseTracking(true);  //将鼠标跟踪开启
    setWindowTitle(tr("GNSS"));//设置主窗口标题
    TWO_SERIES=false;
    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(false);
    QHeaderView* headerView2=ui->tableWidget->horizontalHeader();
    headerView2->setHidden(false);

}
//初始化众多静态变量
int MainWindow::alf=10;
int MainWindow::bit=5;
int MainWindow::m=10;
qreal MainWindow::br=24000000.0;
qreal MainWindow::gs_m=1;
qreal MainWindow::bt=24000000.0;
qreal MainWindow::bl=1;
qreal MainWindow::c_n=1000;
qreal MainWindow::alpha=0.5;
qreal MainWindow::DELTA_TC=3e-8;
QValueAxis* MainWindow::axisX=new QValueAxis;
QValueAxis* MainWindow::axisY=new QValueAxis;
bool MainWindow::TWO_SERIES=false;
//析构函数
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Iniconnect()
{
    //连接槽函数
    QObject::connect(ui->chartView,SIGNAL(mouseMovePoint(QPoint)),this,SLOT(on_mouseMovePoint(QPoint)));//鼠标移动事件
    QObject::connect(ui->btnIniTableData,SIGNAL(clicked()),this,SLOT(on_btnIniTableData_clicked()));//开始出表格
    QObject::connect(ui->btnIniChartData,SIGNAL(clicked()),this,SLOT(on_btnIniChartData_clicked()));//开始表格,波形
    QObject::connect(ui->save_chart,SIGNAL(clicked()),this,SLOT(on_btnsavechart_clicked()));//保存波形
    QObject::connect(ui->save_table,SIGNAL(clicked()),this,SLOT(on_btnsavetable_clicked()));//保存表格
    //连接BOC BPSK btn
    QObject::connect(ui->BOC,SIGNAL(clicked()),this,SLOT(on_radiobtn_chicked()));
    QObject::connect(ui->BPSK,SIGNAL(clicked()),this,SLOT(on_radiobtn_chicked()));
    //连接combo box
    QObject::connect(ui->combo_box_func,SIGNAL(currentIndexChanged(const QString &)),this,
                     SLOT(on_Combo_func_CurrentChanged(const QString &)));
    //连接显示全部表格的按键
    QObject::connect(ui->btn_AllTable,SIGNAL(clicked()),this,SLOT(on_show_all_table()));
    //输入参数的连接
    QObject::connect(ui->lineEdit_alf,SIGNAL(textChanged(const QString &)),this,SLOT(on_alf_set(const QString &)));
    QObject::connect(ui->lineEdit_bit,SIGNAL(textChanged(const QString &)),this,SLOT(on_bit_set(const QString &)));
    QObject::connect(ui->lineEdit_m,SIGNAL(textChanged(const QString &)),this,SLOT(on_m_set(const QString &)));
    QObject::connect(ui->lineEdit_br,SIGNAL(textChanged(const QString &)),this,SLOT(on_br_set(const QString &)));
    QObject::connect(ui->lineEdit_bt,SIGNAL(textChanged(const QString &)),this,SLOT(on_bt_set(const QString &)));
    QObject::connect(ui->lineEdit_gsm,SIGNAL(textChanged(const QString &)),this,SLOT(on_gs_m_set(const QString &)));
    QObject::connect(ui->pbtn_getedit,SIGNAL(clicked()),this,SLOT(on_lineFun_changed()));
}
void MainWindow::setBackG()
{
    this->setAutoFillBackground(true);//设置自动填充
    this->update();//更新窗口，调动重写的paintEvent
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap("D:\\ch_9_4\\bkground.jpeg",Q_NULLPTR,Qt::ColorOnly).scaled(this->size()));
}

void MainWindow::prepareData()     //只负责把生成的点队列赋给series，生成队列由里面的函数去做
{
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    series0->setName(statement_Fea);
    series1->setName(statement_Fea+"2");

    QLineSeries * (*pf)(QLineSeries **);
    pf=map[statement_Fea];
    qDebug()<<"in preparadata"<<statement_Fea;

    //清空队列
    series0->clear();
    series1->clear();

    QLineSeries* y1=nullptr;
    QLineSeries* y=pf(&y1);
    qDebug()<<"y1"<<y1;
    for(int i=0;i<y->count();i++)
    {
        series0->append(y->at(i));
    }
    if(TWO_SERIES==false)
        ;
    else
    {
    for(int j=0;j<y1->count();j++)
    {
        if(y1==nullptr) break;
        series1->append(y1->at(j));
    }
    }
    static int itimes=0;
    chart->setTitle(statement_func);
    series0->setName(statement_way+" ");//statement_func
    series1->setName(statement_way+"2");
    qDebug()<<statement_Fea<<"    "<<TWO_SERIES;
    if (itimes==0) chart->addSeries(series0);
    if (itimes==0 && TWO_SERIES) chart->addSeries(series1);
    if (itimes>0)
    {
        chart->removeSeries(series0);
        chart->addSeries(series0);
        if (TWO_SERIES)
        {
            chart->removeSeries(series1);
            chart->addSeries(series1);
        }
    }

    foreach(QLegendMarker* marker,chart->legend()->markers())   //！！！  //markers获取列表
        connect(marker,SIGNAL(clicked()),this,SLOT(on_LegendMarkerClicked()));


    chart->setAxisX(axisX,series0);
    chart->setAxisY(axisY,series0);
    if (TWO_SERIES)
    {
        chart->setAxisX(axisX,series1);
        chart->setAxisY(axisY,series1);
    }
    itimes++;

    ui->chartView->setChart(chart);
}

//table初始化
void MainWindow::initable()
{
    QTableWidgetItem *headerItem;

    tablerow<<"频谱主搬距频带中心的频偏（MHz）"<<"最大功率谱密度(dBW/Hz)"<<"90%功率的带宽（MHz）"<<"RMS带宽（MHz）"
            <<"等效矩形带宽（MHz）"<<"与自身的频谱隔离系数(dB/Hz)"<<"与1.023MHzBPSK的频谱隔离系数(dB/Hz)"
            <<"与BOC（10,5）的频谱隔离系数(dB/Hz)"<<"带外损失(dB)";
    ui->tableWidget->setHorizontalHeaderLabels(getTableName());//因为会变，所以水平标题要变，竖直先不变 目前的返回的列表只有1个值
    ui->tableWidget->setColumnCount(getTableName().count());

    //设置字体****************************************************************************************************
    headerItem=new QTableWidgetItem(getTableName().at(0));


    QFont font=headerItem->font();
    font.setBold(true); //设置粗体
    font.setPointSize(12);//设置字体大小
    headerItem->setTextColor(Qt::black);//字体颜色
    headerItem->setFont(font); //设置字体
    ui->tableWidget->setHorizontalHeaderItem(0,headerItem);
}

void MainWindow::getTableData(qreal * para)
{

    int i;
    for(i=0;i<9;i++)
    {
        para[i]=i*2.0;
    }
    qDebug()<<"in getTableData";
    Kls K;
    double DBGbocf_[Ni] = {0};
    if (combomap[statement_Fea]<6) //确定BOC还是BPSK    //BOC
    {
        qDebug()<<"正在计算主瓣频偏......";
        para[0]=Fre_Offset(alf*1023000,bit*1023000,DBGbocf_);

        qDebug()<<"正在计算主瓣最大功率谱密度......";
        para[1]=Maxgf(br/2, alf*1023000, bit*1023000, DBGbocf_);/*Band_width_0_9_boc(alf, bit, br)/1000000;*/

        qDebug()<<"正在计算90%功率带宽......";
        para[2]=Band_width_0_9_boc(alf, bit, br)/1000000;/*Brms(br/2, alf*1023000, bit*1023000)/1000000;*/

        qDebug()<<"正在计算RMS带宽......";
        para[3]=Brms(br/2, alf*1023000, bit*1023000)/1000000;/*Brect(br/2, alf*1023000, bit*1023000, DBGbocf_)/1000000;*/

        qDebug()<<"正在计算等效矩形带宽......";
        para[4]=Brect(br/2, alf*1023000, bit*1023000, DBGbocf_)/1000000;

        qDebug()<<"正在计算与自身的频谱隔离系数......";
        K.Kls_TYPE = Kls_BOC_AS_GS_BOC_AS_GL;
        K.Gs_BOC_a = alf;
        K.Gs_BOC_b = bit;
        K.Gl_BOC_a = alf;
        K.Gl_BOC_b = bit;
        para[5]=-Kls_func(K, bt, br);

        qDebug()<<"正在计算与与1.023MBPSK的隔离系数......";
        K.Kls_TYPE = Kls_BOC_AS_GS_PSK_AS_GL;
        K.Gs_BOC_a = alf;
        K.Gs_BOC_b = bit;
        K.Gl_PSK_R_m = 1;
        para[6]=-Kls_func(K, bt, br);

        qDebug()<<"正在计算与BOC（10,5）的频谱隔离系数......";
        K.Kls_TYPE = Kls_BOC_AS_GS_BOC_AS_GL;
        K.Gs_BOC_a = alf;
        K.Gs_BOC_b = bit;
        K.Gl_BOC_a = 10;
        K.Gl_BOC_b = 5;
        para[7]=-Kls_func(K, bt, br);

        qDebug()<<"正在计算带外损失......";
        para[8]=Wideoutloss(br/2, alf*1023000, bit*1023000);//带外损失

    }
    else                     //BPSK
    {
        qDebug()<<"正在计算主瓣频偏......";
        para[0]=0;//缺一个主瓣最大功率谱密度

        qDebug()<<"正在计算90%带宽......";
        para[2]=Band_width_0_9_bpsk(m,br)/1000000;

        qDebug()<<"正在计算RMS带宽......";
        para[3]=RMS_bpsk(m,br)/1000000;//RMS带宽

        qDebug()<<"正在计算等效矩形带宽......";
        para[4]=RECT_bpsk(m,br)/1000000;//等效矩形带宽

        qDebug()<<"正在计算与自身的频谱隔离系数......";
        K.Kls_TYPE= Kls_PSK_AS_GS_PSK_AS_GL;
        K.GS_PSK_R_m = m;//接收BPSK的基带M
        K.Gl_PSK_R_m = m;//干扰BPSK的基带M
        para[5] = -Kls_func(K,bt,br);

        qDebug()<<"正在计算与与1.023MBPSK的隔离系数......";
        K.Kls_TYPE= Kls_PSK_AS_GS_PSK_AS_GL;
        K.GS_PSK_R_m = m;
        K.Gl_PSK_R_m = 1;
        para[6] = -Kls_func(K,bt,br);

        qDebug()<<"正在计算与BOC（10,5）的频谱隔离系数......";
        K.Kls_TYPE = Kls_PSK_AS_GS_BOC_AS_GL;
        K.GS_PSK_R_m = m;
        K.Gl_BOC_a = 10;
        K.Gl_BOC_b = 5;
        para[7] = -Kls_func(K,bt,br);

        qDebug()<<"正在计算主瓣最大功率谱密度......";
        para[1]=Max_power_bpsk(m,br);//主瓣最大功率谱密度

        qDebug()<<"正在计算带外损失......";
        para[8]=LOSS_power_bpsk(m,br);//带外损失
    }
}
QStringList MainWindow::getTableName()  //获得表头字符串
{
    QStringList headerText;
    headerText<<statement_way;
    return headerText;
}
//设置组件位置
void MainWindow::setScreenPos()
{
    QRect wsize=this->geometry();
    setPartPos(wsize,0,0,11.5,8.5);//wsize.setRect(0,30,WT,HT);
    this->setGeometry(wsize);
    //设置组件位置与大小**************************
    //表格

    QRect wsize_chart=ui->chartView->geometry();
    setPartPos(wsize_chart,5,0,6,6.5);//4.5
    //setPartPos(wsize_chart,3,0,4.2,3.5);
    ui->chartView->setGeometry(wsize_chart);
}
//设置组件位置的辅助函数
void MainWindow::setPartPos(QRect &rect,qreal x,qreal y,qreal wt,qreal ht)
{
    rect.setRect(0+qRound(WT*x/11.5),30+qRound(HT*y/8.5),qRound(WT*wt/11.5),qRound(HT*ht/8.5));
}
//计算数据部分******************************************************************************
void MainWindow::IniMap()
{
    combomap["BOC_时域波形"]=0;
    combomap["BOC_自相关函数"]=1;
    combomap["BOC_功率谱密度"]=2;
    combomap["BOC_码跟踪误差"]=3;
    combomap["BOC_多径误差"]=4;
    combomap["BOC_带限自相关函数"]=5;
    combomap["BPSK_时域波形"]=6;
    combomap["BPSK_自相关函数"]=7;
    combomap["BPSK_功率谱密度"]=8;
    combomap["BPSK_码跟踪误差"]=9;
    combomap["BPSK_多径误差"]=10;
    combomap["BPSK_带限自相关函数"]=11;
    map["BOC_时域波形"]=ASampleFunc;
    map["BOC_自相关函数"]=ASampleFunc2;
    map["BOC_功率谱密度"]=ASampleFunc3;
    map["BOC_码跟踪误差"]=ASampleFunc4;
    map["BOC_多径误差"]=ASampleFunc5;
    map["BOC_带限自相关函数"]=ASampleFunc6;
    map["BPSK_时域波形"]=ASampleFunc7;
    map["BPSK_自相关函数"]=ASampleFunc8;
    map["BPSK_功率谱密度"]=ASampleFunc9;
    map["BPSK_码跟踪误差"]=ASampleFunc10;
    map["BPSK_多径误差"]=ASampleFunc11;
    map["BPSK_带限自相关函数"]=ASampleFunc12;
}


//****************************************************************************************

void MainWindow::set_FuncLineEdit_Text()
{
    ui->lineEdit_InputFunc->setText(statement_Fea);
}
void MainWindow::iniFea()
{
    //参数显示初始化
    ui->lineEdit_alf->setText(QString::number(alf));
    ui->lineEdit_bit->setText(QString::number(bit));
    ui->lineEdit_m->setText(QString::number(m));
    ui->lineEdit_br->setText(QString::number(br));
    ui->lineEdit_bt->setText(QString::number(bt));
    ui->lineEdit_gsm->setText(QString::number(gs_m));
    ui->lineEdit_bl->setText(QString::number(bl));
    ui->lineEdit->setText(QString::number(alpha));
    ui->lineEdit_cn0->setText(QString::number(c_n));
    ui->lineEdit_dtc->setText(QString::number(DELTA_TC));

    //函数功能选择初始化
    statement_way="BOC";
    ui->BOC->setChecked(true);
    ui->BPSK->setChecked(false);
    ui->combo_box_func->setCurrentIndex(0);//初始化combo

    statement_func="时域波形";
    statement_Fea=statement_way+"_"+statement_func;
    set_FuncLineEdit_Text();

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    alpha=arg1.toDouble();
}

void MainWindow::on_lineEdit_bl_textChanged(const QString &arg1)
{
    bl=arg1.toDouble();
}

void MainWindow::on_lineEdit_dtc_textChanged(const QString &arg1)
{
    DELTA_TC=arg1.toDouble();
}

void MainWindow::on_lineEdit_cn0_textChanged(const QString &arg1)
{
    c_n=arg1.toDouble();
}
