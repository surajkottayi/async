#include <iostream>
#include "CSample.hpp"
#include <future>
#include <vector>
#include <chrono>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
using namespace sample;

inline constexpr int MAX_NO_THREAD = 5;

inline std::atomic_int16_t g_Common = 0;
std::condition_variable g_CVWait;
std::mutex g_mutex;
std::mutex g_mutex2;
void workerThread()
{
    while (true)
    {
        // std::scoped_lock lock(g_mutex,g_mutex2);
        if (std::try_lock(g_mutex, g_mutex2) == -1)
        {
        }
        g_Common++;
        std::cout << "Running" << std::this_thread::get_id() << ":::" << g_Common << std::endl;
        // std::scoped_lock<std::mutex> lock2(g_mutex2);
    }
}
void workerThread2()
{
    while (true)
    {
        // std::scoped_lock lock(g_mutex2,g_mutex);
        if (std::try_lock(g_mutex2, g_mutex) == -1)
        {
        }
        {
            /* code */
            // std::scoped_lock lock2(g_mutex);
        }
        /* code */
        g_Common++;
        std::cout << "Running" << std::this_thread::get_id() << ":::" << g_Common << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        g_CVWait.notify_all();
    }
}

int main(int argc, char const *argv[])
{
    std::shared_ptr<CSample> lpSample = CSample::getInstance();
    // std::vector<std::future<void>> lvFut;
    // std::vector<std::thread> lvFut;
    // lpSample->startUpload();

    // std::thread th(&workerThread);
    // std::thread th1(&workerThread2);
    // lvFut.push_back(std::move(th));
    // lvFut.push_back(std::move(th1));
    // for (auto &item : lvFut)
    // {
    //     item.join();
    // }

    lpSample->startUpload();
    lpSample->startRetry();
    std::this_thread::sleep_for(std::chrono::milliseconds(5'000));
    lpSample->stop();
    std::cout << "main thread is waitng" << std::endl;

    return 0;
}
