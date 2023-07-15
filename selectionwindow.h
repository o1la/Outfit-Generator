#ifndef SELECTIONWINDOW_H
#define SELECTIONWINDOW_H

#include <QDialog>
#include <QVector>
#include <QPixmap>

namespace Ui {
class SelectionWindow;
}

class SelectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionWindow(QVector<QPixmap>& images, QWidget *parent = nullptr);
    ~SelectionWindow();

private:
    Ui::SelectionWindow *ui;
    QVector<QPixmap>& images;

private slots:
    void on_itemSelected(int index);

signals:
    void itemSelected(int index);
};

#endif // SELECTIONWINDOW_H
