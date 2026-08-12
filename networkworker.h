// networkworker.h
// Runs the TLS + AES-256-GCM + username/password AUTH client logic
// (same protocol as client_secure_pdf.cpp) on a background thread, so
// the GUI never blocks while waiting on the network. Communicates back
// to the GUI thread exclusively via Qt signals (thread-safe by default,
// since NetworkWorker lives on a different QThread than the GUI).
#pragma once
#include <QObject>
#include <QByteArray>
#include <QString>

class NetworkWorker : public QObject {
    Q_OBJECT
public:
    explicit NetworkWorker(QObject *parent = nullptr);

public slots:
    // All of connect + TLS handshake + AUTH + GET + AES-decrypt happens
    // here, synchronously, on whichever thread this slot runs on (the
    // background QThread it's been moved to - see main.cpp).
    void connectAndFetch(const QString &host, const QString &port,
                          const QString &username, const QString &password,
                          const QString &remoteFilename,
                          const QString &caCertPath, const QString &aesKeyPath);

signals:
    void loginFailed(const QString &reason);
    void fileReceived(const QByteArray &pdfData);
    void errorOccurred(const QString &message);
};
