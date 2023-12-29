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
#include <vector>
#include <cstring>
#include <fstream>
#include <functional>

// Constants
int constexpr delay_time = 1;

// Error code/s
enum class ErrorCode {
    USAGE_ERROR = 1,
};

class FileWatcher {
public:
    FileWatcher();
    ~FileWatcher();
    void addWatch(const std::string& path);
    void startWatching(std::function<void()> func);

private:
    int fd;
    std::vector<int> wd;
};




