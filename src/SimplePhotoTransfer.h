#ifndef SIMPLEPHOTOTRANSFER_H
#define SIMPLEPHOTOTRANSFER_H

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

private:
    Ui::SimplePhotoTransfer *ui;
};
#endif // SIMPLEPHOTOTRANSFER_H
