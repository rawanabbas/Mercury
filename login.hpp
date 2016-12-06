#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <QDialog>
#include <QDebug>

#include "client.hpp"

namespace Ui {
        class Login;
    }

class Login : public QDialog {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);

    ~Login();

    std::string getUsername() const;
    void setUsername(const std::string &username);

    std::string getId() const;
    void setId(const std::string &id);

    void emitAuthenticated();

    Ui::Login *getUi() const;

private slots:

    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

signals:

    void authenticated();

private:

    Client *_client;

    std::string _username;
    std::string _id;

    Ui::Login *ui;
};

#endif // LOGIN_HPP
