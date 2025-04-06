#ifndef CSample_HPP
#define CSample_HPP

#include <memory>
#include <future>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <iostream>
namespace sample
{

    class CSample
    {
    public:
        [[nodiscard]] static std::shared_ptr<CSample> getInstance();
        CSample();
        void retry();
        void upload();
        void startUpload();

        ~CSample() = default;
        void startRetry();
        void stopUpload();
        void stopRetry();
        void stop();
        void join();

    private:
        // CSample() = default;

        CSample(const CSample &) = delete;
        CSample &operator=(const CSample &) = delete;

        static std::shared_ptr<CSample> m_Instance;

    public:
        

    private:
    private:
        // Upload
        std::atomic_bool m_IsRunning;
        std::future<void> m_FutUpload;
        std::shared_ptr<std::promise<void>> stopUploadPromise;
        std::future<void> stopUploadFuture;
        std::condition_variable m_CVUpload;
        std::mutex m_MutUpload;

        // Retry
        std::atomic_bool m_IsRunningRetry;
        std::future<void> m_FutRetry;
        std::shared_ptr<std::promise<void>> stopRetryPromise;
        std::future<void> stopRetryFuture;
        std::condition_variable m_CVRetry;
        std::mutex m_MutRetry;

        // Common
        std::shared_future<void> m_FutShared;
        std::shared_ptr<std::promise<void>> stopPromise;
        std::function<bool()> predicateCancel;
    };

} // namespace name
#endif // CSample_HPP