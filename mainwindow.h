#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <save_xls.h>
#include <QDebug>
#include <QImageReader>
#include <QPainter>
#include "ui_mainwindow.h"
#include "qwchartview.h"
#include "BOC_Fac.h"
#include"BPSK_Fac.h"
#include "Kls.h"
#include "Gboc.h"
#include "tbdialog.h"
QT_CHARTS_USE_NAMESPACE
//定义UI命名空间
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow ,public SaveExcel
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static bool TWO_SERIES;
private:
//**************************整体窗口与布局部分*******************
    //屏幕参数  能不能自动获取windows的参数
    int WT=1750;
    int HT=875;
    //主窗口指针
    Ui::MainWindow *ui;
    TbDialog *dialog;

     //设置背景
    void setBackG();
    //重写绘画事件，调用update时实现背景重绘
    void paintEvent(QPaintEvent *event);
    //设置各个组件位置
    void setScreenPos();
    //设置单个组件位置
    void setPartPos(QRect &rect,qreal x,qreal y,qreal wt,qreal ht);
    //初始化连接
    void Iniconnect();
//============================================================

//**************************计算与参数输入部分*******************
    //调制相关参数
    static int alf,bit,m;//
    static qreal gs_m;
    static qreal br,bt;
    static qreal bl,c_n;
    static qreal alpha,DELTA_TC;
;
    //QMap,存函数地址
    QMap<QString,QLineSeries *(*)(QLineSeries **)> map;  //功能字符串与对应绘图序列之间映射
    QMap<QString,int> combomap;            //功能字符串与序号之间映射
    //初始化函数映射表map,和combomap
    void IniMap();
    //为各个要绘图的函数提供接口
    static QLineSeries *ASampleFunc(QLineSeries **);//十二个函数为提供 绘图函数 到 绘制图片所需的序列 的接口
    static QLineSeries *ASampleFunc2(QLineSeries **);
    static QLineSeries *ASampleFunc3(QLineSeries **);
    static QLineSeries *ASampleFunc4(QLineSeries **);
    static QLineSeries *ASampleFunc5(QLineSeries **);
    static QLineSeries *ASampleFunc6(QLineSeries **);
    static QLineSeries *ASampleFunc7(QLineSeries **);
    static QLineSeries *ASampleFunc8(QLineSeries **);
    static QLineSeries *ASampleFunc9(QLineSeries **);
    static QLineSeries *ASampleFunc10(QLineSeries **);
    static QLineSeries *ASampleFunc11(QLineSeries **);
    static QLineSeries *ASampleFunc12(QLineSeries **);
    static void keepHe(qreal *y,int num_y,qreal *x,int num_x);  //可能不用重载
    //绘图辅助函数，保证矩形波矩形绘图效果
    static void keepHe(int *y,int num_y,qreal *x,int num_x);

//============================================================
//******************************chart绘制部分******************
     //画图所需的点序列

     QLineSeries *series0;
     QLineSeries *series1;
     //状态栏显示用的文字标签
     QLabel *labXYValue;
     QChart *chart;
     static QValueAxis *axisX;
     static QValueAxis *axisY;
     //准备绘图的数据
     void prepareData();  //通过映射表与函数指针获取要绘制的点序列
//============================================================

//******************************table绘制部分******************
     //表格部分需要的枚举
     //创建item时单元格type的枚举
     enum CellType{ctData=1000,ctNew};   //枚举个数还是需要调整，虽然是越多越好
     //字段于表格中的列号
     enum FieldColNum{colData=0,col2,col3,col4,col5,col6,col7,col8};
     void getTableData(qreal* para);  //准备表格数据
     void initable();//表格初始化
     QStringList getTableName(); //为获取表头文字
     QStringList tablerow;  //表列名称保存
//============================================================

//******************************功能设置输入********************
     //存储所选择的功能
     QString statement_Fea;  //控制目前功能
     QString statement_way;  //控制调制方式
     QString statement_func; //控制显示波形

     void iniFea();//给工作模式，参数对应窗口组件状态进行初始化
     void set_FuncLineEdit_Text(); //在lineedit显示当前的模式
//********************************保存相关**********************
     QString get_save_dir(QStringList);//弹窗获取保存路径
     void MessageOfSave(bool);//弹窗显示保存状况
//=============================================================

//==============================================================================================
//#######################################################################槽函数部分###############
public slots:
    //与chart相关的各个函数
    void on_LegendMarkerClicked();//图例单击槽函数，自定义槽函数， 显示或隐藏对应序列
    void on_mouseMovePoint(QPoint point);//鼠标移动事件，用于实时显示坐标
    void on_actZoomReset_triggered(); //工具栏按钮
    void on_actZoomIn_triggered(); //工具栏按钮
    void on_actZoomOut_triggered(); //工具栏按钮
    //更新表格和图片按键
    void on_btnIniTableData_clicked();  //更新表格按键
    void on_btnIniChartData_clicked();  //更新图片按键
    //保存表格，图片
    void on_btnsavechart_clicked(); //保存chart按键
    void on_btnsavetable_clicked(); //保存table按键BOC / BPSK

//******************************获取用户要的功能与参数************************
    //改变功能字符串
    //方法1：固定输入
    void on_radiobtn_chicked(); //选择按钮  选择BOC/BPSK
    void on_Combo_func_CurrentChanged(const QString &);//选择所需展示的波形
    //方法2：用户自行输入
    void on_lineFun_changed(); //直接在lineedit改变功能字符串
    //改变参数 用于获取lineedit中的数据，由对应lineedit改变时触发
    void on_alf_set(const QString &);
    void on_bit_set(const QString &);
    void on_m_set(const QString &);
    void on_br_set(const QString &);
    void on_bt_set(const QString &);
    void on_gs_m_set(const QString &);
//=========================================================================
    void on_show_all_table();
private :
    QPoint beginPoint;  //矩形框的起点终点
    QPoint endPoint;
protected:
//    void keyPressEvent(QKeyEvent *event);      //按键事件
    void mousePressEvent(QMouseEvent *event);  //鼠标左键按下
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放左键
signals:
    void mouseMovePoint(QPoint point);  //鼠标移动信号
private slots:
    void on_lineEdit_textChanged(const QString &arg1);//四个更新参数的函数
    void on_lineEdit_bl_textChanged(const QString &arg1);
    void on_lineEdit_dtc_textChanged(const QString &arg1);
    void on_lineEdit_cn0_textChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
