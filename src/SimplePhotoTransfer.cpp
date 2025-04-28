#include "SimplePhotoTransfer.h"
#include "ui_SimplePhotoTransfer.h"

#include <QDir>
#include <QPushButton>
#include <QListWidget>

SimplePhotoTransfer::SimplePhotoTransfer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SimplePhotoTransfer)
{
    ui->setupUi(this);

    connect(ui->retryButton, &QPushButton::clicked, this, &SimplePhotoTransfer::setupUIBasedOnPathAccessibility);

    setupUIBasedOnPathAccessibility();
}

SimplePhotoTransfer::~SimplePhotoTransfer()
{
    delete ui;
}

void SimplePhotoTransfer::setupUIBasedOnPathAccessibility()
{
    QMap<QString, QString> config = configReader.readConfig();

    if (config.contains("error")) {
        qWarning() << config.value("error");  // TODO remove
        ui->statusLabel->setText(config.value("error"));
        return;
    }

    qDebug() << "Search Directory:" << config["search_directory"];  // TODO remove
    ui->horizontalLayout->removeWidget(ui->retryButton);
    delete ui->retryButton;
    ui->retryButton = nullptr;

    ui->statusLabel->setText("Bitte wähle die Bilder aus, die du übertragen möchtest.");

    loadImagesIntoListWidget(QDir(config["search_directory"]));
}

void SimplePhotoTransfer::loadImagesIntoListWidget(QDir dirPath)
{
    QStringList imageFiles = dirPath.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);

    ui->imageList->clear();

    for (int i = 0; i < imageFiles.size(); ++i) {
        const QString &imageFile = imageFiles[i];
        QString imagePath = dirPath.filePath(imageFile);
        QPixmap pixmap(imagePath);
        QIcon icon(pixmap);
        QListWidgetItem *item = new QListWidgetItem(icon, imageFile);
        item->setData(Qt::UserRole, imagePath);
        ui->imageList->addItem(item);
    }
}
