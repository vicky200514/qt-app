// loginwindow.h
#pragma once
#include <QWidget>
#include <QThread>
#include "networkworker.h"

class QLineEdit;
class QPushButton;
class QLabel;

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow() override;

signals:
    // Connected to NetworkWorker::connectAndFetch (queued automatically,
    // since the worker lives on a different thread).
    void requestLogin(const QString &host, const QString &port,
                       const QString &username, const QString &password,
                       const QString &remoteFilename,
                       const QString &caCertPath, const QString &aesKeyPath);

private slots:
    void onLoginClicked();
    void onLoginFailed(const QString &reason);
    void onFileReceived(const QByteArray &pdfData);
    void onErrorOccurred(const QString &message);

private:
    QLineEdit *usernameEdit_;
    QLineEdit *passwordEdit_;
    QPushButton *loginButton_;
    QLabel *statusLabel_;

    QThread workerThread_;
    NetworkWorker *worker_;

    // --- Connection settings ---
    // Adjust these to match your server deployment, or wire them up to
    // additional UI fields / a config file if they need to be user-editable.
    QString serverHost_   = "13.218.232.29";
    QString serverPort_   = "5060";
    QString remoteFile_   = "question_paper.pdf";
    QString caCertPath_   = "server.crt";
    QString aesKeyPath_   = "aes.key";
};
