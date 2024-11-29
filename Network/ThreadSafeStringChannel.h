#pragma once
#include <condition_variable>
#include <mutex>
#include <optional>
#include <vector>

class ThreadSafeByteChannel {
 public:
  void send(const unsigned char* byteArray, size_t size) {
    std::lock_guard<std::mutex> lock(mtx);
    data.assign(byteArray, byteArray + size);  // Copy data to the vector
    dataReady = true;
    cv.notify_one();
  }

  std::optional<std::vector<unsigned char>> receive() {
    std::lock_guard<std::mutex> lock(mtx);
    if (dataReady) {
      dataReady = false;
      return data;
    } else {
      return std::nullopt;
    }
  }

 private:
  std::vector<unsigned char> data;
  bool dataReady = false;
  std::mutex mtx;
  std::condition_variable cv;
};
