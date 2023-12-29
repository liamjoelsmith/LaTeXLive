
#include "pdf_viewer.h"
#include <memory>

PdfViewer::PdfViewer(const QString &filename, QLabel *label, QWidget *parent) 
    : QMainWindow(parent), filename(filename), label(label)
{
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(label);
    scrollArea->resize(PdfViewer::width, PdfViewer::height);
    scrollArea->show();

    PdfViewer::pdfFile = filename + ".pdf";

    reload();
}

void PdfViewer::requestReload()
{
    reload();
}

void PdfViewer::reload()
{
    // Run bash script to compile the .pdf
    QString command = "./make_latex.sh " + PdfViewer::filename;
    std::string commandStd = command.toStdString();
    int error = std::system(commandStd.c_str());
    if (error != 0) {
        std::cerr << "System command failed: " << error << std::endl;
    }

    // Check if the file exists
    std::ifstream file("output/" + PdfViewer::pdfFile.toStdString());
    if (!file) {
        std::cerr << "File does not exist: " << PdfViewer::pdfFile.toStdString() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::unique_ptr<Poppler::Document> document(Poppler::Document::load("output/" + PdfViewer::pdfFile));
    if (!document || document->isLocked()) {
        exit(1);
    }

    std::unique_ptr<Poppler::Page> pdfPage(document->page(0));
    if (!pdfPage) {
        exit(1);
    }

    // Original PDF page size in points
    QSizeF pageSize = pdfPage->pageSizeF();

    // Convert page size from points to pixels
    int dpi = 300;
    int pdfWidthPx = static_cast<int>(pageSize.width() * dpi / 72);
    int pdfHeightPx = static_cast<int>(pageSize.height() * dpi / 72);

    // Render the PDF page
    QImage image = pdfPage->renderToImage(dpi, dpi, 0, 0, pdfWidthPx, pdfHeightPx);
    if (image.isNull()) {
        exit(1);
    }

    // Scale factor
    double scaleFactor = static_cast<double>(PdfViewer::width) / pdfWidthPx;

    // Scale image to match the window width
    QImage scaledImage = image.scaled(PdfViewer::width, static_cast<int>(pdfHeightPx * scaleFactor),
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);

    label->setPixmap(QPixmap::fromImage(scaledImage));
    label->resize(PdfViewer::width, static_cast<int>(pdfHeightPx * scaleFactor));

}

void PdfViewer::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    QApplication::quit();
}

