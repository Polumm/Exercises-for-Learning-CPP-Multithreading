#include <iostream>
#include <thread>

using namespace std;

// ++counter is not an atomic operation. It consists of:
// Reading the value of counter.
// Incrementing the value.
// Writing the incremented value back to counter.

long long counter = 0; // Shared resource

void increment() {
    for (int i = 0; i < 1000000; ++i) { // Larger iteration count
        ++counter; // Increment shared counter
    }
}

int main() {
    // Create two threads that increment the counter
    thread t1(increment);
    thread t2(increment);

    t1.join(); // Wait for thread 1 to finish
    t2.join(); // Wait for thread 2 to finish

    cout << "Final counter value: " << counter << endl;

    return 0;
}
