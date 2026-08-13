// pdfviewerwindow.cpp
#include "pdfviewerwindow.h"

#include <QPdfView>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

PdfViewerWindow::PdfViewerWindow(const QByteArray &pdfData, QWidget *parent)
    : QWidget(parent), pdfData_(pdfData) {
    setWindowTitle("Question Paper");
    resize(800, 900);

    view_ = new QPdfView(this);
    view_->setDocument(&document_);
    view_->setPageMode(QPdfView::PageMode::MultiPage);
    view_->setZoomMode(QPdfView::ZoomMode::FitToWidth);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(view_);

    connect(&document_, &QPdfDocument::statusChanged,
            this, &PdfViewerWindow::onDocumentStatusChanged);

    // Load the in-memory decrypted PDF bytes directly - no temp file is
    // ever written to disk on the client side. load() from a QIODevice
    // is asynchronous in Qt6; result arrives via statusChanged above.
    buffer_.setBuffer(&pdfData_);
    buffer_.open(QIODevice::ReadOnly);
    document_.load(&buffer_);
}

void PdfViewerWindow::onDocumentStatusChanged(QPdfDocument::Status status) {
    if (status == QPdfDocument::Status::Error) {
        QPdfDocument::Error err = document_.error();
        QMessageBox::critical(this, "PDF Load Error",
                               "Failed to load the decrypted PDF (error code: " +
                                   QString::number(static_cast<int>(err)) + ")");
    }
    // status == Ready means the PDF loaded successfully; QPdfView
    // updates itself automatically via its document() binding.
}
