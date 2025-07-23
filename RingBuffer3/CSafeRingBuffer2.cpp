#include <string>
#include "CSafeRingBuffer2.h"

CSafeRingBuffer::CSafeRingBuffer(unsigned long long max_buf_size, unsigned char id)
    : m_capacity(max_buf_size)
    , m_head(0)
    , m_tail(0)
    , m_id(id)
    , m_running(true)
{
    m_buffer = new unsigned char[m_capacity] {0};
}

CSafeRingBuffer::~CSafeRingBuffer()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_running = false;
        m_cv.notify_all();
    }

    delete[] m_buffer;
}

void CSafeRingBuffer::write(const unsigned char* data, unsigned long long size)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (unsigned long long i = 0; i < size; ++i) {
        m_buffer[m_tail] = data[i];
        unsigned long long next_tail = (m_tail + 1) % m_capacity;

        if (next_tail == m_head) {
            m_head = (m_head + 1) % m_capacity;
        }
        m_tail = next_tail;
    }

    m_cv.notify_one();
}

unsigned long long CSafeRingBuffer::read(std::unique_ptr<unsigned char[]>& out)
{
    std::lock_guard<std::mutex> lock(m_mutex);

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

    return count;
}

unsigned long long CSafeRingBuffer::wait_read(std::unique_ptr<unsigned char[]>& out)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [&]() { return m_head != m_tail || !m_running; });

    unsigned long long count = 0;
    if (m_running) 
    {
        if (m_head != m_tail)
        {
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
    }

    return count;
}

unsigned long long CSafeRingBuffer::read_bytes(std::unique_ptr<unsigned char[]>& out, unsigned long long read_size)
{
    std::lock_guard<std::mutex> lock(m_mutex);

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

    return count;
}

unsigned char CSafeRingBuffer::read_byte()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    unsigned char val = 0;
    if (m_head != m_tail) {
        val = m_buffer[m_head];
        m_head = (m_head + 1) % m_capacity;
    }

    return val;
}

unsigned long long CSafeRingBuffer::current_size()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    unsigned long long size = (m_tail >= m_head) ? (m_tail - m_head) : (m_capacity - m_head + m_tail);

    return size;
}

void CSafeRingBuffer::reset()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_head = 0;
    m_tail = 0;

}
