#include "paleteform.h"
#include "ui_paleteform.h"
#include <QDebug>

PaleteForm::PaleteForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PaleteForm)
{
    ui->setupUi(this);
    palete = new PaletePaint(this);
    ui->centralwidget->setFixedSize(palete->size());
    palete->show();
    connect(palete, &PaletePaint::updateColor,       this, &PaleteForm::updateColor);
    connect(palete, &PaletePaint::finishColorUpdate, this, &PaleteForm::closePalete);
}

PaleteForm::~PaleteForm()
{
    delete ui;
}

void PaleteForm::closePalete(QRgb color)
{
    emit updateColor(color);
    emit updateColorComplete();
    this->close();
}
