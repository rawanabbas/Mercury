#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_initialize()
{
    // display available clients
    peers = manager.getPeers();

    if (peers.size()) {

        PeerMap::iterator it;
        int index = 0;
        int rowCount=0;
        for (it = peers.begin(); it != peers.end(); ++it) {
            ui->availableClients->insertRow(ui->availableClients->rowCount());
            ui->availableClients->setItem(rowCount,0,QTableWidgetItem(QString(QString::number(++index) + " : " + it -> first)) );
        }

    }
    /*
     * query images to view all images of client
     */
}

void MainWindow::on_availableClients_itemDoubleClicked(QTableWidgetItem *item)
{
    selectedClient=item;
}


void MainWindow::on_sendImage_clicked()
{
    if(ui->viewsBox->toPlainText()!="")
    {
        QString temp=ui->viewsBox->toPlainText();
        //qintvalidator
        viewsCount=temp[0].toInt();
    }

    /*
     * loop on all selected clients
     * perform stegonagraphy of views count and sends the image
     */

}

void MainWindow::on_AddImage_clicked()
{
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("/Users/Seba/home")
         << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    QFileDialog dialog;
    dialog.setSidebarUrls(urls);
    dialog.setFileMode(QFileDialog::AnyFile);
    if(dialog.exec()) {
        selectedFile = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("Images (*.png *.xpm *.jpg)"));
    }
}

void MainWindow::on_ShowImage_clicked()
{
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(selectedImage));
    ui->viewImage->addItem(item);
    ui->viewImage->show();
}

void MainWindow::on_availableImages_highlighted(const QString &arg1)
{
    selectedImage= arg1;
    // open selected image
}
