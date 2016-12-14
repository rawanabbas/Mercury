#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), _manager(NULL), _server(NULL), _heartbeat(NULL), flag(true) {



    ui->setupUi(this);

    _login = new Login(this);
    _login->setWindowTitle("Login");
    _login->show();

    _upload = new Upload(this);
    _upload->setWindowTitle("Upload Image");

    _viewer = new ImageViewer(this);
    _viewer->setWindowTitle("Decrypted Image");

    connect(_login, SIGNAL(authenticated()), this, SLOT(display()));
    connect(_upload, SIGNAL(ready()), this, SLOT(upload()));
    connect(_upload, SIGNAL(cancelled()), this, SLOT(cancelUpload()));
    QTimer *onlineChecker = new QTimer(this);
    onlineChecker->setInterval(10000);
    onlineChecker->setSingleShot(false);
    connect(onlineChecker, SIGNAL(timeout()), this, SLOT(checkPendingFiles()));
    onlineChecker->start();

    refreshFiles();
    QTimer *myFilesChecker = new QTimer(this);
    myFilesChecker->setInterval(4000);
    myFilesChecker->setSingleShot(false);
    connect(myFilesChecker, SIGNAL(timeout()), this, SLOT(refreshFiles()));
    myFilesChecker->start();


    connect(ui->localImages, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openImage(QModelIndex)));
}



std::string MainWindow::getId() const {
    return _id;
}

void MainWindow::setId(const std::string &id) {
    _id = id;
}


void MainWindow::display() {
    std::cout << "USERNAME: " << _login->getUsername() << std::endl;
    setUsername(_login->getUsername());
    setId(_login->getId());
    _login->close();

    _server = new Server(getId(), getUsername(), 3001);
    //_heartbeat = new Heartbeat(getId(), getUsername(), "10.7.57.133", 3010);
    //_manager = new ConnectionManager(getId(), getUsername(), "10.7.57.133", 3010);
    _heartbeat = new Heartbeat(getId(), getUsername(), "10.7.57.35", 3010);
    _manager = new ConnectionManager(getId(), getUsername(), "10.7.57.35", 3010);
    connect(_manager, SIGNAL(connected()), this, SLOT(heartbeatConnected()));
    connect(_manager, SIGNAL(disconnected()), this, SLOT(heartbeatDisconnected()));


    _server->start();
    _heartbeat->start();
    _manager->start();

    connect(_manager, SIGNAL(peersUpdated()), this, SLOT(updatePeerTable()));

    _setupPeerTable();

    this->show();
}

void MainWindow::_updatePeerTable(std::vector<Peer*> peers) {

    ui->peersTable->clear();
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

    qDebug() << "Querying online peerss..";

    std::vector<Client*> remoteClients;

    PeerMap::iterator it;

    qDebug() << _onlinePeers.size();

    for (it = _onlinePeers.begin(); it != _onlinePeers.end(); ++it) {

        qDebug() << "------------------------------------------";
        qDebug() << QString::fromStdString(it->second->getUsername());
        qDebug() << "------------------------------------------";

        Client * remoteClient = new Client(_id, _username, it->second->getIP(), 3001);
        remoteClient->start();

        remoteClient->setCommand(std::string(1, (char)Commands::EstablishConnection), [=](void *client) {


            ((Client *) client)->setCommand(std::string(1, (char)Commands::Query), [=](void *client) {

                qDebug() << "FILESSSSS!!!";

                std::vector<std::string> pending = ((Client *) client)->getPendingFiles();
                pendingRemoteFiles.push_back(pending);

            });
        });
    }


    //    QTableWidget *remoteImages = ui->remoteImages;
    //    int row = 0;

    //    for (size_t i = 0; i < pendingRemoteFiles.size(); ++i) {

    //        for (size_t j = 0; j < pendingRemoteFiles[i].size(); ++j) {

    //            remoteImages->setRowCount(row + 1);
    //            remoteImages->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(pendingRemoteFiles[i][j])));
    //            row++;

    //        }

    //    }


}

void MainWindow::updatePeerTable() {


    _peers = _manager->getAllPeers();

    _onlinePeers = _manager->getPeers();

    _updatePeerTable(_peers);

    if (flag) {

        flag = false;
        _queryOnlinePeers();

    }
}

MainWindow *window;
std::string image;
int views;

void MainWindow::_uploadImageToClient(std::string userId) {
    Client *client;
    std::string username = _onlinePeers[userId]->getUsername();
    if (_clients.count(username) != 0) {

        client = _clients[username];

    } else {
        client = new Client(_id, _username, _onlinePeers[userId]->getIP(), 3001);
        //client = new Client(_id, _username, "10.", 3001);

    }

    std::string newImage = image + "_steg.jpg";
    if (Steganography::embedImage("cover.jpg", image, std::to_string(views), newImage, "123456")) {

        image = newImage;

    }

    client->start();

    client->setCommand(std::string(1, (char)Commands::EstablishConnection), [=](void *client) {

        ((Client *) client)->setCommand(std::string(1, (char)Commands::File), [=](void *client) {


            std::cout << "ImagePath: " << image << std::endl;

            qDebug() << "Sending File!";

            //                    Steganography::embedImage("cover.jpg", image, std::to_string(views), image, ((Client *) client)->getOwnerId());

            ((Client *) client)->addArgument(image);

            ((Client *) client)->setCommand(std::string(1, (char)Commands::SendFile), [=](void *client) {

                qDebug() << "file sent!";
                ((Client *) client)->join();

            });

        });
    });
}

void MainWindow::upload() {

    this->show();
    this->setEnabled(true);

    std::vector<int> indicies = _upload->selectedPeers();
    image = _upload->image();
    views = _upload->views();


    for (unsigned int i = 0; i < indicies.size(); ++i) {

        std::string userId = _peers[indicies[i]]->getUserID();

        if (_manager->isPeerOnline(userId)) {
            qDebug() << "Sending..";
            _uploadImageToClient(userId);
        } else {
            qDebug() << "User is offline adding it to the cache!";
            _server->addFileRecepient(userId, image);

        }

    }

}

void MainWindow::cancelUpload() {

}

void MainWindow::heartbeatConnected() {
    this->statusBar()->showMessage("Connected to peering server.");
}

void MainWindow::heartbeatDisconnected() {
    this->statusBar()->showMessage("Disconnected from peering server.");
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


void MainWindow::checkPendingFiles() {
    FilesMap &pendingFiles = _server->getPendingFiles();
    if(!pendingFiles.size())
        return;
    for (auto it = pendingFiles.begin(); it != pendingFiles.end(); ++it) {
        std::string userId = it->first;
        if (_manager->isPeerOnline(userId)) {
            std::cout << "Sending pending to " << _onlinePeers[userId]->getUsername();
            for (int i = 0; i < it->second.size(); i++) {
                std::cout << "Sending " << it->second[i] << std::endl;
                image = it->second[i];
                _uploadImageToClient(userId);
                it->second.erase(it->second.begin() + i--);
            }
        }
    }
}

void MainWindow::refreshFiles() {
    QDir currentPath = QDir::currentPath();
    QStringList files = currentPath.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    ui->localImages->clear();

    for (int i = 0; i < files.size(); i++) {
        QString directorPath = currentPath.absolutePath() + QDir::separator() + files.at(i);
        QDir userDir(directorPath);
        QStringList images = userDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        for (int i = 0; i < images.size(); i++) {
            QString imagePath = userDir.absolutePath() + QDir::separator() + images.at(i);
            ui->localImages->addItem(imagePath);
        }
    }
}

void MainWindow::openImage(QModelIndex index) {
    if (!index.isValid())
        return;
    QString imagePath = ui->localImages->item(index.row())->text();
    QString tempPath = QFileInfo(imagePath).absoluteDir().absolutePath() + QDir::separator() + "temp.jpg";
    if (Steganography::extractImage(imagePath.toStdString(), tempPath.toStdString(), "123456")) {
        _viewer->setImage(tempPath);
        _viewer->show();
        unlink(tempPath.toStdString().c_str());
        Steganography::decrementViews(imagePath.toStdString(), "123456");
    }
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
