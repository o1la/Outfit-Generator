#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "outfits.h"

#include <QMainWindow>

#include <QSql>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupDatabase();

private slots:
    void on_AddTop_clicked();
    void on_AddBottom_clicked();
    void on_AddShoes_clicked();

    void insertImage(const QString& tableName, const QString& fileName);
    void displayImages(const QString& tableName, QScrollArea* scrollArea);
    void deleteAllImagesFromTable(const QString& tableName);

    void on_generateOutfits_clicked();
    void clearScrollArea(QScrollArea* scrollArea);

private:
    Ui::MainWindow *ui;
    outfits* outfitsWindow;
};
#endif // MAINWINDOW_H
