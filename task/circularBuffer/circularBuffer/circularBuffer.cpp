#include "circularBuffer.h"
#include <memory>
#include <stdexcept>
#include <cstring>


CircularBuffer::CircularBuffer(int capacity)
{
	if (capacity >= 0)
	{                  
		m_buffer = new value_type[capacity];
		m_capacity = capacity;
		m_size = 0;
		m_first = 0;
	}
	else
	{
		throw std::out_of_range("bad capacity");
	}
}

CircularBuffer::CircularBuffer(int capacity, const value_type& elem):CircularBuffer(capacity)
{
	while (!full()) {
		push_back(elem);
	}
}

CircularBuffer::CircularBuffer(const CircularBuffer& cb)
{
	m_buffer = new value_type[cb.m_capacity];
	memcpy(m_buffer, cb.m_buffer, cb.m_capacity);
	m_capacity = cb.m_capacity;
	m_size = cb.m_size;
	m_first = cb.m_first;
}

CircularBuffer& CircularBuffer::operator=(const CircularBuffer& cb)
{
	if (this == &cb) {
		return *this;
	}
	if (m_capacity)
	{
		delete m_buffer;
	}
	m_buffer = new value_type[cb.m_capacity];
	memcpy(m_buffer, cb.m_buffer, cb.m_capacity);
	m_capacity = cb.m_capacity;
	m_size = cb.m_size;
	m_first = cb.m_first;
	return *this;
}

value_type& CircularBuffer::operator[](int i)
{
	return m_buffer[(m_first + i) % m_capacity];
}

const value_type& CircularBuffer::operator[](int i) const
{
	return m_buffer[(m_first + i) % m_capacity];
}

value_type& CircularBuffer::at(int i)
{
	if (i >= m_size) {
		throw std::out_of_range("out of range");
	}
	return operator[](i);
}

// А почему у тебя отличается реализация от простого at? Методы же одинаковые, просто доступ к данным разный.
// 
const value_type& CircularBuffer::at(int i) const
{
	if (i >= m_size) {
		throw std::out_of_range("out of range");
	}
	return operator[](i);
}

value_type& CircularBuffer::front()
{
	if (m_size == 0) {
		throw std::out_of_range("out of range");
	}
	return m_buffer[m_first];
}

value_type& CircularBuffer::back()
{
	if (m_size == 0) {
		throw std::out_of_range("out of range");
	}
	return operator[](m_size - 1);
}

const value_type& CircularBuffer::front() const
{
	if (m_size == 0) {
		throw std::out_of_range("out of range");
	}
	return m_buffer[m_first];
}

const value_type& CircularBuffer::back() const
{
	if (m_size == 0) {
		throw std::out_of_range("out of range");
	}
	return operator[](m_size-1);
}

//linearize fix
value_type* CircularBuffer::linearize(){
	int true_size = m_size;
	m_size = m_capacity;
	rotate(m_first);
	m_first = 0;
	m_size = true_size;
	return m_buffer;
}

bool CircularBuffer::is_linearized() const
{
	return m_first == 0;
}

//rotate fix
void CircularBuffer::rotate(int new_begin){
	if (new_begin<0 || new_begin>m_size){
		throw std::out_of_range("New begin index out of range");
	}
	for (int j = 0; j < new_begin; j++) {
		auto frnt = front();
		for (int i = 0; i < m_size-1; i++) {
			
			this->operator[](i) = this->operator[](i + 1);
		}
		this->operator[](m_size-1) = frnt;
	}
}

//fixed set_capacity +
void CircularBuffer::set_capacity(int new_capacity){
	if (new_capacity> m_capacity){
		auto newBuffer = new value_type[new_capacity];
		for (int i = 0; i < m_size; i++)
		{
			newBuffer[i] = this->operator[](i);
		}
		delete[] m_buffer;
		m_buffer = newBuffer;
		m_capacity = new_capacity;
		m_first = 0;
	}
	else if (new_capacity<m_capacity)
	{
		clear();
		m_capacity = new_capacity;
	}
}

//fixed resize +
void CircularBuffer::resize(int new_size, const value_type& item)
{
	// set_capacity нужно делать только если new_size больше m_capacity
	// Если размер надо уменьшить, то просто делаешь pop_back нужное число раз.
	if (new_size<0){
		throw std::out_of_range("negative size");
	}

	if (new_size>m_capacity){
		set_capacity(new_size);
	}

	while(m_size<new_size){
		push_back(item);
	}

	while (m_size>new_size){
		pop_back();
	}
}

//fixed swap
void CircularBuffer::swap(CircularBuffer& cb)
{
	// Зачем лианеризацию делаешь? Тут достачно поменять местами все поля.
	if(cb.capacity()>capacity())
	{
		set_capacity(cb.capacity());
		m_capacity = cb.m_capacity;
	}
	else
	{
		cb.set_capacity(capacity());
		cb.m_capacity = m_capacity;
	}
	std::swap(cb.m_first, m_first);
	std::swap(cb.m_size, m_size);
	std::swap(m_buffer, cb.m_buffer);
}

void CircularBuffer::push_back(const value_type& item)
{
	if (m_capacity == 0) {
		return;
	}
	if (m_size < m_capacity) {
		m_buffer[(m_first + m_size) % m_capacity]= item;
		m_size++;
	}
	else {
		m_buffer[m_first] = item;
		m_first = (m_first + 1) % m_capacity;
	}

}

void CircularBuffer::push_front(const value_type& item)
{
	if (m_capacity == 0) {
		return;
	}
	if (m_first - 1 >= 0) {
		m_buffer[m_first - 1] = item;
		m_first--;
	}
	else {
		m_buffer[m_capacity-1] = item;
		m_first = m_capacity-1;
	}
	if (m_size < m_capacity) {
		m_size++;
	}
}

void CircularBuffer::insert(int pos, const value_type& item)
{
	if (pos > m_size) {
		throw std::out_of_range("out of range");
	}
	m_buffer[(m_first + pos) % m_capacity] = item;
}

void CircularBuffer::pop_back()
{
	if (!empty()) {
		m_buffer[(m_first + m_size - 1) % m_capacity] = value_type();
		m_size--;
	}
}

void CircularBuffer::pop_front()
{
	if (!empty()) {
		m_buffer[m_first] = value_type();
		m_first = (m_first + 1) % m_capacity;
		m_size--;
	}
}

//fixed erase +
void CircularBuffer::erase(int first, int last)
{
	if(first>last || first < 0 || last > m_size){
		throw std::out_of_range("bad range");
	}
	for (int i = 0; i< last-first ;i++){
		operator[](i + first) = operator[](i + last);
	}
	m_size -= (last - first);
}

void CircularBuffer::clear()
{
	while (!empty()) {
		pop_back();
	}
}