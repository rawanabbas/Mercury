#ifndef FORMLOGIN_HPP
#define FORMLOGIN_HPP

#include <QDialog>

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

class FormLogin : public QDialog {
    Q_OBJECT

private slots:

    void _OnQuit();
    void _OnLogin();

private:
    void reject();

    QLabel* _userLabel;
    QLabel* _passLabel;

    QLineEdit* _userLineEdit;
    QLineEdit* _passLineEdit;

    QPushButton* _loginButton;
    QPushButton* _quitButton;

public:
    FormLogin(QWidget* parent = 0);
    virtual ~FormLogin();

};

#endif // FORMLOGIN_HPP
