#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <iostream>

// Constants
int constexpr delay_time = 1;

enum class ErrorCode {
    USAGE_ERROR = 1,
};

int main(int argc, char** argv)
{
    // Check usage error
    if (argc < 2) {
        std::cerr << "Usage: track <file1> [<file2> ...]" << std::endl;
        exit(static_cast<int>(ErrorCode::USAGE_ERROR));
    }

    // Create file descriptor for accessing inotify API
    int fd = inotify_init1(IN_NONBLOCK);
    if (fd == -1) {
        std::cerr << "Error: inotify_init1" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allocate memory for watch descriptors
    int *wd = (int*)calloc(argc, sizeof(int));
    if (wd == NULL) {
        std::cerr << "Error: unable to allocate memory fr watch descriptors";
        exit(EXIT_FAILURE);
    }

    // Mark directories for event "file was saved"
    for (int i = 1; i < argc; i++) {
        wd[i] = inotify_add_watch(fd, argv[i], IN_MODIFY);
        if (wd[i] == -1) {
            std::cerr << "Cannot watch '" << argv[i] << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    const size_t BUF_LEN = 496;
    char buf[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));

    // Loop to read events
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
            }
        }

        // Add delay before waiting for another event
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(delay_time));
    }

    for (int i; i < argc; i++) {
        inotify_rm_watch(fd, wd[i]);
    }   
    close(fd);

    return 0;
}





