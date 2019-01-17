#include "mainwindow.h"
#include"NELP.h"
#include"Gboc.h"
#include"Multipath.h"

#define BUFF_SIZE_SHOW 1000
QLineSeries * MainWindow::ASampleFunc(QLineSeries ** y111=nullptr)
{
    MainWindow::TWO_SERIES=true;
    QLineSeries * y11=new QLineSeries;
    *y111=y11;
    qDebug()<<"fffffffffff"<<y11<<"      "<<*y111;

    int value[200];
    double time[200];
    static QLineSeries y1;
    int Ut[200];
    y1.clear();
    Create_boc(alf, bit, value, time, 200,10, Ut);
    int i;
    keepHe(value,200,time,200);
    keepHe(Ut,200,time,200);
    for(i=0;i<200;i++)
    {
//       if(Ut[i]>2) Ut[i]=2;
//       else if(Ut[i]<-2) Ut[i]=-1;
//       else ;
//       qDebug()<<Ut[i];
       qDebug()<<value[i]<<"    "<<Ut[i];
       y1.append(time[i],value[i]);
       y11->append(time[i],Ut[i]);

    }
    axisX->setRange(0,time[199]);               //设置坐标轴范围
    axisX->setTitleText("time/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-3,3);
    axisY->setTitleText("value");

    return &y1;
}
QLineSeries * MainWindow::ASampleFunc2(QLineSeries ** y11=nullptr)
{
    MainWindow::TWO_SERIES=false;
    qreal value[1000];
    qreal time[1000];
    Sel_boc_func(alf,bit,value,1000, time);


    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<1000;i++)
    {
        y1.append(time[i],value[i]);
    }
    axisX->setRange(-time[999],time[999]);               //设置坐标轴范围
    axisX->setTitleText("τ/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-1,1);
    axisY->setTitleText("value");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc3(QLineSeries ** y11=nullptr)
{
    MainWindow::TWO_SERIES=false;
    qreal value[1000];
    qreal f[1000];
    Create_boc_spectrum(alf, bit, value,f,1000);


    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<1000;i++)
    {
        y1.append(f[i],value[i]);
    }
    axisX->setRange(-f[999],f[999]);               //设置坐标轴范围
    axisX->setTitleText("f/Hz");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-150,-50);
    axisY->setTitleText("value");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc4(QLineSeries ** y111=nullptr)
{
    MainWindow::TWO_SERIES=true;
    qreal value[500];
    qreal value11[500];
    qreal time[500];
    QLineSeries * y11=new QLineSeries;
    *y111=y11;
//(int A, int B, double * BUFF, double BR, int SIZE, double * TM, double BL, double C_N, double T)
    Sigma_NELP_boc(alf, bit,value,br,500,time,bl,c_n,0.02);//Sigma_NELP_boc(int A, int B, double * BUFF, double BR, int SIZE, double * TM);
    Sigma_NELP_boc(alf, bit,value11,br,500,time,bl,c_n,0.005);

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<400;i++)
    {
        y1.append(time[i],value[i]);
        y11->append(time[i],value11[i]);
    }
    axisX->setRange(0,time[400]);               //设置坐标轴范围
    axisX->setTitleText("△/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(0,1);
    axisY->setTitleText("trancing error/m");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc5(QLineSeries ** y111=nullptr)
{
    MainWindow::TWO_SERIES=true;
    qreal value[500];
    qreal value11[500];
    qreal time[500];
    //int type=0;
    QLineSeries * y11=new QLineSeries;
    *y111=y11;
    //(int a, int b, double * BUFF, double Br, int size, double * TM, double ALPHA, double DELTA_TC)
    Multipath_boc(alf, bit,value,br,500,time,alpha,DELTA_TC);//Multipath_bpsk(int m, double * BUFF, double Br, int size, int TYPE, double *TM);
    Multipath_boc(alf, bit,value11,br,500,time,(-1)*alpha,DELTA_TC);

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<500;i++)
    {
        y1.append(time[i],value[i]);
        y11->append(time[i],value11[i]);
    }
    axisX->setRange(0,time[499]);               //设置坐标轴范围
    axisX->setTitleText("△/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-5,5);
    axisY->setTitleText("trancing error/m");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc6(QLineSeries ** y11=nullptr)
{//RST(double Beta, double fs, double fc, double * Rst,double * Rst_index,double down,double up);
    MainWindow::TWO_SERIES=false;
    qreal value[1000];
    qreal time[1000];
    qreal Beta=12000000;

    RST(Beta, alf*1023000, bit*1023000,value,time,-40,40);//1000个点 //Beta是单边的限

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<1000;i++)
    {
        y1.append(time[i],value[i]);
        qDebug()<<y1.at(i);
    }

    axisX->setRange(-32,32);               //设置坐标轴范围
    axisX->setTitleText("τ/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(0,1);
    axisY->setTitleText("value");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc7(QLineSeries ** y111=nullptr)
{
    MainWindow::TWO_SERIES=true;
    qreal value[3000];
    qreal time[3000];
    int Ut[3000];//Ut是原始序列
    QLineSeries * y11=new QLineSeries;
    *y111=y11;
    Gene_bpsk(m, value, 10,3000,time,Ut);

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<3000;i++)
    {
        y1.append(time[i],value[i]);
        y11->append(time[i],Ut[i]);
    }
    axisX->setRange(0,time[999]);               //设置坐标轴范围
    axisX->setTitleText("time/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-3,3);
    axisY->setTitleText("value");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc8(QLineSeries ** y11=nullptr)
{
    MainWindow::TWO_SERIES=false;
    qreal value[1000];
    qreal time[1000];
    Self_bpsk_func(m,value,1000, time);


    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<1000;i++)
    {
        y1.append(time[i],value[i]);
    }
    axisX->setRange(-time[999],time[999]);               //设置坐标轴范围
    axisX->setTitleText("τ/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-1,1);
    axisY->setTitleText("value");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc9(QLineSeries ** y11=nullptr)
{//void P_bpsk(int m, double * P_BPSK, int size, double * Fx);
    MainWindow::TWO_SERIES=false;
    qreal value[1000];
    qreal f[1000];
    P_bpsk(m, value,1000,f);


    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<1000;i++)
    {
        y1.append(f[i],value[i]);
    }
    axisX->setRange(-f[999],f[999]);               //设置坐标轴范围
    axisX->setTitleText("f/Hz");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-150,-50);
    axisY->setTitleText("value");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc10(QLineSeries ** y111=nullptr)
{
    MainWindow::TWO_SERIES=true;
    qreal value[500];
    qreal time[500];
    qreal value11[500];
//    qreal br=24000000;
    QLineSeries * y11=new QLineSeries;
    *y111=y11;
    Sigma_NELP_bpsk(m,value,br,500,time,bl,c_n,0.02);//Sigma_NELP_boc(int A, int B, double * BUFF, double BR, int SIZE, double * TM);
    Sigma_NELP_bpsk(m,value11,br,500,time,bl,c_n,0.005);

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<500;i++)
    {
        y1.append(time[i],value[i]);
        y11->append(time[i],value11[i]);
    }
    axisX->setRange(0,time[499]);               //设置坐标轴范围
    axisX->setTitleText("△/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(0,1);
    axisY->setTitleText("trancing error/m");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc11(QLineSeries ** y111=nullptr)
{
    MainWindow::TWO_SERIES=true;
    qreal value[250];
    qreal value11[250];
    qreal time[250];
    //int type=0;
    QLineSeries * y11=new QLineSeries;
    *y111=y11;

    Multipath_bpsk(m,value,br,250,time,alpha,DELTA_TC);//Multipath_bpsk(int m, double * BUFF, double Br, int size, int TYPE, double *TM);
    Multipath_bpsk(m,value11,br,250,time,(-1)*alpha,DELTA_TC);

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<250;i++)
    {
        y1.append(time[i],value[i]);
        y11->append(time[i],value11[i]);
    }
    axisX->setRange(0,time[249]);               //设置坐标轴范围
    axisX->setTitleText("△/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-5,5);
    axisY->setTitleText("trancing error/m");
    return &y1;
}
QLineSeries * MainWindow::ASampleFunc12(QLineSeries ** y11=nullptr)
{
    MainWindow::TWO_SERIES=false;
    qreal value[1000];
    qreal time[1000];
    Left_self_bpsk(br,m,value,1000, time);

    int i;
    static QLineSeries y1;
    y1.clear();
    for(i=0;i<1000;i++)
    {
        y1.append(time[i],value[i]);
    }
    axisX->setRange(-time[999],time[999]);               //设置坐标轴范围
    axisX->setTitleText("τ/s");       //轴标题文字，还可以设置字体等等

    axisY->setRange(-2,2);
    axisY->setTitleText("value");
    return &y1;
}
void MainWindow::keepHe(qreal *y,int num_y,qreal *x,int num_x)
{
//y为输入纵坐标数组，x为输入横坐标数组
    int i=0;
    qreal hl=0.99;//突变阈值暂设为0.99
    if (!(num_x==num_y))
    {  qDebug()<<"num_x!=num_y!!!";
       exit(-1);//查一下
    }
    for(i=0;i<num_x-1;i++)
    {
        if(((y[i]-y[i+1])>hl)||((y[i]-y[i+1])<-hl))
            x[i+1]=x[i];
    }
}
void MainWindow::keepHe(int *y,int num_y,qreal *x,int num_x)
{
//y为输入纵坐标数组，x为输入横坐标数组
    int i=0;
    qreal hl=0.99;//突变阈值暂设为0.99
    if (!(num_x==num_y))
    {  qDebug()<<"num_x!=num_y!!!";
       exit(-1);//查一下
    }
    for(i=0;i<num_x-1;i++)
    {
        if(((y[i]-y[i+1])>hl)||((y[i]-y[i+1])<-hl))
            x[i+1]=x[i];
    }
}
