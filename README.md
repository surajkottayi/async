# CSample - A Singleton Class for Managing Upload and Retry Threads

## Overview

`CSample` is a singleton class that manages two separate threads: one for handling uploads and one for retrying failed operations. The class utilizes `std::future`, `std::promise`, and condition variables to control the execution of these threads. It ensures that only one instance of the `CSample` class exists throughout the lifetime of the application and provides methods to start, stop, and manage the upload and retry operations.

## Features

- **Singleton Pattern**: Ensures a single instance of the `CSample` class.
- **Upload Handling**: Manages the upload process with a separate thread.
- **Retry Mechanism**: Manages retry attempts in case of failures with a separate thread.
- **Thread Management**: Start and stop the upload and retry threads asynchronously using `std::async`.
- **Graceful Shutdown**: Allows for a graceful shutdown of both threads using promises and condition variables.

## Classes

### `CSample`

The main class that provides thread management for both uploading and retrying. The class includes the following key methods:

- **`getInstance()`**: Returns the singleton instance of `CSample`.
- **`startUpload()`**: Starts the upload thread if it is not already running.
- **`startRetry()`**: Starts the retry thread if it is not already running.
- **`stopUpload()`**: Stops the upload thread gracefully.
- **`stopRetry()`**: Stops the retry thread gracefully.
- **`stop()`**: Stops both threads (upload and retry) gracefully.
- **`join()`**: Waits for both threads (upload and retry) to finish before continuing execution.

### `m_Instance`

A static shared pointer to the singleton instance of the `CSample` class. This is initialized lazily when `getInstance()` is called.

### `m_IsRunning` and `m_IsRunningRetry`

Atomic flags to track the running status of the upload and retry threads.

### `stopPromise`, `stopRetryPromise`, `stopUploadPromise`

Promises used to signal the threads to stop. The associated `std::future` objects are used in conjunction with condition variables to allow for graceful shutdowns.

### `m_FutShared`, `m_FutUpload`, and `m_FutRetry`

Futures associated with the threads to track their execution state and wait for completion.

### Condition Variables

- **`m_CVUpload`**: Used to synchronize the upload thread.
- **`m_CVRetry`**: Used to synchronize the retry thread.

## Methods

### `getInstance()`

This method checks if the singleton instance exists. If not, it creates and returns the instance. It ensures that only one instance of `CSample` is created throughout the application's lifetime.

```cpp
std::shared_ptr<CSample> CSample::getInstance();
