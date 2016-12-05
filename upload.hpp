#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include "peer.hpp"

#include <QDialog>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
        class upload;
    }


struct Package {

    std::vector<int> indicies;
    std::string image;
};

class Upload : public QDialog
{
    Q_OBJECT

public:
    explicit Upload(QWidget *parent);
    ~Upload();

    void setPeers(std::vector<Peer*> *peers);
    std::vector<int> selectedPeers() const;
    std::string image() const;


private slots:

    void on_browseBtn_clicked();

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

signals:

    void cancelled();
    void ready();

private:

    Ui::upload *ui;

    std::vector<Peer*> *_peers;

    std::vector<int> _selectedPeers;
    std::string _image;

};

#endif // UPLOAD_HPP
