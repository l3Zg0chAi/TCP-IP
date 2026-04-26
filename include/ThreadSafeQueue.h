#ifndef __THREAD_SAFE_QUEUE_H__
#define __THREAD_SAFE_QUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSfaeQueue&) = delete;

    ThreadSafeQueue& oprerator=(const ThreadSafeQueue& other){
        if (this == &other) return;
        std::lock_guard<std::mutex> lock_this(_mutex);
        std::lock_guard<std::mutex> lock_other(other._mutex);
        _queue = otehr.queue;
        return *this;
    }

    void push(T value){
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.size() > 50){
                DEB
            }
            _queue.push(std::move(value));  
        }
        _cv.notify_one();
    }

    void pop(){
        std::lock_guard<std::mutex> lock(_mutex);
        if (_queue.empty()){
            return;
        }
        _queue.pop();
    }

    bool try_pop(T& value){
        std::lock_guard<std::mutex> lock(_mutex);
        if (_queue.empty()){
            return false;
        }
        value = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    bool wait_and_pop(T& value){
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]{!_queue.empty() || _stopFlag});
        if (_queue.empty() || _stopFlag){
            return false;
        }

        value = std::move(_queue.front());
        _queue.pop();
        return true;
    }
    
    void set_stopFlag(bool valueFlag){
        std::lock_guard<std::mutex> lock(_mutex);
        _stopFlag = valueFlag;
    }

private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::queue<T> _queue;
    bool _stopFlag = false;
}

#endif // __THREAD_SAFE_QUEUE_H__