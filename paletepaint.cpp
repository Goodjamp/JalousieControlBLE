#include "paletepaint.h"
#include "QStyle"
#include "QPen"
#include "QPainter"
#include "QColor"
#include "QRgb"
#include "QDebug"

#define MINIMUM_H      0
#define MAXIMUM_H      1920
#define FIXED_H        300
#define FIXED_W        600


PaletePaint::PaletePaint(QWidget *parent) : QWidget(parent)
{
    /*
     *  Set geometry settings
     */
    mousePress = false;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    this->setMinimumSize(FIXED_W, FIXED_H);
    this->setMaximumSize(FIXED_W, FIXED_H);

}

QRgb PaletePaint::getColor(int x, int height, int width)
{
    int r = 0, g = 0, b = 0;

    height = width * 3;

    int x1_r = (2 * width) / 3;
    int x2_r = 0;
    int x3_r = width / 3;

    int x1_g = 0;
    int x2_g = width / 3;
    int x3_g = (2 * width) / 3;

    int x1_b = width / 3;
    int x2_b = (2 * width) / 3;
    int x3_b =  width;

    int k = 9;

    if (x == 51) {
        x = 51;
    }

    // Calc red color
    if ((x > x1_r) && (x <= width)) {
         r = k * x - 2 * height;
    } else if  ((x >= 0) && (x < x3_r)) {
         r = -1 * k * x + height;
    }

    // Calc green color
    if ((x > x1_g) && (x <= x2_g)) {
         g = k * x;
    } else if  ((x > x2_g) && (x < x3_g)) {
         g = (-1) * k * x + 2 * height;
    }

    // Calc blue color
    if ((x > x1_b) && (x <= x2_b)) {
         b = k * x - height;
    } else if  ((x > x2_b) && (x < x3_b)) {
         b = (-1) * k * x + 3 * height;
    }

    return qRgb((r * 255) / height, (g * 255) / height, (b * 255) / height);
}

void PaletePaint::paintEvent(QPaintEvent *event)
{
    QRgb colorRgb;
    QColor colorQ;
    int color;
    //QStyleOption opt;
    //opt.init(this);
    QPainter p(this);
    //style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    for (int k = 0; k < FIXED_W; k++) {
        colorRgb = getColor(k, FIXED_H, FIXED_W);
        colorQ.setRgb(colorRgb);
        color = static_cast<int>(colorRgb);
        //qDebug()<<"Color: "<<k<<" "<<"R:"<<(0xFF & (color>>16))<<" G:"<<(0xFF & (color >> 8))<<" B:"<<(0xFF & color);
        p.setPen(colorQ);
        p.drawLine(k, 0, k, FIXED_H - 1);
    }
}

void PaletePaint::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"PaletePaint: MousePress";
    mousePress = true;
}

void PaletePaint::mouseReleaseEvent(QMouseEvent *event)
{
    int x = 0;

    mousePress = true;
    qDebug()<<"PaletePaint: MouseRelease";
    if (event->pos().x() > FIXED_W) {
       x= FIXED_W - 1;
    } else if (event->pos().x() < 0) {
        x= 0;
    } else {
        x= event->pos().x();
    }
    emit finishColorUpdate(getColor(x, 0, FIXED_W));
}

void PaletePaint::mouseMoveEvent(QMouseEvent *event)
{
    int x = 0;

    if (mousePress == false){
        return;
    }
    if (event->pos().x() > FIXED_W) {
       x= FIXED_W - 1;
    } else if (event->pos().x() < 0) {
        x= 0;
    } else {
        x= event->pos().x();
    }
    qDebug()<<"PaletePaint: MouseMove x:"<<event->pos().x();
    emit updateColor(getColor(x, 0, FIXED_W));
}
