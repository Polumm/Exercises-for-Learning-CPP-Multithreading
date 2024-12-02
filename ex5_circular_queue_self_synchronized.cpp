#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <mutex>

using namespace std;

class CircularQueue {
private:
    vector<int> queue;
    size_t maxSize;
    size_t front = 0, rear = 0, count = 0;
    mutex mtx;

public:
    explicit CircularQueue(size_t size) : maxSize(size) {
        queue.resize(size);
    }

    bool enqueue(int item) {
        lock_guard<mutex> lock(mtx);
        if (count == maxSize) return false;
        queue[rear] = item;
        rear = (rear + 1) % maxSize;
        ++count;
        return true;
    }

    bool dequeue(int& item) {
        lock_guard<mutex> lock(mtx);
        if (count == 0) return false;
        item = queue[front];
        front = (front + 1) % maxSize;
        --count;
        return true;
    }

    bool isEmpty() {
        lock_guard<mutex> lock(mtx);
        return count == 0;
    }
};

void producer(CircularQueue& q, int producerId, int numItems, atomic<int>& producedCount) {
    for (int i = 0; i < numItems; ++i) {
        int item = rand() % 100; // Random number between 0-99
        while (!q.enqueue(item)) {
            this_thread::sleep_for(chrono::milliseconds(10)); // Retry after 10ms if queue is full
        }
        cout << "Producer " << producerId << " enqueued: " << item << endl;
        ++producedCount;
    }
}

void consumer(CircularQueue& q, int consumerId, atomic<int>& consumedCount) {
    while (true) {
        int item;
        if (q.dequeue(item)) {
            cout << "Consumer " << consumerId << " dequeued: " << item << endl;
            ++consumedCount;
        } else {
            // Exit loop if queue is empty and all producers are done
            if (consumedCount == 20) break; // Match total number of items produced
            this_thread::sleep_for(chrono::milliseconds(10)); // Retry after 10ms
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr))); // Seed for random number generation

    const size_t queueSize = 5; // Circular queue size
    const int numProducers = 2;
    const int numConsumers = 2;
    const int itemsPerProducer = 10;

    CircularQueue q(queueSize);
    atomic<int> producedCount(0);
    atomic<int> consumedCount(0);

    vector<thread> producers, consumers;

    // Start producer threads
    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(producer, ref(q), i + 1, itemsPerProducer, ref(producedCount));
    }

    // Start consumer threads
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back(consumer, ref(q), i + 1, ref(consumedCount));
    }

    // Wait for all threads to finish
    for (auto& t : producers) {
        t.join();
    }
    for (auto& t : consumers) {
        t.join();
    }

    cout << "All producers and consumers finished." << endl;
    cout << "Total items produced: " << producedCount << endl;
    cout << "Total items consumed: " << consumedCount << endl;

    return 0;
}
