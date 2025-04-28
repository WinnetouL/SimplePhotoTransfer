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
    void moveSelectedImages();
    void onImageSelectionChanged();
    void setupUIBasedOnPathAccessibility();

private:
    QDir createDestinationDir();
    void finaliseUI();
    void loadImagesIntoListWidget(QDir);
    bool moveFileToFolder(const QString& sourceFilePath, const QDir targetFolderPath);

    Ui::SimplePhotoTransfer *ui;
    ConfigReader configReader;
    QStringList selectedImagePaths;
};
#endif // SIMPLEPHOTOTRANSFER_H
