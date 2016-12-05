#include "upload.hpp"
#include "ui_upload.h"

void Upload::setPeers(std::vector<Peer*> *peers)
{
    _peers = peers;
    QTableWidget *peerTable = ui->peerTbl;
    peerTable->verticalHeader()->hide();
    peerTable->horizontalHeader()->hide();
    peerTable->setShowGrid(false);

    peerTable->setColumnCount(1);
    peerTable->setRowCount(peers->size());

    peerTable->setAlternatingRowColors(true);
    peerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    peerTable->setEditTriggers(QAbstractItemView::EditTriggers(0));
    peerTable->setSelectionMode(QAbstractItemView::MultiSelection);
    peerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    for (int i = 0; i < peers->size(); ++i) {
        peerTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString((*peers)[i]->getUsername())));

    }
}

Upload::Upload(QWidget *parent) :
    QDialog(parent), ui(new Ui::upload) {

    ui->setupUi(this);

}

Upload::~Upload() {
    delete ui;
}

void Upload::on_browseBtn_clicked() {

    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image to Send..", QDir::homePath(), tr("Images(*.jpg *.jpeg)"));
    ui->pathEdit->setText(imagePath);

    QPixmap *image = new QPixmap(imagePath);

    ui->imgView->setPixmap((*image));

}

void Upload::on_okBtn_clicked() {

    QItemSelectionModel *selection = ui->peerTbl->selectionModel();

    qDebug() << "-----------------------------================================";
    qDebug() << selection->selectedRows(0);
    qDebug() << "-----------------------------================================";
    qDebug() << "******************************************================================";

    _selectedPeers;

    for (int i = 0; i < selection->selectedRows().size(); i++) {

        qDebug() << selection->selectedRows().at(i).row();
        _selectedPeers.push_back(selection->selectedRows().at(i).row());

    }

    qDebug() << "******************************************================================";
    Package pkg;

    _image = ui->pathEdit->text().toStdString();

    this->close();

    emit ready();


}

void Upload::on_cancelBtn_clicked() {
    emit cancelled();
}

std::string Upload::image() const {
    return _image;
}

std::vector<int> Upload::selectedPeers() const {
    return _selectedPeers;
}


