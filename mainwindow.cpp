#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QBuffer>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Outfit Generator");
    setupDatabase();

    // Style cursor pointer
    ui->AddTop->setCursor(Qt::PointingHandCursor);
    ui->AddBottom->setCursor(Qt::PointingHandCursor);
    ui->AddShoes->setCursor(Qt::PointingHandCursor);
    ui->generateOutfits->setCursor(Qt::PointingHandCursor);
    ui->tabWidget->setCursor(Qt::PointingHandCursor);

    // Creating QHBoxLayout for Scroll Area
    QHBoxLayout *topsLayout = new QHBoxLayout;
    QHBoxLayout *bottomsLayout = new QHBoxLayout;
    QHBoxLayout *shoesLayout = new QHBoxLayout;

    // Setting layout for Scroll Area
    ui->topsScrollArea->widget()->setLayout(topsLayout);
    ui->bottomsScrollArea->widget()->setLayout(bottomsLayout);
    ui->shoesScrollArea->widget()->setLayout(shoesLayout);

    // Displaying existing images
    displayImages("Tops", ui->topsScrollArea);
    displayImages("Bottoms", ui->bottomsScrollArea);
    displayImages("Shoes", ui->shoesScrollArea);

}


MainWindow::~MainWindow()
{
    delete ui;
}

// DATABASE
void MainWindow::setupDatabase()
{
    // Opening connection to SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/aleksandragrygorczyk/Documents/mydatabase.db");

    if (!db.open()) {
        qDebug()<<("Cannot open database");
        return;
    }

    // Creating tables for clothes
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Tops (id INTEGER PRIMARY KEY AUTOINCREMENT, image BLOB)");
    query.exec("CREATE TABLE IF NOT EXISTS Bottoms (id INTEGER PRIMARY KEY AUTOINCREMENT, image BLOB)");
    query.exec("CREATE TABLE IF NOT EXISTS Shoes (id INTEGER PRIMARY KEY AUTOINCREMENT, image BLOB)");

}

// IMAGES
void MainWindow::insertImage(const QString& tableName, const QString& fileName) {
    QPixmap pixmap(fileName);

    // Converting QPixmap to QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    // Inserting images to database
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (image) VALUES (:image)").arg(tableName));
    query.bindValue(":image", byteArray);
    if (!query.exec()) {
        qDebug() << "Błąd wstawiania obrazu do bazy danych: ";
    }
}

void MainWindow::displayImages(const QString& tableName, QScrollArea* scrollArea) {

    // Reading images from database
    QSqlQuery query;
    query.prepare(QString("SELECT image FROM %1").arg(tableName));
    if (query.exec()) {
        while (query.next()) {
            // Conversion QByteArray to QPixmap
            QByteArray byteArray = query.value(0).toByteArray();
            QPixmap pixmap = QPixmap();
            if(!pixmap.loadFromData(byteArray, "PNG")) {
                qDebug() << "Failed to load image from database";
            } else {
                // Scalling QPixmap height to 220 pixels
                pixmap = pixmap.scaledToHeight(220, Qt::SmoothTransformation);

                // Creating QLabel, to display QPixmap
                QLabel* label = new QLabel;
                label->setPixmap(pixmap);

                // Adding QLabel to Scroll Area
                scrollArea->widget()->layout()->addWidget(label);
            }
        }
    } else {
        qDebug() << "Error displaying image from database: ";
    }
}

// DELETING old images
void MainWindow::deleteAllImagesFromTable(const QString& tableName) {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1").arg(tableName));
    if (!query.exec()) {
        qDebug() << "Error deleting images from database: ";
    }
}

void MainWindow::clearScrollArea(QScrollArea* scrollArea) {
    QLayout* layout = scrollArea->widget()->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}


// ADDING PHOTOS
void MainWindow::on_AddTop_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        insertImage("Tops", fileName);
        clearScrollArea(ui->topsScrollArea);
        displayImages("Tops", ui->topsScrollArea);

        // Setting current open tab to tops
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_AddBottom_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        insertImage("Bottoms", fileName);
        clearScrollArea(ui->bottomsScrollArea);
        displayImages("Bottoms", ui->bottomsScrollArea);

        // Setting current open tab to bottoms
        ui->tabWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_AddShoes_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        insertImage("Shoes", fileName);
        clearScrollArea(ui->shoesScrollArea);
        displayImages("Shoes", ui->shoesScrollArea);

        // Setting current open tab to shoes
        ui->tabWidget->setCurrentIndex(2);

    }
}


// Generate outfits window
void MainWindow::on_generateOutfits_clicked() {
    outfitsWindow = new outfits(this);
    outfitsWindow->show();
}


// Delete all photos
void MainWindow::on_deletePhotos_clicked() {
    deleteAllImagesFromTable("Tops");
    deleteAllImagesFromTable("Bottoms");
    deleteAllImagesFromTable("Shoes");

    clearScrollArea(ui->topsScrollArea);
    clearScrollArea(ui->bottomsScrollArea);
    clearScrollArea(ui->shoesScrollArea);
}

