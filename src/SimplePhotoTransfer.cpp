#include "SimplePhotoTransfer.h"
#include "ui_SimplePhotoTransfer.h"

SimplePhotoTransfer::SimplePhotoTransfer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SimplePhotoTransfer)
{
    ui->setupUi(this);
}

SimplePhotoTransfer::~SimplePhotoTransfer()
{
    delete ui;
}
