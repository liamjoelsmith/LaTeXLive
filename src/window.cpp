#include <poppler-qt5.h>
#include <QImage>
#include <QApplication>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("PDF Viewer");

    QApplication app(argc, argv);

    // QCommandLineParser parser;
    // parser.addHelpOption();
    // parser.addVersionOption();
    // parser.addPositionalArgument("file", "The file to open.");
    // parser.process(app);

    // Load PDF document
    QString filename = "../minimal.pdf";
    Poppler::Document *document = Poppler::Document::load(filename);
    if (!document || document->isLocked()) {
        exit(1);
    }

    // Render first page
    Poppler::Page *pdfPage = document->page(0);
    if (pdfPage == 0) {
        exit(1);
    }

    // Window width
    int width = 600;
    int height = 600;

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
    double scaleFactor = static_cast<double>(width) / pdfWidthPx;

    // Scale image to match the window width
    QImage scaledImage = image.scaled(width, static_cast<int>(pdfHeightPx * scaleFactor),
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);

    QLabel *label = new QLabel;
    label->setPixmap(QPixmap::fromImage(scaledImage));
    label->resize(width, static_cast<int>(pdfHeightPx * scaleFactor));

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(label);
    scrollArea->resize(width, height);
    scrollArea->show();

    return app.exec();
}