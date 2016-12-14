#include "login.hpp"
#include "ui_login.h"

Login *self;

Login::Login(QWidget *parent) :
    QDialog(parent), ui(new Ui::Login) {


    ui->setupUi(this);

    self = this;

    _client = new Client("10.7.57.35", 3010);
    _client->start();

}

Login::~Login() {
    delete ui;
    _client->join();
    delete _client;
    std::cout << "Deleted" << std::endl;
}

void Login::on_loginBtn_clicked() {

    std::cout << "USERNAME: " << ui->usrnameInput->text().toStdString() << std::endl;
    _client->addArgument(ui->usrnameInput->text().toStdString());
    _client->addArgument(ui->pwdInput->text().toStdString());


    _client->setCommand(std::string(1, (char)Commands::SignIn), [=](void* client)  {

        if (((Client*) client) -> getStatus() == ClientStatus::Authenticated || ((Client *)client)->isAuthenticated()) {

            self->setUsername(((Client *) client)->getUsername());
            self->setId(((Client *) client)->getOwnerId());

            qDebug() << "Authenticated!";

            self->emitAuthenticated();

        } else {

            self->getUi()->usrnameInput->setStyleSheet("border: 1px solid red");
            self->getUi()->pwdInput->setStyleSheet("border: 1px solid red");

            qDebug() << "Unauthorized!";

        }

    });


}

void Login::on_registerBtn_clicked() {

    _client->addArgument(ui->usrnameInput->text().toStdString());
    _client->addArgument(ui->pwdInput->text().toStdString());

    _client->setCommand(std::string(1, (char)Commands::Register), [&](void* client)  {

        if (((Client*) client) -> getStatus() == ClientStatus::Authenticated) {

            self->setUsername(((Client *) client)->getUsername());
            self->setId(((Client *) client)->getOwnerId());

            qDebug() << "Registered!";

            self->emitAuthenticated();

        } else {

            self->getUi()->usrnameInput->setStyleSheet("border: 1px solid red;");
            self->getUi()->pwdInput->setStyleSheet("border: 1px solid red; ");

            qDebug() << "Unauthorized!";

        }
    });
}
Ui::Login *Login::getUi() const {
    return ui;
}

std::string Login::getId() const {
    return _id;
}

void Login::setId(const std::string &id) {
    _id = id;
}

void Login::emitAuthenticated() {
    emit authenticated();
}

std::string Login::getUsername() const {
    return _username;
}

void Login::setUsername(const std::string &username) {
    _username = username;
}

