#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
using namespace std;

counting_semaphore<3> semaphore(3); // Allow up to 3 threads

void task(int id, int waitTime) {
    if (semaphore.try_acquire_for(chrono::seconds(waitTime))) { // Try to acquire a slot within waitTime
        cout << "Thread " << id << " is working.\n";
        this_thread::sleep_for(chrono::seconds(2)); // Simulate work
        cout << "Thread " << id << " is done.\n";
        semaphore.release(); // Release the slot
    } else {
        // Timeout: unable to acquire a slot
        cout << "Thread " << id << " timed out after waiting " << waitTime << " seconds.\n";
    }
}

int main() {
    // Threads with different wait times
    thread t1(task, 1, 5); // Thread 1 will wait up to 5 seconds for a slot
    thread t2(task, 2, 5); // Thread 2 will wait up to 5 seconds for a slot
    thread t3(task, 3, 5); // Thread 3 will wait up to 5 seconds for a slot
    thread t4(task, 4, 2); // Thread 4 will wait up to 2 seconds for a slot

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
