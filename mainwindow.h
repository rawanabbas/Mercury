#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QDialog>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDir>
#include <QFileDialog>

#include "client.hpp"
#include "connection_manager.hpp"
#include "heartbeat.hpp"
#include "peer.hpp"
#include "server.hpp"


#include "login.hpp"
#include "upload.hpp"


typedef std::map<std::string, Client*> ClientMap;

namespace Ui {
        class MainWindow;
    }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    std::string getId() const;
    void setId(const std::string &id);

    std::string getUsername() const;
    void setUsername(const std::string &username);

    virtual ~MainWindow();

    void _updatePeerTable(std::vector<Peer*> peers, PeerMap peerMap);
    void _setupPeerTable();

    std::vector<Peer *> getPeers() const;

public slots:

    void display();
    void updatePeerTable();
    void upload();
    void cancelUpload();

private slots:

    void on_uploadBtn_clicked();

private:

    Ui::MainWindow *ui;
    Login *_login;
    Upload *_upload;

    //Middleware
    Server *_server;
    ConnectionManager *_manager;
    Heartbeat *_heartbeat;
    ClientMap _clients;

    std::string _username;
    std::string _id;

    std::vector<Peer*> _peers;
    PeerMap _onlinePeers;
};

#endif // MAINWINDOW_H
