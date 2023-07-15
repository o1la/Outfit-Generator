#ifndef OUTFITS_H
#define OUTFITS_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>
#include <QScrollArea>

namespace Ui {
class outfits;
}

class outfits : public QDialog
{
    Q_OBJECT

public:
    explicit outfits(QWidget *parent = nullptr);
    ~outfits();

private:
    Ui::outfits *ui;
    bool selectionInProgress = false;

    QVector<QPixmap> topsImages;
    QVector<QPixmap> bottomsImages;
    QVector<QPixmap> shoesImages;
    int currentTopIndex;
    int currentBottomIndex;
    int currentShoeIndex;

private slots:
    void loadImages(const QString& tableName, QVector<QPixmap>& images, int imageHeight);

    void changeTop(int increment);
    void changeBottom(int increment);
    void changeShoe(int increment);

    void on_topsView_clicked();
    void on_bottomsView_clicked();
    void on_shoesView_clicked();
};

#endif // OUTFITS_H
