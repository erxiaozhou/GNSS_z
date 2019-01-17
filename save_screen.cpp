//关于截屏保存的函数
#include <mainwindow.h>
void MainWindow::mousePressEvent(QMouseEvent *event)  //鼠标左键按下
{//鼠标左键按下，记录beginPoint
    beginPoint=event->pos();
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)//鼠标释放左键
{
    qDebug()<<"in MainWindow::mouseReleaseEvent";
    //鼠标左键释放事件
    if(ui->save_screen_checkbox->isChecked())
    {//鼠标左键释放，获取endpoint
        endPoint=event->pos();
        QRectF rectF;
        rectF.setTopLeft(this->beginPoint);
        rectF.setBottomRight(this->endPoint);

        QScreen * screen = QGuiApplication::primaryScreen();
        QPixmap p = screen->grabWindow(this->winId(),this->beginPoint.rx(),this->beginPoint.ry(),this->endPoint.rx(),this->endPoint.ry());//ui->chartView->winId()
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
    else
        ;
    QMainWindow::mouseReleaseEvent(event);

}
