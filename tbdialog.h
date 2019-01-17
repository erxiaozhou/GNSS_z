#ifndef TBDIALOG_H
#define TBDIALOG_H

#include <save_xls.h>
#include <QDialog>
#include <QImageReader>
#include <QPainter>
#include<QMessageBox>
#include<QFileDialog>
#include<QPixmap>
#include<QStandardPaths>
#include<QDate>
#include<QScreen>


namespace Ui {
class TbDialog;
}

class TbDialog : public QDialog,public SaveExcel
{
    Q_OBJECT

public:
    explicit TbDialog(QWidget *parent = nullptr);
    ~TbDialog();

    //初始化全部表格
    void AllTable_ini();
    void TableDataIni();
private slots:
    void on_btn_close_clicked();

    void on_btn_updata_clicked();



    void on_btn_save_clicked();

private:
    Ui::TbDialog *ui;
    int WT=1750;
    int HT=875;
    void setPartPos(QRect &rect,qreal x,qreal y,qreal wt,qreal ht);
    void setBackG();
    void paintEvent(QPaintEvent *event);
private:
    double tabledata[9][5];
    int tabledata_hei=9;
    int tabledata_wei=5;

    QString get_save_dir(QStringList tail);
    void MessageOfSave(bool saveOK);
//=========================================
    //单元格type
    enum CellType{cta=1000,ctb,ctc,ctd,cte,ctf};
    //列
    enum FieldColNum{cola=0,colb,colc,cold,cole,colf};
    //void createItemARow();
};

#endif // TBDIALOG_H
