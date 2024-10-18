#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <iomanip>
#include <functional>

#define M_PI 3.14159265358979323846

using namespace std;

mutex myMutex;

void monte_carlo(unsigned int& global_count, int POINTS, unsigned int seed){
    srand(seed);
    int local_count = 0;
    double x, y, distance;

    for (int i = 0; i < POINTS; i++) {
        x = ((double)rand()) / ((double)RAND_MAX);
        y = ((double)rand()) / ((double)RAND_MAX);
        distance = pow(x, 2) + pow(y, 2);
        if (distance <= 1) {
            local_count++;
       }
    }
    
    lock_guard<mutex> myLock(myMutex);
    global_count += local_count;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        cout << "usage: monte [1...10] [10...1000000]" << endl;
        return 1;
    }

    int threads = atoi(argv[1]);
    int points = atoi(argv[2]);

    if (threads < 1 || threads > 10 || points < 10 || points > 1000000) {
        cout << "usage: monte [1...10] [10...1000000]" << endl;
        return 1;
    }

    unsigned int global_count = 0;
    long long slice;
    int i;

    double pi_estimate, delta;
    chrono::duration<double> dur;

    auto start_time = chrono::system_clock::now();

    thread t[threads];
    slice = points / threads;
    int remainder = points % threads;

    for (i = 0; i < threads; ++i) {
        int points_for_thread = slice + (i < remainder ? 1 : 0);
        t[i] = thread(monte_carlo, ref(global_count), points_for_thread, i + time(0));
    }

    for (i = 0; i < threads; ++i){
        t[i].join();
    }

    pi_estimate = (4.0 * global_count) / points;
    delta = fabs(M_PI - pi_estimate);
    dur = chrono::system_clock::now() - start_time;

    // cout << threads << " THREADS - " 
    //      << "POINTS: " << points 
    //      << ", ESTIMATED PI: " << pi_estimate 
    //      << ", DELTA: " << setprecision(4) 
    //      << delta << ", TIME: " 
    //      << setprecision(6) << dur.count() 
    //      << " s." << endl;
        cout << threads << " THREADS - " 
         << "POINTS: " << points << " "
         << pi_estimate << " "
         << setprecision(4) 
         << delta << " "
         << setprecision(6) << dur.count() 
         << endl;


    return 0;
}
