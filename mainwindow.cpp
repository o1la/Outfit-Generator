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
    setupDatabase();

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
            pixmap.loadFromData(byteArray, "PNG");

            // Scalling QPixmap height to 230 pixels
            pixmap = pixmap.scaledToHeight(230, Qt::SmoothTransformation);

            // Creating QLabel, to display QPixmap
            QLabel* label = new QLabel;
            label->setPixmap(pixmap);

            // Adding QLabel to Scroll Area
            scrollArea->widget()->layout()->addWidget(label);
        }
    } else {
        qDebug() << "Error displaying image from database: ";
    }

}

// Deleting old images
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
        displayImages("Tops", ui->topsScrollArea);
    }
}

void MainWindow::on_AddBottom_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        insertImage("Bottoms", fileName);
        displayImages("Bottoms", ui->bottomsScrollArea);
    }
}

void MainWindow::on_AddShoes_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        insertImage("Shoes", fileName);
        displayImages("Shoes", ui->shoesScrollArea);
    }
}






