#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stdafx.h"
#include "server.hpp"
#include "client.hpp"
#include "peer.hpp"
#include "heartbeat.hpp"
#include "connection_manager.hpp"
#include "Steganography.hpp"


#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void _initialize();

    void on_sendImage_clicked();

    void on_AddImage_clicked();

    void on_availableClients_itemDoubleClicked(QTableWidgetItem *item);

    void on_ShowImage_clicked();

    void on_availableImages_highlighted(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString selectedFile; // file path of selected image to be sent
    QTableWidgetItem *selectedClient;
    int viewsCount=0;
    Steganography steg;
    QString selectedImage;
};

#endif // MAINWINDOW_H
