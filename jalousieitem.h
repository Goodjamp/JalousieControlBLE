#ifndef JALOUSIEITEM_H
#define JALOUSIEITEM_H

#include <QWidget>
#include <QRgb>

#include "jalousiepaint.h"
#include "paletepaint.h"
#include "paleteform.h"

namespace Ui {
class JalousieItem;
}

class JalousieItem : public QWidget
{
    Q_OBJECT

public:
    JalousieItem(void *uContext = nullptr, QWidget *parent = nullptr);
    void addDebugMess(QString &message);

private:
    PaleteForm *colorPalete;
    JalousiePaint *jalousiePaint;
    QWidget *newWij;

    Ui::JalousieItem *ui;
    void *userContext;
    bool onLight;
    QRgb currentColor;

    void updateUiColorStyle(void);

private slots:
    void valueReady(int value);
    void getColor(QRgb color);
    void txColor(void);

    void on_LightControl_clicked();
    void on_MoveLeft_clicked();
    void on_MoveRight_clicked();
    void on_ColorControl_clicked();

signals:
    void posChange(void *uContext, int value);
    void move(void *uContext, bool left);
    void lightOn(void *uContext, bool lightOn, QRgb color);

};

#endif // JALOUSIEITEM_H
