#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template<class T>
void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

template <class T>
class Vector {
private:
	const static size_t ScalingFactor = 2;
	size_t m_size;
	size_t m_capacity;
	T* m_elements;
public:
	Vector() : m_size(0), m_capacity(0), m_elements(nullptr) {}
	explicit Vector(const size_t& capacity): m_size(capacity), m_capacity(capacity) {
		Reallocate(capacity);
	}
	Vector(const size_t& size, const T& value): m_size(size), m_capacity(size) {
		Reallocate(m_capacity); 
		for (int k = 0; k < size; ++k)
			m_elements[k] = value;

	}
	~Vector(){
		m_size = 0;
		m_capacity = 0;
		if (m_elements) delete[] m_elements;
	}
	Vector(const Vector& other) {
		m_size = other.Size();
		m_capacity = other.Capacity();
		Reallocate(m_capacity);
		for (size_t i = 0; i < m_size; i++) {
			m_elements[i] = other.m_elements[i];
		}
	}
	Vector& operator=(const Vector& other) {
		if (this == &other) return *this;
		delete[] m_elements;
		m_size = other.Size();
		m_capacity = other.Capacity();
		Reallocate(m_capacity);
		for (int i = 0; i < m_size; ++i)
			m_elements[i] = other.m_elements[i];
		return *this;
	}
	void Clear() {
		m_size = 0;
	}
	void PushBack(const T& value) {
		if (m_capacity == 0) {
			Reserve(1);
		}
		else if (m_size >= m_capacity) Reserve(m_capacity * ScalingFactor);
		++m_size;
		m_elements[m_size - 1] = value;
	}
	void PopBack() {
		--m_size;
	}
	void Resize(const int& new_size) {
		if (new_size < m_capacity) {
			m_size = new_size;
			return;
		}
		size_t oldSize = m_size;
		Reallocate(new_size, oldSize);
		m_capacity = new_size;
		m_size = new_size;
	}
	void Resize(const int& new_size, const T& value) {
		size_t oldSize = m_size;
		Resize(new_size);
		if (new_size > oldSize) {
			for (int i = oldSize; i < new_size; ++i)
				m_elements[i] = value;
		}
	}
	void Reallocate(const size_t& capacity, const size_t& size) {
		T* buffer = m_elements;
		m_elements = new T[capacity];
		for (size_t i = 0; i < size; ++i)
			m_elements[i] = buffer[i];
		if (buffer) delete[]buffer;
	}
	void Reallocate(const size_t& capacity) {
		m_elements = new T[capacity];
	}

	void Reserve(const size_t& new_cap) {
		size_t max = (new_cap > m_capacity) ? new_cap : m_capacity;
		Reallocate(max, m_size);
		m_capacity = max;
	}
	void ShrinkToFit() {
		if (m_capacity != m_size) Reallocate(m_size, m_size);
		m_capacity = m_size; 
	}
	void Swap(Vector& other) {
		swap(other.m_elements, m_elements);
		swap(m_size, other.m_size);
		swap(m_capacity, other.m_capacity);
	}
	T& Front() {
		return m_elements[0];
	}
	const T& Front() const {
		return m_elements[0];
	}
	T& Back() {
		return m_elements[m_size - 1];
	}
	const T& Back() const {
		return m_elements[m_size - 1];
	}
	const T& operator[](int l)const {
		return m_elements[l];
	}
	T& operator[](int l) {
		return m_elements[l];
	}
	T* Data() {
		return m_elements;
	}
	const T* Data() const {
		return m_elements;
	}
	bool Empty() const {
		return (m_size == 0);
	}
	int Size() const {
		return m_size;
	}
	int Capacity() const {
		return m_capacity;
	}
	template<class X>
	friend bool operator< (const Vector<X>&, const Vector<X>& rStr);
	template<class X>
	friend bool operator<=(const Vector<X>&, const Vector<X>& rStr);
	template<class X>
	friend bool operator> (const Vector<X>&, const Vector<X>& rStr);
	template<class X>
	friend bool operator>=(const Vector<X>&, const Vector<X>& rStr);
	template<class X>
	friend bool operator==(const Vector<X>&, const Vector<X>& rStr);
	template <class X>
	friend bool operator!=(const Vector<X>&, const Vector<X>& rStr);
};

template <class T> bool operator<(const Vector<T>& lStr, const Vector<T>& rStr) {
	int i = 0;
	if (lStr.m_size == 0 && rStr.m_size != 0) return true;
	if (rStr.m_size == 0 && lStr.m_size != 0) return false;
	if (lStr.m_size == 0 && rStr.m_size == 0) return false;
	while (i < lStr.m_size && i < rStr.m_size && lStr.m_elements[i] == rStr.m_elements[i]) ++i;
	if (i == lStr.m_size || i == rStr.m_size) --i;
	if (lStr.m_elements[i] != rStr.m_elements[i])
		return (lStr.m_elements[i] < rStr.m_elements[i]);
	if (lStr.m_size >= rStr.m_size) return false;
	else return true;
}
template <class T> bool operator<=(const Vector<T>& lStr, const Vector<T>& rStr) {
	return (lStr < rStr || lStr == rStr);
}
template <class T> bool operator>(const Vector<T>& lStr, const Vector<T>& rStr) {
	return (rStr < lStr);
}
template <class T> bool operator>=(const Vector<T>& lStr, const Vector<T>& rStr) {
	return rStr <= lStr;
}
template <class T> bool operator==(const Vector<T>& lStr, const Vector<T>& rStr) {
	if (lStr.m_size != rStr.m_size) return false;
	for (int i = 0; i < lStr.m_size; ++i)
		if (lStr.m_elements[i] != rStr.m_elements[i]) return false;
	return true;
}
template <class T> bool operator!=(const Vector<T>& lStr, const Vector<T>& rStr) {
	return !(lStr == rStr);
}

#endif // VECTOR_H
