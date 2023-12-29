#include "file_watcher.h"

FileWatcher::FileWatcher()
{
    // File descriptor for accessing inotify API
    fd = inotify_init1(IN_NONBLOCK);
    if (fd == -1) {
        std::cerr << "Error: inotify_init1" << std::endl;
        exit(EXIT_FAILURE);
    }
}

FileWatcher::~FileWatcher()
{
    for (int w : wd) {
        inotify_rm_watch(fd, w);
    }   
    close(fd);
}

void FileWatcher::addWatch(const std::string& path)
{
    // Add .pdf extension
    std::string texFile = path + ".tex";

    // Check if the file exists
    std::ifstream file(texFile);
    if (!file) {
        std::cerr << "File does not exist: " << texFile << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Add inotify watch
    int watchDescriptor = inotify_add_watch(fd, texFile.c_str(), IN_MODIFY);
    if (watchDescriptor == -1) {
        throw std::runtime_error("Cannot watch " + texFile + ": " + strerror(errno) + "\n");
    }
    wd.push_back(watchDescriptor);
}

void FileWatcher::startWatching(std::function<void()> func)
{
    const size_t BUF_LEN = 4096;
    char buf[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    while (true) {
        ssize_t numRead = read(fd, buf, BUF_LEN);
        if (numRead == -1) {
            if (errno != EAGAIN) {
                std::cerr << "Read error: " << std::endl;
                break;
            }
        } else if (numRead > 0) {
            struct inotify_event *event = (struct inotify_event *) buf;
            if (event->mask & IN_MODIFY) {
                std::cout << "File has been modified." << std::endl;
                func();
            }
        }

        // Add delay before waiting for another change
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(delay_time));
    }
}





