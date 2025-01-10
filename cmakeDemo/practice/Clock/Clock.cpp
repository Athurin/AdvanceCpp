#include "Clock.h"

Clock::Clock(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_pTimer = new QTimer(this);
    //(void)connect(m_pTimer, &QTimer::timeout, this, QOverload<>::of(Clock::update));
    (void)connect(m_pTimer, &QTimer::timeout, this, &Clock::update);

    m_pTimer->start(1000);

}

Clock::~Clock()
{}

void Clock::update()
{
    // 更新时钟显示的代码
    qDebug() << "Update called"; // 调试信息
    repaint(); // 重绘窗口
}

void Clock::paintEvent(QPaintEvent* event)
{
    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);// 反锯齿 
    painter.translate(width() / 2, height() / 2); // 将坐标系原点移动到窗口中心
    painter.scale(side / 200.0, side / 200.0);// 并进行缩放


    // 时针的三角形顶点坐标值
    static const QPoint hourHand[3] = {
         QPoint(7, 8),
         QPoint(-7, 8),
         QPoint(0, -40)
    };

    QTime time = QTime::currentTime(); // 获取当前时间
    QColor hourColor(127, 0, 127); //创建颜色集
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);

    painter.save();//保存
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0))); //旋转
    painter.drawConvexPolygon(hourHand, 3); // 绘制多变形(三角形)
    painter.restore();//恢复

    painter.setPen(hourColor);  //绘制表盘边缘时钟刻度
    for (int i = 0; i < 12; ++i)
    {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    ////在Qt中，角度是以16点制顺时针方向计量的，即0度位于3点钟方向，90度位于12点钟方向
    //painter.restore();//恢复
    //painter.rotate(300); //将画笔调整为最上方
    //for (int i = 0; i <= 11; i++)
    //{
    //    QString number = QString::number(i + 1);
    //    qreal angle = i * 30.0; // 每个数字之间的角度差
    //    qreal x = 88; // 数字的x坐标
    //    qreal y = 0; // 数字的y坐标

    //    QRectF boundingRect = painter.boundingRect(0, 0, 10, 10, Qt::AlignCenter, number);
    //    painter.drawText(QPointF(x - boundingRect.width() / 2, y - boundingRect.height() / 2), number);
    //    painter.rotate(30.0);
    //}

    int radius = 100;
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    int pointSize = font.pointSize();

     // 绘制小时文本
    int nHour = 0;
    for (int i = 0; i < 12; ++i) 
    {
        nHour = i + 3;
        if (nHour > 12)
            nHour -= 12;
        painter.drawText(textRectF(radius * 0.8, pointSize, i * 30), Qt::AlignCenter, QString::number(nHour));
    }

    // 分钟
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor minuteColor(0, 127, 127);
    painter.setPen(minuteColor);
    painter.setBrush(minuteColor);

    //绘制分针
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));//旋转的角度为 6°
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    for (int j = 0; j < 60; ++j)  //绘制分针刻度
    {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);  //6°
    }


    //绘制秒针
    static const QPoint secondHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor secondcolor(127, 127, 0);
    painter.setPen(secondcolor);
    painter.setBrush(secondcolor);

    //绘制秒针
    painter.save();
    painter.rotate(6.0 * time.second());//旋转的角度为 6°
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    //for (int j = 0; j < 60; ++j)  //绘制秒针刻度
    //{
    //    if ((j % 5) != 0)
    //        painter.drawLine(92, 0, 96, 0);
    //    painter.rotate(6.0);  //6°
    //}


}

QRectF Clock::textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    rectF.setX(radius * qCos(angle * M_PI / 180.0) - pointSize * 2);
    rectF.setY(radius * qSin(angle * M_PI / 180.0) - pointSize / 2.0);
    rectF.setWidth(pointSize * 4);
    rectF.setHeight(pointSize);
    return rectF;
}
