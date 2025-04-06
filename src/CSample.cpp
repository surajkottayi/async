#include "CSample.hpp"
using namespace sample;
std::shared_ptr<CSample> CSample::m_Instance = nullptr;

std::shared_ptr<CSample> CSample::getInstance()
{
    if (!m_Instance)
    {
        m_Instance = std::shared_ptr<CSample>(new CSample());
    }
    return m_Instance;
}
CSample::CSample()
{
    m_IsRunning.store(false);
    m_IsRunningRetry.store(false);
    stopPromise = std::make_shared<std::promise<void>>();
    m_FutShared = stopPromise->get_future();
    predicateCancel = [this]()
    {
        return (m_FutShared.wait_for(std::chrono::milliseconds(10)) != std::future_status::timeout);
    };
}

void CSample::retry()
{
    if (!(m_IsRunningRetry.load()))
    {
        m_IsRunningRetry.store(true);
        stopRetryPromise = std::make_shared<std::promise<void>>();
        stopRetryFuture = stopRetryPromise->get_future();
        auto lPredicateCancel = [this]()
        {
            return (m_FutShared.wait_for(std::chrono::milliseconds(10)) != std::future_status::timeout);
        };
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_MutRetry);
            m_CVRetry.wait(lock, [&]()
                           { return m_IsRunningRetry.load() || predicateCancel(); });

            if (predicateCancel())
            {
                std::cout << "Retry thread cancelled" << std::endl;
                break;
            }

            std::cout << "Retry thread is running" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1'000));
        }
    }
}
void CSample::upload()
{
    if (!(m_IsRunning.load()))
    {
        m_IsRunning.store(true);
        stopUploadPromise = std::make_shared<std::promise<void>>();
        stopUploadFuture = stopUploadPromise->get_future();
        auto lPredicateCancel = [this]()
        {
            return (m_FutShared.wait_for(std::chrono::milliseconds(10)) != std::future_status::timeout);
        };
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_MutUpload);
            m_CVUpload.wait(lock, [&]()
                            { return m_IsRunning.load() || predicateCancel(); });

            if (predicateCancel())
            {
                std::cout << "Upload thread cancelled" << std::endl;
                break;
            }

            std::cout << "Upload thread is running" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1'000));
        }
    }
}

void CSample::startUpload()
{
    if (!(m_FutUpload.valid()))
    {
        m_FutUpload = std::async(std::launch::async, &CSample::upload, this);
    }
    else
    {
        std::cout << "Upload thread is already running" << std::endl;
    }
}

void CSample::startRetry()
{
    if (!(m_FutRetry.valid()))
    {
        m_FutRetry = std::async(std::launch::async, &CSample::retry, this);
    }
    else
    {
        std::cout << "Retry thread is already running" << std::endl;
    }
}
void CSample::stopUpload()
{
    if (m_FutUpload.valid())
    {
        try
        {
            stopUploadPromise->set_value();
        }
        catch (std::exception &lExcep)
        {
        }
        m_IsRunning.store(false);
        m_CVUpload.notify_one();
        m_FutUpload.get();
    }
}
void CSample::stopRetry()
{
    if (m_FutRetry.valid())
    {
        try
        {
            stopRetryPromise->set_value();
        }
        catch (std::exception &lExcep)
        {
        }
        m_IsRunningRetry.store(false);
        m_CVRetry.notify_one();
        m_FutRetry.get();
    }
}
void CSample::stop()
{
    try
    {
        if (stopPromise)
        {
            stopPromise->set_value();
        }
    }
    catch (std::exception &lExcep)
    {
    }
    m_IsRunningRetry.store(false);
    m_CVRetry.notify_one();
    m_FutRetry.get();
}
void CSample::join()
{

    if (m_FutRetry.valid())
    {
        m_FutRetry.get();
    }
    if (m_FutUpload.valid())
    {
        m_FutUpload.get();
    }
}