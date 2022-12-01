#ifndef JALOUSIEPAINT_H
#define JALOUSIEPAINT_H

#include <QWidget>
#include <QPoint>
#include <QTimer>

class JalousiePaint: public QWidget
{
    Q_OBJECT

public:
    JalousiePaint(QWidget *parent = 0);
    ~JalousiePaint();

    bool setEngle(int inAngle);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QTimer valueTimer;
    QPoint coord;
    int angle;
    const int valueTimeout = 2000;

private slots:
    void valueTimeoute();

signals:
    void valueReady(int value);

};

#endif // JALOUSIEPAINT_H
