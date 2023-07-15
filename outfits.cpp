#include "outfits.h"
#include "ui_outfits.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QBuffer>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>

outfits::outfits(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::outfits)
{
    ui->setupUi(this);

    // Load images
    loadImages("Tops", topsImages, 211);
    loadImages("Bottoms", bottomsImages, 261);
    loadImages("Shoes", shoesImages, 121);

    // Initialize the current indexes
    currentTopIndex = 0;
    currentBottomIndex = 0;
    currentShoeIndex = 0;

    // Display the initial images
    if (!topsImages.empty()) {
        ui->topsView->setPixmap(topsImages[currentTopIndex]);
    } else {
        qDebug() << "No tops images loaded.";
    }

    if (!bottomsImages.empty()) {
        ui->bottomsVIew->setPixmap(bottomsImages[currentBottomIndex]);
    } else {
        qDebug() << "No bottoms images loaded.";
    }

    if (!shoesImages.empty()) {
        ui->shoesView->setPixmap(shoesImages[currentShoeIndex]);
    } else {
        qDebug() << "No shoes images loaded.";
    }

    // Connect buttons with slots
    connect(ui->rightTops, &QPushButton::clicked, this, [this] { changeTop(1); });
    connect(ui->leftTops, &QPushButton::clicked, this, [this] { changeTop(-1); });

    connect(ui->rightBotttoms, &QPushButton::clicked, this, [this] { changeBottom(1); });
    connect(ui->leftBottoms, &QPushButton::clicked, this, [this] { changeBottom(-1); });

    connect(ui->rightShoes, &QPushButton::clicked, this, [this] { changeShoe(1); });
    connect(ui->leftShoes, &QPushButton::clicked, this, [this] { changeShoe(-1); });

}

outfits::~outfits()
{
    delete ui;
}

void outfits::loadImages(const QString& tableName, QVector<QPixmap>& images, int imageHeight) {
    // Reading images from database
    QSqlQuery query;
    query.prepare(QString("SELECT image FROM %1").arg(tableName));
    if (query.exec()) {
        while (query.next()) {
            // Conversion QByteArray to QPixmap
            QByteArray byteArray = query.value(0).toByteArray();
            QPixmap pixmap = QPixmap();
            if (!pixmap.loadFromData(byteArray, "PNG")) {
                qDebug() << "Failed to load image from database";
            } else {
                // Scalling QPixmap height to imageHeight pixels
                pixmap = pixmap.scaledToHeight(imageHeight, Qt::SmoothTransformation);
                // Add to images
                images.append(pixmap);
            }
        }
        qDebug() << "Loaded" << images.size() << "images from" << tableName;
    } else {
        qDebug() << "Error loading image from database: ";
    }
}


void outfits::changeTop(int increment) {
    if (!topsImages.empty()) {
        currentTopIndex = (currentTopIndex + increment + topsImages.size()) % topsImages.size();
        ui->topsView->setPixmap(topsImages[currentTopIndex]);
    } else {
        qDebug() << "No tops images loaded.";
    }
}

void outfits::changeBottom(int increment) {
    if (!bottomsImages.empty()) {
        currentBottomIndex = (currentBottomIndex + increment + bottomsImages.size()) % bottomsImages.size();
        ui->bottomsVIew->setPixmap(bottomsImages[currentBottomIndex]);
    } else {
        qDebug() << "No bottoms images loaded.";
    }
}

void outfits::changeShoe(int increment) {
    if (!shoesImages.empty()) {
        currentShoeIndex = (currentShoeIndex + increment + shoesImages.size()) % shoesImages.size();
        ui->shoesView->setPixmap(bottomsImages[currentShoeIndex]);
    } else {
        qDebug() << "No shoes images loaded.";
    }
}
