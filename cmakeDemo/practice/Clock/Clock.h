#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Clock.h"
#include <QPainter>
#include <QEvent>
#include <QTimer>
#include <QTime>
#include <QObject>
#include <QDebug>
#include <QtMath>


class Clock : public QMainWindow
{
    Q_OBJECT

public:
    Clock(QWidget *parent = nullptr);
    ~Clock();

    void paintEvent(QPaintEvent* event);
    QRectF textRectF(double radius, int pointSize, double angle);

public slots:
    void update();

private:

    QTimer* m_pTimer = nullptr;



    Ui::ClockClass ui;
};
