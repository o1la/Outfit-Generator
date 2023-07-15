#include "selectionwindow.h"
#include "ui_selectionwindow.h"

#include <QPushButton>
#include <QGridLayout>

SelectionWindow::SelectionWindow(QVector<QPixmap>& images, QWidget *parent) :
    QDialog(parent),
    images(images),
    ui(new Ui::SelectionWindow)
{
    ui->setupUi(this);

    auto *layout = new QGridLayout(this);
    for (int i = 0; i < images.size(); ++i) {
        auto *button = new QPushButton(this);
        button->setIcon(QIcon(images[i]));
        button->setIconSize(images[i].size());
        connect(button, &QPushButton::clicked, this, [this, i] { on_itemSelected(i); });
        layout->addWidget(button, i / 5, i % 5); // 5 items per row
    }
    setLayout(layout);
}


SelectionWindow::~SelectionWindow()
{
    delete ui;
}

void SelectionWindow::on_itemSelected(int index) {
    // Emit a signal to inform the main window about the selection
    emit itemSelected(index);
    close();
}
