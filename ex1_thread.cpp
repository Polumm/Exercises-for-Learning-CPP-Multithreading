#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

// Simple thread function
void sayHello() {
    cout << "Hello from a thread!" << endl;
}

// Producer function for multi-threading
void producer(int producerId, int jobsPerProducer) {
    for (int i = 0; i < jobsPerProducer; ++i) {
        cout << "Producer " << producerId << " producing job " << i + 1 << endl;
        this_thread::sleep_for(chrono::milliseconds(500)); // Simulate work
    }
}

int main() {
    // Way 1: Simple single-threaded function
    cout << "Starting a simple thread..." << endl;
    thread simpleThread(sayHello); // Create a thread to sayHello
    simpleThread.join(); // Wait for the thread to complete
    cout << "Simple thread finished." << endl;

    // Way 2: Multi-threaded producer example
    cout << "\nStarting producer threads..." << endl;

    vector<thread> producerThreads;
    int numProducers = 3;
    int jobsPerProducer = 5;

    // Create threads for producers
    for (int i = 0; i < numProducers; ++i) {
        producerThreads.emplace_back(producer, i + 1, jobsPerProducer);
    }

    // Wait for all threads to finish
    for (auto& t : producerThreads) {
        t.join(); // Ensure each thread completes its work
    }

    cout << "All producer threads have finished their work." << endl;

    return 0;
}
