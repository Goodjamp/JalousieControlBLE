#include "jalousiepaint.h"

#include "QSize"
#include "QDebug"
#include "QPainter"
#include "QStyleOption"
#include "QtMath"
#include "QMouseEvent"

#define MINIMUM_H      0
#define MAXIMUM_H      1920
#define FIXED_W        150
#define J_W_PERCENT    80
#define J_H_PERCENT    90
#define D_J_PERCENT    5
#define ANGLE_MIN      3
#define ANGLE_MAX      (180 - ANGLE_MIN)
#define D_ANGLE        2

JalousiePaint::JalousiePaint(QWidget *parent) :
    QWidget(parent)
{
    /*
     *  Set geometry settings
     */
    this->setStyleSheet("background-color: rgb(217, 217, 217);");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    this->setMinimumSize(FIXED_W, MINIMUM_H);
    this->setMaximumSize(FIXED_W, MAXIMUM_H);

    connect(&valueTimer, &QTimer::timeout, this, &JalousiePaint::valueTimeoute);

    angle = ANGLE_MIN;
}

JalousiePaint::~JalousiePaint()
{}

void JalousiePaint::paintEvent(QPaintEvent *event)
{
    qreal radAngle = (M_PI * angle) / 180;

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));

    int jalAreaWidth = (this->width() * J_W_PERCENT) / 100;
    int jalAreaHeight = (this->height() * J_H_PERCENT) / 100;
    int jalWidth = jalAreaWidth;
    int jalCnt = jalAreaHeight / jalAreaWidth;
    int jalX0 = this->width() / 2;
    int jalY0 = (jalAreaHeight - jalCnt * jalAreaWidth) / 2 + jalWidth / 2 + ((this->height() * (100 - J_H_PERCENT)) / 100) / 2;
    int jalRad = ((jalWidth * (D_J_PERCENT + 100)) / 100) / 2;
    for (int k = 0; k < jalCnt; k++) {
        p.drawLine(jalX0 + jalRad * qSin(radAngle), (jalY0 + k * jalWidth) + jalRad * qCos(radAngle),
                   jalX0 - jalRad * qSin(radAngle), (jalY0 + k * jalWidth) - jalRad * qCos(radAngle));
    }
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(jalX0, ((this->height() * (100 - J_H_PERCENT)) / 100) / 2,
               jalX0, this->height() - ((this->height() * (100 - J_H_PERCENT)) / 100) / 2);
}

bool JalousiePaint::setEngle(int inAngle)
{
    if (inAngle > ANGLE_MAX
        || inAngle > ANGLE_MIN) {
        return false;
    }
    angle = inAngle;
    update();
    return true;
}

void JalousiePaint::mouseMoveEvent(QMouseEvent *event)
{
    if (coord.y() > event->pos().y()) {
        if (coord.y() - event->pos().y() > 5) {
            coord = event->pos();
            if (angle >= (ANGLE_MIN + D_ANGLE)) {
                angle += -D_ANGLE;
                valueTimer.start(valueTimeout);
                update();
            }
        }
    } else {
        if (event->pos().y() - coord.y() > 5) {
            coord = event->pos();
            if (angle <= (ANGLE_MAX - D_ANGLE)) {
                angle += D_ANGLE;
                valueTimer.start(valueTimeout);
                update();
            }
        }
    }
}

void JalousiePaint::mousePressEvent(QMouseEvent *event)
{
   coord = event->pos();
}

void JalousiePaint::valueTimeoute()
{
    int rezValue = (100 * (angle - ANGLE_MIN)) / (ANGLE_MAX - ANGLE_MIN);
    valueTimer.stop();
    qDebug()<<"JalousiePaint: value Ready "<<rezValue;
    emit valueReady(rezValue);
}
