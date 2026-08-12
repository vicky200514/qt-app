// loginwindow.cpp
#include "loginwindow.h"
#include "pdfviewerwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Question Paper Access - Login");
    resize(360, 180);

    usernameEdit_ = new QLineEdit(this);
    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setEchoMode(QLineEdit::Password);

    loginButton_ = new QPushButton("Login", this);
    statusLabel_ = new QLabel(this);
    statusLabel_->setStyleSheet("color: red;");
    statusLabel_->setWordWrap(true);

    auto *form = new QFormLayout;
    form->addRow("Username:", usernameEdit_);
    form->addRow("Password:", passwordEdit_);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(loginButton_);
    layout->addWidget(statusLabel_);

    connect(loginButton_, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    // Pressing Enter in the password field also triggers login.
    connect(passwordEdit_, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);

    // --- Set up the background network worker/thread ---
    worker_ = new NetworkWorker();
    worker_->moveToThread(&workerThread_);

    connect(&workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &LoginWindow::requestLogin, worker_, &NetworkWorker::connectAndFetch);
    connect(worker_, &NetworkWorker::loginFailed, this, &LoginWindow::onLoginFailed);
    connect(worker_, &NetworkWorker::fileReceived, this, &LoginWindow::onFileReceived);
    connect(worker_, &NetworkWorker::errorOccurred, this, &LoginWindow::onErrorOccurred);

    workerThread_.start();
}

LoginWindow::~LoginWindow() {
    workerThread_.quit();
    workerThread_.wait();
}

void LoginWindow::onLoginClicked() {
    const QString username = usernameEdit_->text().trimmed();
    const QString password = passwordEdit_->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel_->setText("Please enter both username and password.");
        return;
    }

    loginButton_->setEnabled(false);
    statusLabel_->setStyleSheet("color: gray;");
    statusLabel_->setText("Connecting...");

    emit requestLogin(serverHost_, serverPort_, username, password,
                       remoteFile_, caCertPath_, aesKeyPath_);
}

void LoginWindow::onLoginFailed(const QString &reason) {
    loginButton_->setEnabled(true);
    statusLabel_->setStyleSheet("color: red;");
    statusLabel_->setText("Login failed: " + reason);
    passwordEdit_->clear();
}

void LoginWindow::onErrorOccurred(const QString &message) {
    loginButton_->setEnabled(true);
    statusLabel_->setStyleSheet("color: red;");
    statusLabel_->setText(message);
}

void LoginWindow::onFileReceived(const QByteArray &pdfData) {
    loginButton_->setEnabled(true);
    statusLabel_->setText("");

    // Open the PDF viewer with the decrypted data (kept in memory only -
    // never written to disk on the client side).
    auto *viewer = new PdfViewerWindow(pdfData);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();

    this->close();
}
