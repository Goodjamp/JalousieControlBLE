#ifndef CUSTOMSPINER_H
#define CUSTOMSPINER_H

#include <QWidget>
#include <QMovie>
#include <QLabel>
#include <QSize>

namespace Ui {
class CustomSpiner;
}

class CustomSpiner : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSpiner(QWidget *parent = nullptr, QSize size = QSize(100, 100), int posX = 0 , int posY = 0);
    ~CustomSpiner();

private:
    Ui::CustomSpiner *ui;


    QMovie *spinner;
    QLabel *spinnerLabel;
};

#endif // CUSTOMSPINER_H
