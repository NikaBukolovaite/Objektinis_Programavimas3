
#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>			// srautams (naudojama testavimui)
#include <memory>			// std::allocator, atminties valdymui
#include <algorithm>		// std::move, std::copy, std::equal, lexicographical_compare
#include <limits>			// max_size()
#include <initializer_list> // initializer_list palaikymui
#include <iterator>			// std::reverse_iterator

// Vector klasės šablonas
// Veikia su bet kokio tipo T

/**
 * @brief Dinaminis masyvas, panašus į std::vector
 */
template <typename T>
class Vector
{
public:
	// Tipų alias'ai
	using size_type = size_t;
	using value_type = T;
	using reference = T &;
	using const_reference = const T &;
	using iterator = T *;
	using const_iterator = const T *;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// Konstruktoriai ir destruktorius
	Vector() { create(); }
	explicit Vector(size_type n, const T &t = T{}) { create(n, t); }
	Vector(const Vector &v) { create(v.begin(), v.end()); } // copy konstruktorius
	Vector(Vector &&v) noexcept
	{ // move konstruktorius
		dat = v.dat;
		avail = v.avail;
		limit = v.limit;
		v.dat = v.avail = v.limit = nullptr;
	}
	template <class InputIterator>
	Vector(InputIterator first, InputIterator last) { create(first, last); } // range
	Vector(std::initializer_list<T> il) { create(il.begin(), il.end()); }
	~Vector() { uncreate(); } // destruktorius

	// Priskyrimo operatoriai
	Vector &operator=(const Vector &other)
	{
		if (this != &other)
		{
			uncreate();
			create(other.begin(), other.end());
		}
		return *this;
	}
	Vector &operator=(Vector &&other) noexcept
	{
		if (this != &other)
		{
			uncreate();
			dat = other.dat;
			avail = other.avail;
			limit = other.limit;
			other.dat = other.avail = other.limit = nullptr;
		}
		return *this;
	}

	// Main funkcijos

	// Prieiga prie elementų
	reference operator[](size_type i) { return dat[i]; }
	const_reference operator[](size_type i) const { return dat[i]; }

	reference at(size_type i)
	{
		if (i >= size())
			throw std::out_of_range("at(): indeksas viršijo ribas");
		return dat[i];
	}
	const_reference at(size_type i) const
	{
		if (i >= size())
			throw std::out_of_range("at(): indeksas viršijo ribas");
		return dat[i];
	}

	reference front() { return *dat; }
	const_reference front() const { return *dat; }
	reference back() { return *(avail - 1); }
	const_reference back() const { return *(avail - 1); }
	value_type *data() noexcept { return dat; }
	const value_type *data() const noexcept { return dat; }

	iterator begin() { return dat; }
	const_iterator begin() const { return dat; }
	iterator end() { return avail; }
	const_iterator end() const { return avail; }

	const_iterator cbegin() const { return dat; }
	const_iterator cend() const { return avail; }
	reverse_iterator rbegin() { return reverse_iterator(avail); }
	reverse_iterator rend() { return reverse_iterator(dat); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(avail); }
	const_reverse_iterator rend() const { return const_reverse_iterator(dat); }
	const_reverse_iterator crbegin() const { return const_reverse_iterator(avail); }
	const_reverse_iterator crend() const { return const_reverse_iterator(dat); }

	// Vietos valdymas
	size_type size() const { return avail - dat; }
	size_type capacity() const { return limit - dat; }
	size_type max_size() const { return std::numeric_limits<size_type>::max(); }
	bool empty() const noexcept { return size() == 0; }

	void reserve(size_type n)
	{
		if (n > capacity())
			grow(n);
	}
	void shrink_to_fit()
	{
		if (size() < capacity())
		{
			size_type sz = size();
			iterator new_data = alloc.allocate(sz);
			std::uninitialized_move(dat, avail, new_data);
			uncreate();
			dat = new_data;
			avail = dat + sz;
			limit = dat + sz;
		}
	}
	void resize(size_type sz, const T &value = T{})
	{
		if (sz < size())
		{
			while (avail != dat + sz)
				alloc.destroy(--avail);
		}
		else if (sz > capacity())
		{
			grow(sz);
		}
		while (size() < sz)
			push_back(value);
	}

	void clear() noexcept
	{
		uncreate();
		create();
	}

	void push_back(const T &val)
	{
		if (avail == limit)
			grow();
		alloc.construct(avail++, val);
	}
	void push_back(T &&val)
	{
		if (avail == limit)
			grow();
		alloc.construct(avail++, std::move(val));
	}

	template <class... Args>
	reference emplace_back(Args &&...args)
	{
		if (avail == limit)
			grow();
		alloc.construct(avail, std::forward<Args>(args)...);
		return *avail++;
	}

	template <class... Args>
	iterator emplace(iterator pos, Args &&...args)
	{
		size_type index = pos - dat;
		if (avail == limit)
			grow();
		pos = dat + index;
		std::move_backward(pos, avail, avail + 1);
		alloc.construct(pos, std::forward<Args>(args)...);
		++avail;
		return pos;
	}

	iterator insert(iterator pos, size_type n, const T &val)
	{
		size_type index = pos - dat;
		if (avail + n > limit)
			grow(size() + n);
		pos = dat + index;
		std::move_backward(pos, avail, avail + n);
		std::uninitialized_fill(pos, pos + n, val);
		avail += n;
		return pos;
	}

	template <class InputIterator>
	iterator insert(iterator pos, InputIterator first, InputIterator last)
	{
		size_type index = pos - dat;
		size_type n = std::distance(first, last);
		if (avail + n > limit)
			grow(size() + n);
		pos = dat + index;
		std::move_backward(pos, avail, avail + n);
		std::uninitialized_copy(first, last, pos);
		avail += n;
		return pos;
	}

	iterator insert(iterator pos, std::initializer_list<T> il)
	{
		return insert(pos, il.begin(), il.end());
	}

	void pop_back()
	{
		if (!empty())
			alloc.destroy(--avail);
	}

	iterator erase(iterator pos)
	{
		std::move(pos + 1, avail, pos);
		alloc.destroy(--avail);
		return pos;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator new_end = std::move(last, avail, first);
		while (avail != new_end)
			alloc.destroy(--avail);
		return first;
	}

	void swap(Vector &x)
	{
		std::swap(dat, x.dat);
		std::swap(avail, x.avail);
		std::swap(limit, x.limit);
	}

	void assign(size_type n, const T &val)
	{
		clear();
		for (size_type i = 0; i < n; ++i)
			push_back(val);
	}

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		clear();
		for (; first != last; ++first)
			push_back(*first);
	}

	void assign(std::initializer_list<T> il)
	{
		assign(il.begin(), il.end());
	}

	std::allocator<T> get_allocator() const { return alloc; }

	// Lyginimo operatoriai
	bool operator==(const Vector &other) const
	{
		return size() == other.size() && std::equal(begin(), end(), other.begin());
	}
	bool operator!=(const Vector &other) const { return !(*this == other); }
	bool operator<(const Vector &other) const
	{
		return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
	}
	bool operator>(const Vector &other) const { return other < *this; }
	bool operator<=(const Vector &other) const { return !(other < *this); }
	bool operator>=(const Vector &other) const { return !(*this < other); }

private:
	iterator dat = nullptr;	  // pradžia
	iterator avail = nullptr; // pabaiga (naudojami)
	iterator limit = nullptr; // pabaiga (alokacija)
	std::allocator<T> alloc;  // atminties alokatorius

	void create() { dat = avail = limit = nullptr; }

	void create(size_type n, const T &val)
	{
		dat = alloc.allocate(n);
		avail = limit = dat + n;
		std::uninitialized_fill(dat, avail, val);
	}

	template <class InputIterator>
	void create(InputIterator first, InputIterator last)
	{
		size_type n = std::distance(first, last);
		dat = alloc.allocate(n);
		avail = limit = std::uninitialized_copy(first, last, dat);
	}

	void uncreate()
	{
		if (dat)
		{
			while (avail != dat)
				alloc.destroy(--avail);
			alloc.deallocate(dat, limit - dat);
		}
		dat = avail = limit = nullptr;
	}

	void grow(size_type new_cap = 0)
	{
		size_type new_size = std::max(new_cap, std::max(2 * capacity(), size_type(1)));
		iterator new_data = alloc.allocate(new_size);
		iterator new_avail = std::uninitialized_move(dat, avail, new_data);
		uncreate();
		dat = new_data;
		avail = new_avail;
		limit = dat + new_size;
	}
};

#endif