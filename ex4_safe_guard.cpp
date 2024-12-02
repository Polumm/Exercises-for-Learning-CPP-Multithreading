#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx; // Mutex for synchronizing shared resource

void printNumbers(int id) {
    for (int i = 1; i <= 5; ++i) {
        lock_guard<mutex> lock(mtx); // Lock the mutex
        cout << "Thread " << id << ": " << i << endl;
        // Mutex is automatically unlocked at the end of this block
    }
}

int main() {
    thread t1(printNumbers, 1);
    thread t2(printNumbers, 2);

    t1.join();
    t2.join();

    return 0;
}
