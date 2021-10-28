#pragma once

#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class ObjectPool {
 public:
  using DeleterType = std::function<void(T*)>;

  ObjectPool() {}
  ~ObjectPool() {
    for (int i = 0; i < pool_.size(); i++) {
      pool_[i] = nullptr;
    }
  }

  std::unique_ptr<T, DeleterType> Get(int timeout_ms = 10000) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto rel_time = std::chrono::milliseconds(timeout_ms);
    bool bValid = condition_variable_.wait_for(
        lock, rel_time, [this] { return !pool_.empty(); });

    if (!bValid) {
      std::unique_ptr<T, DeleterType> empty_ptr(nullptr,
                                                [this](T* t) { int tmp = 1; });
      return std::move(empty_ptr);
    }

    std::unique_ptr<T, DeleterType> ptr(
        pool_.back().release(), [this](T* t) { Push(std::unique_ptr<T>(t)); });
    pool_.pop_back();
    return std::move(ptr);
  }
  int Push(std::unique_ptr<T> t) {
    std::lock_guard<std::mutex> lock(mutex_);
    pool_.push_back(std::move(t));
    condition_variable_.notify_one();
    return 0;
  }

 private:
  std::vector<std::unique_ptr<T>> pool_;
  std::mutex mutex_;
  std::condition_variable condition_variable_;
};
