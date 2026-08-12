// pdfviewerwindow.h
#pragma once
#include <QWidget>
#include <QByteArray>
#include <QBuffer>
#include <QPdfDocument>

class QPdfView;

class PdfViewerWindow : public QWidget {
    Q_OBJECT
public:
    // pdfData is the already-decrypted PDF bytes, kept in memory only.
    explicit PdfViewerWindow(const QByteArray &pdfData, QWidget *parent = nullptr);

private slots:
    void onDocumentStatusChanged(QPdfDocument::Status status);

private:
    QByteArray pdfData_;   // owns the bytes for the lifetime of this window
    QBuffer buffer_;       // QIODevice wrapper QPdfDocument reads from
    QPdfDocument document_;
    QPdfView *view_;
};
