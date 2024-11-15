#pragma once
#include <string>
#include <mutex>
#include <condition_variable>
#include <optional>

class ThreadSafeStringChannel {
public:
    void send(const std::string& value) {
        std::lock_guard<std::mutex> lock(mtx);
        data = value;
        dataReady = true;
        cv.notify_one();
    }

    std::optional<std::string> receive() {
        std::lock_guard<std::mutex> lock(mtx);
        if (dataReady) {
            dataReady = false;
            return data;
        } else {
            return std::nullopt;
        }
    }

private:
    std::string data;
    bool dataReady = false;
    std::mutex mtx;
    std::condition_variable cv;
};
