#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>
#include <ctime>

#include <chrono>
#include <thread>

using namespace std::chrono;

typedef unsigned long long ticks;
static __inline__ ticks getticks(void)
{
#if defined (__amd64__) || defined (__x86_64__)
    // Read the high res tick counter into 32 bit int variables "eax" and
    // "edx", and then combine them into 64 bit int "now"
    unsigned int eax, edx;
    asm volatile ("rdtsc" : "=a" (eax), "=d" (edx) : : "memory");
    return (((ACE_UINT64) eax) | (((ACE_UINT64) edx) << 32));
#else
    // Read the high-res tick counter directly into memory variable "now".
    // The A constraint signifies a 64-bit int.
    ticks now;
    asm volatile ("rdtsc" : "=A" (now) : : "memory");
    return now;
#endif
}

// tests
const bool doTestThread = true;
const bool doTestChrono = true;
const bool doTestOnePublisherOneListenerThread = true;
const bool doTestOnePublisherOneListenerThreadBasic = true;
const bool doTestOnePublisherOneListenerThreadParallel = true;

#endif

