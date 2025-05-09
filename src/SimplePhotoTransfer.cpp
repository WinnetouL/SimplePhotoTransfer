#include "SimplePhotoTransfer.h"
#include "ui_SimplePhotoTransfer.h"

#include <QDir>
#include <QPushButton>
#include <QListWidget>
#include <QStandardPaths>

SimplePhotoTransfer::SimplePhotoTransfer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SimplePhotoTransfer)
{
    ui->setupUi(this);

    connect(ui->retryButton, &QPushButton::clicked, this, &SimplePhotoTransfer::setupUIBasedOnPathAccessibility);
    connect(ui->moveButton, &QPushButton::clicked, this, &SimplePhotoTransfer::moveSelectedImages);
    connect(ui->imageList, &QListWidget::itemSelectionChanged, this, &SimplePhotoTransfer::onImageSelectionChanged);

    setupUIBasedOnPathAccessibility();
}

SimplePhotoTransfer::~SimplePhotoTransfer()
{
    delete ui;
}

void SimplePhotoTransfer::setupUIBasedOnPathAccessibility()
{
    ui->moveButton->hide();

    QMap<QString, QString> config = configReader.readConfig();

    if (config.contains("error")) {
        ui->statusLabel->setText(config.value("error"));
        return;
    }

    ui->horizontalLayout->removeWidget(ui->retryButton);
    delete ui->retryButton;
    ui->retryButton = nullptr;

    ui->statusLabel->setText("Bitte wähle die Bilder aus, die du übertragen möchtest.");
    ui->moveButton->show();

    loadImagesIntoListWidget(QDir(config["search_directory"]));
}

void SimplePhotoTransfer::loadImagesIntoListWidget(QDir dirPath)
{
    ui->imageList->clear();

    QStringList filesToDisplay = getFilesToDisplay(dirPath);
    for (int i = 0; i < filesToDisplay.size(); ++i) {
        const QString &imageFile = filesToDisplay[i];
        QString imagePath = dirPath.filePath(imageFile);
        QPixmap pixmap(imagePath);
        QIcon icon(pixmap);
        QListWidgetItem *item = new QListWidgetItem(icon, imageFile);
        item->setData(Qt::UserRole, imagePath);
        item->setSizeHint(QSize(350, 310));
        ui->imageList->addItem(item);
    }
}

QStringList SimplePhotoTransfer::getFilesToDisplay(QDir dirPath) {
    QStringList imageFiles = dirPath.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);
    QStringList filesToDisplay;
    QList<QFileInfo> fileInfoList;

    for (int i = 0; i < imageFiles.size(); ++i) {
        QFileInfo fileInfo(dirPath.filePath(imageFiles[i]));
        fileInfoList.append(fileInfo);
    }

    std::sort(fileInfoList.begin(), fileInfoList.end(), [](const QFileInfo& a, const QFileInfo& b) {
        return a.lastModified() > b.lastModified();
    });

    int maxFiles = 3;
    int filesToLoad = qMin(fileInfoList.size(), maxFiles);
    for (int i = 0; i < filesToLoad; ++i) {
        const QFileInfo& fileInfo = fileInfoList[i];
        filesToDisplay.append(fileInfo.absoluteFilePath());
    }

    return filesToDisplay;
}

void SimplePhotoTransfer::onImageSelectionChanged()
{
    selectedImagePaths.clear();
    QList<QListWidgetItem*> selectedItems = ui->imageList->selectedItems();
    for (int i = 0; i < selectedItems.size(); ++i) {
        const QListWidgetItem* item = selectedItems[i];
        QString imagePath = item->data(Qt::UserRole).toString();
        selectedImagePaths.append(imagePath);
    }
}

void SimplePhotoTransfer::moveSelectedImages()
{
    if (selectedImagePaths.isEmpty()) {
        ui->statusLabel->setText("Es wurden keine Bilder ausgewählt.\n"
                                 "Bitte wähle die Bilder aus, die du übertragen möchtest."
                                 );
        return;
    }

    QDir destinationPath = createDestinationDir();

    for (int i = 0; i < selectedImagePaths.size(); ++i) {
        bool success = moveFileToFolder(selectedImagePaths[i], destinationPath);
        if (!success) {
            ui->statusLabel->setText("Bildübertragung ist fehlgeschlagen.");
            finaliseUI();
            return;
        }
    }

    finaliseUI();
    ui->statusLabel->setText("Bilder wurden erfolgreich verschoben.\n"
                             "Sie befinden sich im folgenden Ordner im Downloads-Verzeichnis:\n" +
                             destinationPath.dirName() +
                             "\n\nDas Programm kann geschlossen werden."
                            );
}

void SimplePhotoTransfer::finaliseUI() {
    selectedImagePaths.clear();

    ui->imageList->clear();
    ui->imageList->hide();
    ui->moveButton->hide();
}

QDir SimplePhotoTransfer::createDestinationDir()
{
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (downloadsPath.isEmpty()) {
        return QString();
    }

    QString dateTimeStr = QDateTime::currentDateTime().toString("dd_MM_yyyy-hh_mm_ss");
    QString folderName = QString("SimplePhotoTransfer-%1").arg(dateTimeStr);
    QString fullPath = downloadsPath + QDir::separator() + folderName;
    QDir dir;
    if (!dir.mkpath(fullPath)) {
        return QString();
    }

    return QDir(fullPath);
}

bool SimplePhotoTransfer::moveFileToFolder(const QString& sourceFilePath, const QDir targetFolderPath)
{
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.exists()) {
        return false;
    }

    QString fileName = QFileInfo(sourceFilePath).fileName();
    QString targetFilePath = targetFolderPath.filePath(fileName);

    return sourceFile.rename(targetFilePath);
}
