#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), _manager(NULL), _server(NULL), _heartbeat(NULL) {



    ui->setupUi(this);

    _login = new Login(this);
    _login->setWindowTitle("Login");
    _login->show();

    _upload = new Upload(this);
    _upload->setWindowTitle("Upload Image");

    connect(_login, SIGNAL(authenticated()), this, SLOT(display()));
    connect(_upload, SIGNAL(ready()), this, SLOT(upload()));
    connect(_upload, SIGNAL(cancelled()), this, SLOT(cancelUpload()));

}


std::string MainWindow::getId() const {
    return _id;
}

void MainWindow::setId(const std::string &id) {
    _id = id;
}


void MainWindow::display() {

    setUsername(_login->getUsername());
    setId(_login->getId());
    _login->close();

    _server = new Server(getId(), getUsername(), 3002);
    _heartbeat = new Heartbeat(getId(), getUsername(), "127.0.0.1", 3010);
    _manager = new ConnectionManager(getId(), getUsername(), "127.0.0.1", 3010);

    _server->start();
    _heartbeat->start();
    _manager->start();

    connect(_manager, SIGNAL(peersUpdated()), this, SLOT(updatePeerTable()));

    _setupPeerTable();

    this->show();
}

void MainWindow::_updatePeerTable(std::vector<Peer*> peers) {

    QTableWidgetItem *prototype = new QTableWidgetItem;
    prototype->setTextAlignment(Qt::AlignCenter);

    QTableWidget *peerTable = ui->peersTable;

    int numberOfPeers = peers.size();

    peerTable->setRowCount(numberOfPeers);

    for (int i = 0; i < numberOfPeers; i++) {

        QTableWidgetItem *newItem = prototype->clone();
        newItem->setText(QString::fromStdString(peers[i]->getUsername()));

        peerTable->setItem(i, 0, newItem);

        QIcon onlineIcon(":/status/build/online-dot-small.png");
        QIcon offlineIcon(":/status/build/offline-dot-small.png");

        QTableWidgetItem *icon = prototype->clone();

        if (_manager->isPeerOnline(peers[i]->getUserID())) {


            icon->setIcon(onlineIcon);
            icon->setToolTip("Online");

        } else {

            icon->setIcon(offlineIcon);
            icon->setToolTip("Offline");

        }

        peerTable->setItem(i, 1, icon);

    }
}

void MainWindow::_setupPeerTable() {

    QStringList tableHeaders;
    tableHeaders << "Username" << "Status";

    QTableWidget *peerTable = ui->peersTable;

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);

    peerTable->verticalHeader()->hide();
    peerTable->setItemPrototype(item);
    peerTable->setShowGrid(false);
    peerTable->setColumnCount(2);
    peerTable->setAlternatingRowColors(true);
    peerTable->setHorizontalHeaderLabels(tableHeaders);
    peerTable->horizontalHeader()->hide();
    peerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    peerTable->setEditTriggers(QAbstractItemView::EditTriggers(0));
    peerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    peerTable->setSelectionBehavior(QAbstractItemView::SelectRows);

}

std::vector<std::vector<std::string>> pendingRemoteFiles;

void MainWindow::_queryOnlinePeers() {

    PeerMap::iterator it;

    for (it = _onlinePeers.begin(); it != _onlinePeers.end(); ++it) {

        Client * remoteClient = new Client(_id, _username, it->second->getIP(), 3001);
        remoteClient->setCommand(std::string(1, (char)Commands::Query), [=](void *client){

            std::vector<std::string> pending = ((Client *) client)->getPendingFiles();
            pendingRemoteFiles.push_back(pending);

        });
    }

    for (size_t i = 0; i < pendingRemoteFiles.size(); i++) {

        for (size_t j = 0; j < pendingRemoteFiles[i].size(); j++) {

            qDebug() << "File-Name: " << QString::fromStdString(pendingRemoteFiles[i][j]);

        }

    }

}

void MainWindow::updatePeerTable() {


    _peers = _manager->getAllPeers();

    _onlinePeers = _manager->getPeers();

    _updatePeerTable(_peers);

    _queryOnlinePeers();
}

MainWindow *window;
std::string image;

void MainWindow::upload() {

    this->show();
    this->setEnabled(true);

    std::vector<int> indicies = _upload->selectedPeers();
    image = _upload->image();

    for (unsigned int i = 0; i < indicies.size(); ++i) {

        std::string username = _peers[indicies[i]]->getUsername();
        std::string userId = _peers[indicies[i]]->getUserID();

        if (_manager->isPeerOnline(userId)) {

            Client *client;

            if (_clients.count(username) != 0) {

                client = _clients[username];

            } else {

                client = new Client(_id, _username, _onlinePeers[userId]->getIP(), 3001);

            }

            client->start();

            client->setCommand(std::string(1, (char)Commands::EstablishConnection), [=](void *client) {

                ((Client *) client)->setCommand(std::string(1, (char)Commands::File), [=](void *client) {

                    qDebug() << "Sending File!";

                    ((Client *) client)->addArgument(image);

                    ((Client *) client)->setCommand(std::string(1, (char)Commands::SendFile), [=](void *client){

                        qDebug() << "file sent!";
                        ((Client *) client)->join();

                    });

                });
            });

        } else {

            _server->addFileRecepient(username, image);

        }

    }

}

void MainWindow::cancelUpload() {

}


std::string MainWindow::getUsername() const {
    return _username;
}

void MainWindow::setUsername(const std::string &username) {
    _username = username;
}


MainWindow::~MainWindow() {

    if (_server != NULL) {

        _server->join();

    }

    if (_manager != NULL) {

        _manager->join();

    }

    if (_heartbeat != NULL) {

        _heartbeat->join();

    }

    for (ClientMap::iterator it = _clients.begin(); it != _clients.end(); ++it) {

        it->second->join();

    }

    delete _server;
    delete _manager;
    delete _heartbeat;
    delete ui;

}

void MainWindow::on_uploadBtn_clicked() {

    this->setDisabled(true);
    _upload->setPeers(&_peers);
    _upload->setEnabled(true);
    _upload->show();

}
std::vector<std::string> MainWindow::pendingFiles() const {
    return _pendingFiles;
}

std::vector<Peer *> MainWindow::getPeers() const {
    return _peers;
}
