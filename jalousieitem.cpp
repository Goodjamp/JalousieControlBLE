#include "jalousieitem.h"
#include "ui_jalousieitem.h"
#include "jalousiepaint.h"
#include "QColorDialog"


#define DEFAULT_COLLOR    "#FFFFFF"

JalousieItem::JalousieItem(void *uContext, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JalousieItem)
{
    userContext = uContext;
    color.setNamedColor(DEFAULT_COLLOR);
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

void JalousieItem::on_LightControl_clicked()
{
    onLight = !onLight;
    lightOn(userContext, onLight, color);
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
}
