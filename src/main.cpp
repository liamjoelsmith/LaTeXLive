#include "file_watcher.cpp"
#include "pdf_viewer.cpp"
#include <thread>

void watch_files(PdfViewer* viewer, int argc, char** argv);

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("PDF Viewer");
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    QString filename = argv[1];
    QLabel label;
    PdfViewer viewer(filename, &label);
    label.show();

    std::thread watcherThread(watch_files, &viewer, argc, argv);

    int result = app.exec();

    if (watcherThread.joinable()) {
        watcherThread.join();
    }

    return result;
}

void watch_files(PdfViewer* viewer, int argc, char** argv) {
    FileWatcher fileWatcher;
    for (int i = 1; i < argc; ++i) {
        fileWatcher.addWatch(argv[i]);
    }

    fileWatcher.startWatching([viewer]() {
        QMetaObject::invokeMethod(viewer, "requestReload", Qt::QueuedConnection);
    });
}