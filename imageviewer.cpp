#include "imageviewer.hpp"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageViewer) {
    ui->setupUi(this);
}

ImageViewer::~ImageViewer() {
    delete ui;
}

void ImageViewer::setImage(QString imagePath) {
    ui->label->setPixmap(QPixmap(imagePath));
}
