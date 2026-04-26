#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

std::atomic<int> globalCounter = 0;
void task1() {
    for (int i = 0; i < 10000; ++i) {
        globalCounter = i;
    }
}

void task2() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Task 2: " << i << std::endl;
    }
}

int main() {
    std::thread t1(task1);
    std::thread t2(task1);

    t1.join();
    t2.join();

    cout << globalCounter << endl;

    while(true){}
    return 0;
}