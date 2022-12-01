#include "jalousieitem.h"
#include "ui_jalousieitem.h"
#include "jalousiepaint.h"
#include "QColorDialog"
#include "QDebug"

#define DEFAULT_COLLOR    "#FFFFFF"

JalousieItem::JalousieItem(void *uContext, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JalousieItem)
{
    userContext = uContext;
    currentColor = qRgb(0xFF, 0xFF, 0xFF);
    ui->setupUi(this);

    jalousiePaint = new JalousiePaint(this);
    connect(jalousiePaint, &JalousiePaint::valueReady, this, &JalousieItem::valueReady);
    static_cast<QHBoxLayout *>(ui->horizontalLayout_2->layout())->insertWidget(1, jalousiePaint);
    jalousiePaint->show();
}

void JalousieItem::valueReady(int value)
{
    emit posChange(userContext, value);
}

void JalousieItem::updateUiColorStyle(void)
{
    QString colorStyle = "background-color: rgb(" + QString::number(qRed(currentColor)) + ","
                                                  + QString::number(qGreen(currentColor)) + ","
                                                  + QString::number(qBlue(currentColor)) + ");";
    ui->MoveRight->setStyleSheet(colorStyle);
    ui->MoveLeft->setStyleSheet(colorStyle);
    ui->ColorControl->setStyleSheet(colorStyle);
    ui->LightControl->setStyleSheet(colorStyle);
}

void JalousieItem::getColor(QRgb color)
{
    currentColor = color;
    updateUiColorStyle();
    qDebug()<<"Rx color: "<<"R:"<<(0xFF & (color>>16))<<" G:"<<(0xFF & (color >> 8))<<" B:"<<(0xFF & color);
}

void JalousieItem::on_LightControl_clicked()
{
    onLight = !onLight;
    lightOn(userContext, onLight, currentColor);
}

void JalousieItem::on_MoveLeft_clicked()
{
    emit move(userContext, true);
}

void JalousieItem::on_MoveRight_clicked()
{
    emit move(userContext, false);
}

void JalousieItem::on_ColorControl_clicked()
{
    colorPalete = new PaleteForm(this);
    connect(colorPalete, &PaleteForm::updateColor, this, &JalousieItem::getColor);
    colorPalete->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    colorPalete->setWindowModality(Qt::ApplicationModal);
    colorPalete->show();
    colorPalete->resize(150, 300);
    colorPalete->move(300, 300);
}
