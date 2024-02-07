#include <list>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sys/stat.h>

#define NEW_LOG 0
#define APPEND_LOG 1
#define MAX_LEN 524288

typedef int S_MODE;

class FLog {
private:
    std::string fpath, cache;
    char instance_address[128];
    bool auto_s = false;

    std::string getTimestamp() {
        time_t now = time(0);
        char buf[128];
        strftime(buf, sizeof(buf), "%Y-%m-%d | %X", localtime(&now));
        return std::string(buf);
    }

public:
    FLog(std::string path) : fpath(std::move(path)) {
        sprintf(instance_address, "%p", this);
        cache.append("/--- New FLog Instance / MemPtr : ").append(instance_address).append(" ---/\n/--- Path : ").append(fpath).append(" ---/\n/--- Creation Date : ").append(getTimestamp()).append(" ---/\n\n");
    }

    void append(const std::string& str) {
        cache += "\n" + getTimestamp() + " : " + str + "\n";
        if (auto_s)
            save(NEW_LOG);
    }

    void append(int num) {
        cache += "\n" + getTimestamp() + " : " + std::to_string(num) + "\n";
        if (auto_s)
            save(NEW_LOG);
    }

    template<typename... Args>
    void append_arg(const std::string& str, Args... args) {
        char memr[MAX_LEN];
        sprintf(memr, str.c_str(), args...);
        cache += "\n" + getTimestamp() + " : " + std::string(memr) + "\n";
        if (auto_s)
            save(NEW_LOG);
    }

    template<typename... Args>
    void append_arg_mode(const std::string& str, int modee, Args... args) {
        char memr[MAX_LEN];
        sprintf(memr, str.c_str(), args...);
        cache += "\n" + getTimestamp() + " : " + std::string(memr) + "\n";
        if (auto_s)
            save(modee);
    }

    std::string get_address_str() const {
        return std::string(instance_address);
    }

    uintptr_t get_address_ptr() const {
        return reinterpret_cast<uintptr_t>(this);
    }

    void autosave(bool state) {
        auto_s = state;
    }

    void clear() {
        cache.clear();
    }

    void erase() {
        if (std::ifstream(fpath.c_str())) {
            remove(fpath.c_str());
        }
    }

    void save(S_MODE mode) {
        std::ofstream out(fpath, std::ios::out | std::ios::app);
        if (out.is_open()) {
            if (mode == NEW_LOG)
                out << cache;
            else if (mode == APPEND_LOG)
                out << "\n" << cache;
        } else {
            std::cerr << "Failed to open file for saving logs!" << std::endl;
        }
    }
};
