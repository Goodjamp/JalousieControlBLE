#ifndef JALOUSIEITEM_H
#define JALOUSIEITEM_H

#include <QWidget>
#include <QColor>

#include "jalousiepaint.h"

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
    JalousiePaint *jalousiePaint;
    QWidget *newWij;

    Ui::JalousieItem *ui;
    void *userContext;
    bool onLight;
    QColor color;

private slots:
    void valueReady(int value);
    void on_LightControl_clicked();
    void on_MoveLeft_clicked();
    void on_MoveRight_clicked();
    void on_ColorControl_clicked();

signals:
    void posChange(void *uContext, int value);
    void move(void *uContext, bool left);
    void lightOn(void *uContext, bool lightOn, QColor color);


};

#endif // JALOUSIEITEM_H
