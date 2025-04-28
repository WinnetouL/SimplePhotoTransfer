#ifndef SIMPLEPHOTOTRANSFER_H
#define SIMPLEPHOTOTRANSFER_H

#include "ConfigReader.h"

#include <QDir>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class SimplePhotoTransfer;
}
QT_END_NAMESPACE

class SimplePhotoTransfer : public QWidget
{
    Q_OBJECT

public:
    SimplePhotoTransfer(QWidget *parent = nullptr);
    ~SimplePhotoTransfer();

private slots:
    void setupUIBasedOnPathAccessibility();

private:
    void loadImagesIntoListWidget(QDir);

    Ui::SimplePhotoTransfer *ui;
    ConfigReader configReader;
};
#endif // SIMPLEPHOTOTRANSFER_H
