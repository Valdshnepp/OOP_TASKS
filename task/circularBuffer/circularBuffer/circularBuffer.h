#pragma once

typedef char value_type;

class CircularBuffer final
{
    value_type* m_buffer;
    int m_capacity;
    int m_size;
    int m_first;
public:
    
    CircularBuffer():CircularBuffer(0) {}

    explicit CircularBuffer(int capacity);

    CircularBuffer(int capacity, const value_type& elem);

    CircularBuffer(const CircularBuffer& cb);

    CircularBuffer& operator=(const CircularBuffer& cb);

    ~CircularBuffer()
    {
    	delete m_buffer;
    }

    value_type& operator[](int i);

    const value_type& operator[](int i) const;

    value_type& at(int i);

    const value_type& at(int i) const;

    value_type& front();

    value_type& back();

    const value_type& front() const;

    const value_type& back() const;

    value_type* linearize();

    bool is_linearized() const;

    void rotate(int new_begin);

    inline int size() const
    {
        return m_size;
    }
    inline bool empty() const
    {
        return size() == 0;
    }
    inline bool full() const
    { 
        return size() == capacity();
    }
    inline int reserve() const
    {
        return capacity() - size();
    }
    inline int capacity() const
    {
        return m_capacity;
    }

    void set_capacity(int new_capacity);

    void resize(int new_size, const value_type &item = value_type());

    void swap(CircularBuffer &cb);

    void push_back(const value_type& item = value_type());

    void push_front(const value_type& item = value_type());

    void insert(int pos, const value_type& item = value_type());

    void pop_back();

    void pop_front();

    void erase(int first, int last);

    void clear();
};

// надо бы сравнить все поля, а не только массив данных. Ты тут как минимум их capacity не сравниваешь.
inline bool operator==(const CircularBuffer& a, const CircularBuffer& b)
{
    if (a.size() == b.size())
    {
        for (int i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

inline bool operator!=(const CircularBuffer& a, const CircularBuffer& b)
{
    // Как-то ортодоксально написано... Давай попроще
    return !operator==(a, b);
}
