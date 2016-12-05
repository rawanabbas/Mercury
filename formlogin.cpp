#include "formlogin.hpp"

FormLogin::FormLogin(QWidget* parent)
    : QDialog(parent) {
    setFixedSize(300, 120);
    setWindowTitle("Mercury Login");
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    _userLabel = new QLabel("Username:");
    _passLabel = new QLabel("Password:");
    _userLineEdit = new QLineEdit();
    _passLineEdit = new QLineEdit();
    _passLineEdit->setEchoMode(QLineEdit::Password);
    _loginButton = new QPushButton("Login");
    _quitButton = new QPushButton("Quit");

    QVBoxLayout* vbox = new QVBoxLayout(this);
    QHBoxLayout* hbox1 = new QHBoxLayout();
    QHBoxLayout* hbox2 = new QHBoxLayout();
    QHBoxLayout* hbox3 = new QHBoxLayout();

    hbox1->addWidget(_userLabel, 1);
    hbox1->addWidget(_userLineEdit, 2);
    hbox2->addWidget(_passLabel, 1);
    hbox2->addWidget(_passLineEdit, 2);
    hbox3->addWidget(_loginButton, 1, Qt::AlignRight);
    hbox3->addWidget(_quitButton, 0, Qt::AlignRight);

    vbox->addSpacing(1);
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addLayout(hbox3);

    connect(_quitButton, SIGNAL(clicked()), this, SLOT(_OnQuit()));
    connect(_loginButton, SIGNAL(clicked()), this, SLOT(_OnLogin()));
}

void FormLogin::reject() {

    _OnQuit();
}

void FormLogin::_OnQuit() {
    this->close();
    parentWidget()->close();
}

void FormLogin::_OnLogin() {
    QString username = _userLineEdit->text();
    QString password = _passLineEdit->text();

    // Checking if username or password is empty
    if (username.isEmpty() || password.isEmpty())
        QMessageBox::information(this, tr("Peringatan!"), "Username atau password tidak boleh kosong");
    else
        this->destroy();
}

FormLogin::~FormLogin() {

}
