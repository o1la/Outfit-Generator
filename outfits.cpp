#include "outfits.h"
#include "ui_outfits.h"
#include "selectionwindow.h"

#include <cstdlib>
#include <ctime>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QBuffer>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>
#include <QRandomGenerator>

outfits::outfits(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::outfits)
{
    ui->setupUi(this);
    this->setWindowTitle("");

    // Flattening buttons
    ui->topsView->setFlat(true);
    ui->bottomsView->setFlat(true);
    ui->shoesView->setFlat(true);

    // Style cursor pointer
    ui->topsView->setCursor(Qt::PointingHandCursor);
    ui->bottomsView->setCursor(Qt::PointingHandCursor);
    ui->shoesView->setCursor(Qt::PointingHandCursor);
    ui->leftTops->setCursor(Qt::PointingHandCursor);
    ui->rightTops->setCursor(Qt::PointingHandCursor);
    ui->leftBottoms->setCursor(Qt::PointingHandCursor);
    ui->rightBotttoms->setCursor(Qt::PointingHandCursor);
    ui->leftShoes->setCursor(Qt::PointingHandCursor);
    ui->rightShoes->setCursor(Qt::PointingHandCursor);

    // Load images
    loadImages("Tops", topsImages, 211);
    loadImages("Bottoms", bottomsImages, 261);
    loadImages("Shoes", shoesImages, 121);

    // Initialize the current indexes
    if (!topsImages.empty())
        currentTopIndex = QRandomGenerator::global()->bounded(topsImages.size());
    if (!bottomsImages.empty())
        currentBottomIndex = QRandomGenerator::global()->bounded(bottomsImages.size());
    if (!shoesImages.empty())
        currentShoeIndex = QRandomGenerator::global()->bounded(shoesImages.size());

    // Display the initial images
    if (!topsImages.empty()) {
        ui->topsView->setIcon(QIcon(topsImages[currentTopIndex]));
        ui->topsView->setIconSize(topsImages[currentTopIndex].size()); // Set the size of the icon
    } else {
        qDebug() << "No tops images loaded.";
    }

    if (!bottomsImages.empty()) {
        ui->bottomsView->setIcon(QIcon(bottomsImages[currentBottomIndex]));
        ui->bottomsView->setIconSize(bottomsImages[currentBottomIndex].size()); // Set the size of the icon
    } else {
        qDebug() << "No bottoms images loaded.";
    }

    if (!shoesImages.empty()) {
        ui->shoesView->setIcon(QIcon(shoesImages[currentShoeIndex]));
        ui->shoesView->setIconSize(shoesImages[currentShoeIndex].size()); // Set the size of the icon
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

    connect(ui->shuffleButton, &QPushButton::clicked, this, &outfits::on_shuffleButton_clicked);


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
        ui->topsView->setIcon(QIcon(topsImages[currentTopIndex]));
        ui->topsView->setIconSize(topsImages[currentTopIndex].size());
    } else {
        qDebug() << "No tops images loaded.";
    }
}

void outfits::changeBottom(int increment) {
    if (!bottomsImages.empty()) {
        currentBottomIndex = (currentBottomIndex + increment + bottomsImages.size()) % bottomsImages.size();
        ui->bottomsView->setIcon(QIcon(bottomsImages[currentBottomIndex]));
        ui->bottomsView->setIconSize(bottomsImages[currentBottomIndex].size());
    } else {
        qDebug() << "No bottoms images loaded.";
    }
}

void outfits::changeShoe(int increment) {
    if (!shoesImages.empty()) {
        currentShoeIndex = (currentShoeIndex + increment + shoesImages.size()) % shoesImages.size();
        ui->shoesView->setIcon(QIcon(shoesImages[currentShoeIndex]));
        ui->shoesView->setIconSize(shoesImages[currentShoeIndex].size());
    } else {
        qDebug() << "No shoes images loaded.";
    }
}

// Selecting clothes from all pics
void outfits::on_topsView_clicked() {

    auto *selectionWindow = new SelectionWindow(this->topsImages);
    connect(selectionWindow, &SelectionWindow::itemSelected, this, [this](int index) {
        currentTopIndex = index;
        ui->topsView->setIcon(QIcon(topsImages[currentTopIndex]));
        ui->topsView->setIconSize(topsImages[currentTopIndex].size());
    });
    selectionWindow->exec();
}

void outfits::on_bottomsView_clicked() {

    auto *selectionWindow = new SelectionWindow(this->bottomsImages);
    connect(selectionWindow, &SelectionWindow::itemSelected, this, [this](int index) {
        currentBottomIndex = index;
        ui->bottomsView->setIcon(QIcon(bottomsImages[currentBottomIndex]));
        ui->bottomsView->setIconSize(bottomsImages[currentBottomIndex].size());
    });
    selectionWindow->exec();
}

void outfits::on_shoesView_clicked() {

    auto *selectionWindow = new SelectionWindow(this->shoesImages);
    connect(selectionWindow, &SelectionWindow::itemSelected, this, [this](int index) {
        currentShoeIndex = index;
        ui->shoesView->setIcon(QIcon(shoesImages[currentShoeIndex]));
        ui->shoesView->setIconSize(shoesImages[currentShoeIndex].size());
    });
    selectionWindow->exec();
}

void outfits::on_shuffleButton_clicked() {
    // Seed the random number generator
    srand(time(0));

    if (!topsImages.empty()) {
        int randomTopIndex = rand() % topsImages.size();
        ui->topsView->setIcon(QIcon(topsImages[randomTopIndex]));
        ui->topsView->setIconSize(topsImages[randomTopIndex].size());
    }

    if (!bottomsImages.empty()) {
        int randomBottomIndex = rand() % bottomsImages.size();
        ui->bottomsView->setIcon(QIcon(bottomsImages[randomBottomIndex]));
        ui->bottomsView->setIconSize(bottomsImages[randomBottomIndex].size());
    }

    if (!shoesImages.empty()) {
        int randomShoeIndex = rand() % shoesImages.size();
        ui->shoesView->setIcon(QIcon(shoesImages[randomShoeIndex]));
        ui->shoesView->setIconSize(shoesImages[randomShoeIndex].size());
    }
}


