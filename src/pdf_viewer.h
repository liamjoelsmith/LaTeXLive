#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <QObject>
#include <poppler-qt5.h>
#include <QImage>
#include <QApplication>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QCloseEvent>
#include <QMainWindow>

class PdfViewer : public QMainWindow {
    Q_OBJECT

public:   
    PdfViewer(const QString &filename, QLabel *label, QWidget *parent = nullptr);

public slots:
    void requestReload();

private:
    QString filename;
    QString pdfFile;
    QLabel *label;
    QScrollArea *scrollArea;
    static const int width = 600;
    static const int height = 600;

    void reload();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // PDFVIEWER_H
