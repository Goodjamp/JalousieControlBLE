#ifndef PALETEPAINT_H
#define PALETEPAINT_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QRgb>
#include <QSize>

class PaletePaint : public QWidget
{
    Q_OBJECT
public:
    explicit PaletePaint(QWidget *parent = nullptr);
    QSize getSize();

signals:
    void updateColor(QRgb color);
    void finishColorUpdate(QRgb color);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
   QRgb getColor(int x, int height, int width);
   bool mousePress;

};

#endif // PALETEPAINT_H
