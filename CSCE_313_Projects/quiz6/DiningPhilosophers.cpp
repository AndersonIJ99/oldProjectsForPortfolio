#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include "Semaphore.h"
using namespace std;

const int THINK = 0;
const int HUNGRY = 1;
const int EAT = 2;

Semaphore me(1); // used for mutual exclusion
// Semaphore s[5] = {0,0,0,0,0}; // for synchronization
// since Semaphore arrays aren't possible this is my implementation
Semaphore s1(0);
Semaphore s2(0);
Semaphore s3(0);
Semaphore s4(0);
Semaphore s5(0);
int pflag[5] = {THINK,THINK,THINK,THINK,THINK}; // philisopher flag {THINK, HUNGRY, EAT}, initially THINK

void test(int i)            /* Let phil[i] eat, if waiting */
{
    // conditional statements protect out of bounds
    int before, after;
    if (i == 0) {
        before = 4;
        after = 1;
    }
    else if (i == 4) {
        before = 3;
        after = 0;
    }
    else {
        before = i - 1;
        after = i + 1;
    }
    if ( pflag[i] == HUNGRY && pflag[before] != EAT && pflag[after] != EAT) {
        pflag[i] = EAT;
        // s[i].V();
        // 
        switch (i) {
            case 0:
                s1.V();
                break;
            case 1:
                s2.V();
                break;
            case 2:
                s3.V();
                break;
            case 3:
                s4.V();
                break;
            case 4:
                s5.V();
                break;
        }
    }
}

// eating function for philosophers
void eat(int i) {
    cout << "Philosopher " << i << " starting to eat" << endl;
    sleep(rand()%5 + 1); // every phil spends a random amount of time eating
    cout << "Philosopher "  << i << " done eating" << endl;
    pflag[i] = THINK;
}

// thinking function for philosophers
void think (int i) {
    // cout << "Philosopher "  << i << " thinking" << endl;
    sleep(rand()%4 + 1); // spends a random time thinking
}

void take_chopsticks(int i) {
    me.P();               /* critical section */
    pflag[i] = HUNGRY;
    // cout << "Philosopher "  << i << " is hungry" << endl;
    test(i);
    me.V();                 /* end critical section */
    // s[i].P();              /* Eat if enabled */
    switch (i) {
        case 0:
            s1.P();
            break;
        case 1:
            s2.P();
            break;
        case 2:
            s3.P();
            break;
        case 3:
            s4.P();
            break;
        case 4:
            s5.P();
            break;
    }
}

void drop_chopsticks(int i) {
    me.P();                /* critical section */
    // conditional statements protect out of bounds
    if (i == 0) {
        test(4);
        test(1);
    }
    else if (i == 4) {
        test(3);
        test(0);
    }
    else {
        test(i-1);               /* Let phil. on left eat if possible */
        test(i+1);               /* Let phil. on right eat if possible */
    }
    me.V();                  /* up critical section */
}

void philosopher(int i) {
    while (true) {
        // THINKING;
        pflag[i] = THINK;
        think(i);
        take_chopsticks(i);
        // EATING;
        eat(i);
        drop_chopsticks(i);
    }
}

// set up, start, and finish philosopher threads
int main() {

    vector<thread> phils;

    for (int i = 0; i < 5; i++) {
        phils.push_back (thread (philosopher, i));
    }

    for (int i = 0; i < phils.size(); i++) {
        phils[i].join();
    }

    return 0;
}