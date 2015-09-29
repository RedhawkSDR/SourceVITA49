/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK core.
 *
 * REDHAWK core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <deque>
#include <list>
#include <string>
#include <iostream>

const unsigned long QUEUE_SIZE     = 1000L;
const unsigned long TOTAL_ELEMENTS = QUEUE_SIZE * 1000L;

template <class T>
class bounded_buffer {
public:

    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;

    explicit bounded_buffer(size_type capacity) : m_unread(0), m_container(capacity) {}


    void set_capacity( size_type new_capacity ) {
        m_container.set_capacity(new_capacity);
    }

    size_type size() {
        return m_container.size();
    }

    void clear() {
        boost::mutex::scoped_lock lock(m_mutex);
        typename container_type::iterator i;
        for( i=m_container.begin(); i != m_container.end();)  {
            value_type t = *i;
            if ( t ) delete t;
            i=m_container.erase(i);
        }
    }

    void push_front(const value_type& item) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
        m_container.push_front(item);
        ++m_unread;
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type* pItem) {

        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
        *pItem = m_container[--m_unread];
        lock.unlock();
        m_not_full.notify_one();

    }

    bool is_not_empty() const { return m_unread > 0; }
    bool is_not_full() const { return m_unread < m_container.capacity(); }

private:
    bounded_buffer(const bounded_buffer&);              // Disabled copy constructor
    bounded_buffer& operator = (const bounded_buffer&); // Disabled assign operator



    size_type m_unread;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;
};

template <class T>
class bounded_buffer_space_optimized {
public:

    typedef boost::circular_buffer_space_optimized<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;

    explicit bounded_buffer_space_optimized(size_type capacity) : m_container(capacity) {}

    void set_capacity( size_type new_capacity ) {
        m_container.set_capacity(new_capacity);
    }

    size_type size() {
        return m_container.size();
    }

    void clear() {
        boost::mutex::scoped_lock lock(m_mutex);
        typename container_type::iterator i;
        for( i=m_container.begin(); i != m_container.end();)  {
            value_type t = *i;
            if ( t ) delete t;
            i=m_container.erase(i);
        }
    }

    void push_front(const value_type& item) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_full, this));
        m_container.push_front(item);
        lock.unlock();
        m_not_empty.notify_one();


    }

    void pop_back(value_type* pItem) {

        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_empty, this));
        *pItem = m_container.back();
        m_container.pop_back();
        lock.unlock();
        m_not_full.notify_one();
    }


    bool is_not_empty() const { return m_container.size() > 0; }
    bool is_not_full() const { return m_container.size() < m_container.capacity(); }

private:

    bounded_buffer_space_optimized(const bounded_buffer_space_optimized&);              // Disabled copy constructor
    bounded_buffer_space_optimized& operator = (const bounded_buffer_space_optimized&); // Disabled assign operator


    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;
};

template <class T>
class bounded_buffer_deque_based {
public:

    typedef std::deque<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;

    explicit bounded_buffer_deque_based(size_type capacity) : m_capacity(capacity) {}

    void set_capacity( size_type new_capacity ) {
        m_capacity = new_capacity;
    }

    size_type size() {
        return m_container.size();
    }

    void clear() {
        boost::mutex::scoped_lock lock(m_mutex);
        typename container_type::iterator i;
        for( i=m_container.begin(); i != m_container.end();)  {
            value_type t = *i;
            if ( t ) delete t;
            i=m_container.erase(i);
        }
    }

    void push_front(const value_type& item) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer_deque_based<value_type>::is_not_full, this));
        m_container.push_front(item);
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type* pItem) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer_deque_based<value_type>::is_not_empty, this));
        *pItem = m_container.back();
        m_container.pop_back();
        lock.unlock();
        m_not_full.notify_one();
    }

    bool is_not_empty() const { return m_container.size() > 0; }
    bool is_not_full() const { return m_container.size() < m_capacity; }

private:

    bounded_buffer_deque_based(const bounded_buffer_deque_based&);              // Disabled copy constructor
    bounded_buffer_deque_based& operator = (const bounded_buffer_deque_based&); // Disabled assign operator



    size_type m_capacity;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;
};

template <class T>
class bounded_buffer_list_based {
public:

    typedef std::list<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;

    explicit bounded_buffer_list_based(size_type capacity) : m_capacity(capacity) {}

    void set_capacity( size_type new_capacity ) {
        m_capacity = new_capacity;
    }

    size_type size() {
        return m_container.size();
    }

    void clear() {
        boost::mutex::scoped_lock lock(m_mutex);
        typename container_type::iterator i;
        for( i=m_container.begin(); i != m_container.end();)  {
            value_type t = *i;
            if ( t ) delete t;
            i=m_container.erase(i);
        }
    }

    void push_front(const value_type& item) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer_list_based<value_type>::is_not_full, this));
        m_container.push_front(item);
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type* pItem) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer_list_based<value_type>::is_not_empty, this));
        *pItem = m_container.back();
        m_container.pop_back();
        lock.unlock();
        m_not_full.notify_one();
    }

    bool is_not_empty() const { return m_container.size() > 0; }
    bool is_not_full() const { return m_container.size() < m_capacity; }

private:

    bounded_buffer_list_based(const bounded_buffer_list_based&);              // Disabled copy constructor
    bounded_buffer_list_based& operator = (const bounded_buffer_list_based&); // Disabled assign operator


    const size_type m_capacity;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;
};


#ifdef BTEST
template<class Buffer>
class Consumer {

    typedef typename Buffer::value_type value_type;
    Buffer* m_container;
    value_type m_item;

public:
    Consumer(Buffer* buffer) : m_container(buffer) {}

    void operator()() {
        for (unsigned long i = 0L; i < TOTAL_ELEMENTS; ++i) {
            m_container->pop_back(&m_item);
        }
    }
};

template<class Buffer>
class Producer {

    typedef typename Buffer::value_type value_type;
    Buffer* m_container;

public:
    Producer(Buffer* buffer) : m_container(buffer) {}

    void operator()() {
        for (unsigned long i = 0L; i < TOTAL_ELEMENTS; ++i) {
            m_container->push_front(value_type());
        }
    }
};

template<class Buffer>
void fifo_test(Buffer* buffer) {

    // Start of measurement
    boost::progress_timer progress;

    // Initialize the buffer with some values before launching producer and consumer threads.
    for (unsigned long i = QUEUE_SIZE / 2L; i > 0; --i) {
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x581))
        buffer->push_front(Buffer::value_type());
#else
        buffer->push_front(BOOST_DEDUCED_TYPENAME Buffer::value_type());
#endif
    }

    Consumer<Buffer> consumer(buffer);
    Producer<Buffer> producer(buffer);

    // Start the threads.
    boost::thread consume(consumer);
    boost::thread produce(producer);

    // Wait for completion.
    consume.join();
    produce.join();

    // End of measurement
}

#endif

#endif // BOUNDEDBUFFER_H
