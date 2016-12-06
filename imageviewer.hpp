#ifndef IMAGEVIEWER_HPP
#define IMAGEVIEWER_HPP

#include <QDialog>

namespace Ui {
        class ImageViewer;
    }

class ImageViewer : public QDialog {
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    void setImage(QString imagePath);
private:
    Ui::ImageViewer *ui;
};

#endif // IMAGEVIEWER_HPP
