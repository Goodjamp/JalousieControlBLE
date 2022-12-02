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

static const int k =  (6 * FIXED_H) / FIXED_W;

static const int x1_r = (4 * FIXED_W) / 6;
static const int x2_r = (5 * FIXED_W) / 6;
static const int x3_r = (1 * FIXED_W) / 6;
static const int x4_r = (2 * FIXED_W) / 6;

static const int x1_g = (0 * FIXED_W) / 6;
static const int x2_g = (1 * FIXED_W) / 6;
static const int x3_g = (3 * FIXED_W) / 6;
static const int x4_g = (4 * FIXED_W) / 6;

static const int x1_b = (2 * FIXED_W) / 6;
static const int x2_b = (3 * FIXED_W) / 6;
static const int x3_b = (5 * FIXED_W) / 6;
static const int x4_b = (6 * FIXED_W) / 6;

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

QRgb PaletePaint::getColor(int x, int y)
{
    int r = 0, g = 0, b = 0;

    // Calc red color
    if ((x > x1_r) && (x <= x2_r)) {
         r = k * x - k * x1_r;
    } else if (((x > x2_r) && (x <= FIXED_W))
               || ((x > 0) && (x <= x3_r))) {
         r = FIXED_H;
    } else if  ((x > x3_r) && (x < x4_r)) {
         r = (-1) * k * x + k * x4_r;
    }

    // Calc green color
    if ((x > x1_g) && (x <= x2_g)) {
         g = k * x - k * x1_g;
    } else if ((x > x2_g) && (x <= x3_g)) {
         g = FIXED_H;
    } else if  ((x > x3_g) && (x < x4_g)) {
         g = (-1) * k * x + k * x4_g;
    }

    // Calc blue color
    if ((x > x1_b) && (x <= x2_b)) {
         b = k * x - k * x1_b;
    } else if ((x > x2_b) && (x <= x3_b)) {
         b = FIXED_H;
    } else if  ((x > x3_b) && (x < x4_b)) {
         b = (-1) * k * x + k * x4_b;
    }

    r = (r * 255) / FIXED_H;
    g = (g * 255) / FIXED_H;
    b = (b * 255) / FIXED_H;

    r = (r * (FIXED_H - y)) / FIXED_H;
    g = (g * (FIXED_H - y)) / FIXED_H;
    b = (b * (FIXED_H - y)) / FIXED_H;

    return qRgb(r , g, b);
}

void PaletePaint::paintEvent(QPaintEvent *event)
{
    QColor colorQ;
    QPainter p(this);

    (void)event;
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    for (int k = 0; k < FIXED_W; k++) {
        for (int y = 0; y < FIXED_H; y++) {

            // 3/4 special scaling ciefficient to provide
            // nice color presetation
            colorQ.setRgb(getColor(k, (y * 3) / 4));
            p.setPen(colorQ);
            p.drawPoint(k, y);
        }
    }
}

void PaletePaint::mousePressEvent(QMouseEvent *event)
{
    (void)event;
    mousePress = true;
}

void PaletePaint::mouseReleaseEvent(QMouseEvent *event)
{
    int x = 0;
    int y = 0;

    mousePress = true;
    if (event->pos().x() > FIXED_W) {
       x= FIXED_W - 1;
    } else if (event->pos().x() < 0) {
        x= 0;
    } else {
        x= event->pos().x();
    }
    if (event->pos().y() > FIXED_H) {
       y= FIXED_H - 1;
    } else if (event->pos().y() < 0) {
        y= 0;
    } else {
        y= event->pos().y();
    }


    emit finishColorUpdate(getColor(x, y));
}

void PaletePaint::mouseMoveEvent(QMouseEvent *event)
{
    int x = 0;
    int y = 0;

    if (mousePress == false){
        return;
    }

    /*Calculate  x pos*/
    if (event->pos().x() > FIXED_W) {
       x= FIXED_W - 1;
    } else if (event->pos().x() < 0) {
        x= 0;
    } else {
        x= event->pos().x();
    }

    /*Calculate y pos*/
    if (event->pos().y() > FIXED_H) {
       y= FIXED_H - 1;
    } else if (event->pos().y() < 0) {
        y= 0;
    } else {
        y= event->pos().y();
    }
    emit updateColor(getColor(x, y));
}
