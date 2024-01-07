#include "customspiner.h"
#include "ui_customspiner.h"

CustomSpiner::CustomSpiner(QWidget *parent, QSize size, int posX, int posY) :
    QWidget(parent),
    ui(new Ui::CustomSpiner)
{
    ui->setupUi(this);

    this->setGeometry(posX, posY, size.height(), size.width());

    spinnerLabel = new QLabel(this);
    spinnerLabel->setGeometry(0, 0, size.width(), size.height());
    spinnerLabel->setStyleSheet("background-color: rgb(100, 100, 100, 200)");
    spinner = new QMovie(":/Animation/TimeSpiner_1.gif", NULL, this);
    spinner->setScaledSize(size);
    spinnerLabel->setMovie(spinner);
    spinner->setSpeed(100);
    spinner->start();

    spinnerLabel->setVisible(true);
    setWindowModality(Qt::WindowModality::WindowModal);
    show();
}

CustomSpiner::~CustomSpiner()
{
    delete ui;
}
