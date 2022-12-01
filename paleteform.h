#ifndef PALETEFORM_H
#define PALETEFORM_H

#include <QMainWindow>
#include "paletepaint.h"
#include "QRgb"

namespace Ui {
class PaleteForm;
}

class PaleteForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit PaleteForm(QWidget *parent = nullptr);
    ~PaleteForm();

signals:
    void updateColor(QRgb targetColor);

private:
    Ui::PaleteForm *ui;
    PaletePaint *palete;

private slots:
    void closePalete(QRgb color);
};

#endif // PALETEFORM_H
