#include"mainwindow.h"
void MainWindow::on_radiobtn_chicked()
{
    if(ui->BOC->isChecked())
        statement_way="BOC";
    else if(ui->BPSK->isChecked())
        statement_way="BPSK";
    statement_Fea=statement_way+"_"+statement_func;
    set_FuncLineEdit_Text();
}
void MainWindow::on_Combo_func_CurrentChanged(const QString & a)
{
    statement_func=a;
    statement_Fea=statement_way+"_"+statement_func;
    set_FuncLineEdit_Text();
}
void MainWindow::on_lineFun_changed()
{

    QString a=ui->lineEdit_InputFunc->text();
    statement_Fea=ui->lineEdit_InputFunc->text();

    ui->combo_box_func->setCurrentIndex(combomap[a]%6);
    if(combomap[a]>5)
    {
        ui->BPSK->setChecked(true);
        ui->BOC->setChecked(false);
    }
    else
    {
        ui->BOC->setChecked(true);
        ui->BPSK->setChecked(false);
    }
    set_FuncLineEdit_Text();

}

void MainWindow::on_alf_set(const QString &a)
{
    alf=a.toInt();
}
void MainWindow::on_bit_set(const QString &a)
{
    bit=a.toInt();
}
void MainWindow::on_m_set(const QString &a)
{
    m=a.toInt();
}
void MainWindow::on_br_set(const QString &a)
{
    br=a.toInt();
}
void MainWindow::on_bt_set(const QString &a)
{
    bt=a.toInt();
}
void MainWindow::on_gs_m_set(const QString &a)
{
    gs_m=a.toInt();
}
//设定成按键触发，初始化数据,包括表头
void MainWindow::on_btnIniTableData_clicked()
{
    qreal parameter[9];   //各个参数的计算

    getTableData(parameter);
    QTableWidgetItem *item;
    QString str;
    QString str2;

    QStringList headerText;
    headerText<<statement_way;
   ui->tableWidget->setHorizontalHeaderLabels(headerText);
    ui->tableWidget->clearContents(); //只清除工作区，不清除表头
    for(int i=0;i<9;i++)
    {
        str.setNum(parameter[i]);
        item=new QTableWidgetItem(str,MainWindow::ctData);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i,MainWindow::colData,item);

    }

}
void MainWindow::on_btnIniChartData_clicked()
{
    prepareData();
}
// 下面一系列函数是chartview的重写
void MainWindow::on_mouseMovePoint(QPoint point)//鼠标移动事件，用于实时显示光标
{//响应鼠标移动
    QPointF pt=ui->chartView->chart()->mapToValue(point);//转换为图表数值
    labXYValue->setText(QString::asprintf("X=%.2f,Y=%.2f",pt.x(),pt.y()));      //给标签赋显示
}
void MainWindow::on_actZoomReset_triggered() //工具栏按钮
{
    ui->chartView->chart()->zoomReset();
}
void MainWindow::on_actZoomIn_triggered() //工具栏按钮
{
ui->chartView->chart()->zoom(1.2);
}
void MainWindow::on_actZoomOut_triggered() //工具栏按钮
{
ui->chartView->chart()->zoom(0.8);
}
void MainWindow::on_LegendMarkerClicked()//图例单击槽函数，自定义槽函数， 显示或隐藏相关序列（？？）
{//单击图例maker的响应
    QLegendMarker *marker=qobject_cast<QLegendMarker *>(sender());
    switch (marker->type()) {
        case QLegendMarker::LegendMarkerTypeXY: //QLineSeries序列 ？？？
        {
        marker->series()->setVisible(!marker->series()->isVisible());//序列本身的setVisible属性
        marker->setVisible(true);
        }
    }
}
void MainWindow::on_btnsavechart_clicked()
{

        QScreen * screen = QGuiApplication::primaryScreen();
        QPixmap p = screen->grabWindow(ui->chartView->winId());//ui->chartView->winId()
        QImage image = p.toImage();

        QStringList tail;
        tail<<"*.png"<<"*.jpg"<<"*.bmp";

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
        else
            MessageOfSave(false);//后缀不合规范则保存失败
}
void MainWindow::on_btnsavetable_clicked()   //保存表格的函数里，目前表格名称是钉死的，这个要改   下面注释掉的代码是弹窗保存
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
    qDebug()<<"in on_btnsavetable_clicked() 11111111111";
    //由后缀定保存方式
    if (user_tail==".png")
        MessageOfSave(image.save(filePath,"PNG"));
    else if (user_tail==".jpg")
        MessageOfSave(image.save(filePath,"JPG"));
    else if (user_tail==".bmp")
        MessageOfSave(image.save(filePath,"BMP"));
    else if (user_tail==".xls")
        MessageOfSave(saveFromTable(filePath,ui->tableWidget,""));
    else
        MessageOfSave(false);//后缀不合规范则保存失败                   //这里也可以细划分
}
void MainWindow::MessageOfSave(bool saveOK)
{
    if(saveOK)
    {
        QMessageBox::information(this,tr("提示"),tr("保存成功"));
    }
    else
        QMessageBox::information(this,tr("提示"),tr("保存失败"));
}
QString MainWindow::get_save_dir(QStringList tail)//图形化地获取保存的路径
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    dlg.setNameFilters(tail);
    dlg.selectFile(QDate::currentDate().toString("yyyy-MM-dd"+statement_way+tail.at(0).mid(1)));//加参数保存吗
    if(dlg.exec()!= QDialog::Accepted)
        return nullptr;
    return dlg.selectedFiles()[0];
}
void MainWindow::on_show_all_table()
{
    dialog=new TbDialog;
    dialog->show();
}
