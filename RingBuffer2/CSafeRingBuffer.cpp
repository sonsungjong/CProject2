#include <string>
#include "CSafeRingBuffer.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif


CSafeRingBuffer::CSafeRingBuffer(unsigned long long max_buf_size, unsigned char id)
    : m_capacity(max_buf_size)
    , m_head(0)
    , m_tail(0)
    , m_id(id)
    , m_running(true)
{
    m_buffer = new unsigned char[m_capacity] {0};

#ifdef _WIN32
    InitializeCriticalSection(&m_cs);
    InitializeConditionVariable(&m_cv);
#else
    pthread_mutex_init(&m_mtx, nullptr);
    pthread_cond_init(&m_cv, nullptr);
#endif
}

CSafeRingBuffer::~CSafeRingBuffer() 
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&m_mtx);
#endif

    m_running = false;

#ifdef _WIN32
    WakeAllConditionVariable(&m_cv);
    LeaveCriticalSection(&m_cs);
    DeleteCriticalSection(&m_cs);
#else
    pthread_cond_broadcast(&m_cv);
    pthread_mutex_unlock(&m_mtx);
    pthread_mutex_destroy(&m_mtx);
    pthread_cond_destroy(&m_cv);
#endif

    delete[] m_buffer;
}

void CSafeRingBuffer::write(const unsigned char* data, unsigned long long size) 
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&mtx);
#endif

    for (unsigned long long i = 0; i < size; ++i) {
        m_buffer[m_tail] = data[i];
        unsigned long long next_tail = (m_tail + 1) % m_capacity;
        if (next_tail == m_head) {
            m_head = (m_head + 1) % m_capacity;
        }
        m_tail = next_tail;
    }

#ifdef _WIN32
    WakeConditionVariable(&m_cv);
    LeaveCriticalSection(&m_cs);
#else
    pthread_cond_signal(&m_cv);
    pthread_mutex_unlock(&mtx);
#endif
}

unsigned long long CSafeRingBuffer::read(std::unique_ptr<unsigned char[]>& out)
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&mtx);
#endif

    unsigned long long count = 0;
    if (m_head != m_tail) {
        if (m_tail > m_head) {
            count = m_tail - m_head;
        }
        else {
            count = m_capacity - m_head + m_tail;
        }

        out.reset(new unsigned char[count]);
        if (m_tail > m_head) { 
            memcpy(out.get(), m_buffer + m_head, count); 
        }
        else {
            unsigned long long first = m_capacity - m_head;
            memcpy(out.get(), m_buffer + m_head, first);
            memcpy(out.get() + first, m_buffer, m_tail);
        }
        m_head = m_tail;
    }

#ifdef _WIN32
    LeaveCriticalSection(&m_cs);
#else
    pthread_mutex_unlock(&mtx);
#endif
    return count;
}

unsigned long long CSafeRingBuffer::wait_read(std::unique_ptr<unsigned char[]>& out)
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
    while (m_head == m_tail && m_running)
        SleepConditionVariableCS(&m_cv, &m_cs, INFINITE);
#else
    pthread_mutex_lock(&mtx);
    while (m_head == m_tail && m_running)
        pthread_cond_wait(&m_cv, &mtx);
#endif

    unsigned long long count = 0;
    if (m_running) {
        if (m_tail > m_head) {
            count = m_tail - m_head;
        }
        else {
            count = m_capacity - m_head + m_tail;
        }

        out.reset(new unsigned char[count]);
        if (m_tail > m_head) {
            memcpy(out.get(), m_buffer + m_head, count);
        }
        else {
            unsigned long long first = m_capacity - m_head;
            memcpy(out.get(), m_buffer + m_head, first);
            memcpy(out.get() + first, m_buffer, m_tail);
        }
        m_head = m_tail;
    }

#ifdef _WIN32
    LeaveCriticalSection(&m_cs);
#else
    pthread_mutex_unlock(&mtx);
#endif
    return count;
}

unsigned long long CSafeRingBuffer::read_bytes(std::unique_ptr<unsigned char[]>& out, unsigned long long read_size)
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&mtx);
#endif

    unsigned long long count = 0;
    if (m_head != m_tail) {
        if (m_tail > m_head) {
            count = m_tail - m_head;
        }
        else {
            count = m_capacity - m_head + m_tail;
        }

        if (count > read_size) {
            count = read_size;
        }
        out.reset(new unsigned char[count]);

        if (m_tail > m_head || m_head + count <= m_capacity) {
            memcpy(out.get(), m_buffer + m_head, count);
        }
        else {
            unsigned long long first = m_capacity - m_head;
            unsigned long long second = count - first;
            memcpy(out.get(), m_buffer + m_head, first);
            memcpy(out.get() + first, m_buffer, second);
        }
        m_head = (m_head + count) % m_capacity;
    }

#ifdef _WIN32
    LeaveCriticalSection(&m_cs);
#else
    pthread_mutex_unlock(&mtx);
#endif
    return count;
}

unsigned char CSafeRingBuffer::read_byte() 
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&mtx);
#endif

    unsigned char val = 0;
    if (m_head != m_tail) {
        val = m_buffer[m_head];
        m_head = (m_head + 1) % m_capacity;
    }

#ifdef _WIN32
    LeaveCriticalSection(&m_cs);
#else
    pthread_mutex_unlock(&mtx);
#endif
    return val;
}

unsigned long long CSafeRingBuffer::current_size()
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&mtx);
#endif

    unsigned long long size = (m_tail >= m_head) ? (m_tail - m_head) : (m_capacity - m_head + m_tail);

#ifdef _WIN32
    LeaveCriticalSection(&m_cs);
#else
    pthread_mutex_unlock(&mtx);
#endif
    return size;
}

void CSafeRingBuffer::reset() 
{
#ifdef _WIN32
    EnterCriticalSection(&m_cs);
#else
    pthread_mutex_lock(&mtx);
#endif

    m_head = 0;
    m_tail = 0;

#ifdef _WIN32
    LeaveCriticalSection(&m_cs);
#else
    pthread_mutex_unlock(&mtx);
#endif
}
