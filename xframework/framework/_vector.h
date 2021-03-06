#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdlib.h>
//#define CHECK_BOUNDS

template <typename T, int step = 16>
class _Vector {
public:
	T     *base;
	size_t usedCapacity, maxCapacity;

	inline _Vector() {
		base         = NULL;
		usedCapacity = maxCapacity = 0;
	}

	inline _Vector(_Vector<T> *v) {
		if (0 != v->maxCapacity) {
			base = (T*)malloc(v->maxCapacity * sizeof(T));
			memcpy(base, v->base, v->usedCapacity * sizeof(T));
			usedCapacity = v->usedCapacity;
			maxCapacity  = v->maxCapacity;
		} else   {
			base         = NULL;
			usedCapacity = maxCapacity = 0;
		}
	}

	inline _Vector(size_t initialCapacity) {
		maxCapacity  = initialCapacity;
		base         = (T*)malloc(initialCapacity * sizeof(T));
		usedCapacity = 0;
	}

	inline ~_Vector() {
		if (NULL != base) {
			//free(base);
		}
	}

	typedef T* iterator;

	iterator begin() {
		return (iterator)base;
	}

	iterator end() {
		return (_Vector<T>::iterator)(((size_t)base) + (usedCapacity * sizeof(T)));
	}

	iterator at(size_t i) {
#ifdef CHECK_BOUNDS
		if (i < usedCapacity) {
#endif
		return (_Vector<T>::iterator)(((size_t)base) + (i * sizeof(T)));
#ifdef CHECK_BOUNDS
	}

	return NULL;
#endif
	}

	iterator insert(iterator at, T *val) {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		memmove((void*)at(i + 1), (void*)at(i), (1 + (usedCapacity - i)) * sizeof(T));
		(*at) = *val;
		usedCapacity++;
	}

	inline size_t index(iterator it) {
		return (((size_t)it) - ((size_t)base)) / sizeof(T);
	}

	inline size_t size() {
		return usedCapacity;
	}

	inline size_t capacity() {
		return maxCapacity;
	}

	inline bool empty() {
		return 0 == usedCapacity;
	}

	inline T *front() {
		return (T*)base;
	}

	inline T *back() {
		return (T*)(((size_t)base) + (usedCapacity * sizeof(T)));
	}

	inline void push_back(T *value) {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		((T*)base)[++usedCapacity] = *value;
	}

	inline void push_back(T value) {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		((T*)base)[usedCapacity++] = value;
	}

	inline T *push_new() {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		return &((T*)base)[usedCapacity++];
	}

	inline void pop_back() {
		if (0 != usedCapacity) {
			usedCapacity--;
		}
	}

	inline void reserve(size_t cap) {
		if (maxCapacity < cap) {
			maxCapacity = cap;
			base = (T*)realloc(base, cap * sizeof(T));
		}
	}

	inline void clear() {
		usedCapacity = 0;
	}

	inline void remove(size_t i) {
		if (usedCapacity != i) {
			memmove((void*)at(i), (void*)at(i + 1), (1 + (usedCapacity - i)) * sizeof(T));
		}

		usedCapacity--;
	}

	inline void erase(iterator it) {
		remove(index(it));
	}

	inline iterator find(T value) {
		for (iterator it = begin(); it != end(); it++) {
			if (value == *it) {
				return it;
			}
		}

		return NULL;
	}

	inline T operator[](size_t i) {
#ifdef CHECK_BOUNDS
		if (i < usedCapacity) {
#endif
		return ((T*)base)[i];
#ifdef CHECK_BOUNDS
	}

	return NULL;
#endif
	}

	inline _Vector<T> *operator=(_Vector<T> *v) {
		if (0 != v->usedCapacity) {
			base         = (T*)realloc(base, v->maxCapacity * sizeof(T));
			usedCapacity = v->usedCapacity;
			maxCapacity  = v->maxCapacity;
			memcpy(base, v->base, v->usedCapacity * sizeof(T));
		} else {
			base         = NULL;
			usedCapacity = maxCapacity = 0;
		}
	}
};

template <typename T, int size = 16>
class _FixedVector {
public:
	T      base[size];
	size_t usedCapacity, maxCapacity;

	typedef T* iterator;

	inline _FixedVector() {
		usedCapacity = 0;
		maxCapacity  = size;
	}

	inline _FixedVector(_FixedVector<T> *v) {
		if (0 != v->maxCapacity) {
			base = (T*)malloc(v->maxCapacity * sizeof(T));
			memcpy(base, v->base, v->usedCapacity * sizeof(T));
			usedCapacity = v->usedCapacity;
			maxCapacity  = v->maxCapacity;
		} else   {
			base         = NULL;
			usedCapacity = maxCapacity = 0;
		}
	}

	inline _FixedVector(size_t initialCapacity) {
		maxCapacity  = initialCapacity;
		base         = (T*)malloc(initialCapacity * sizeof(T));
		usedCapacity = 0;
	}

	inline ~_FixedVector() {
		if (NULL != base) {
			//free(base);
		}
	}

	iterator begin() {
		return (iterator)base;
	}

	iterator end() {
		return (_FixedVector<T>::iterator)(((size_t)base) + (usedCapacity * sizeof(T)));
	}

	iterator at(size_t i) {
#ifdef CHECK_BOUNDS
		if (i < usedCapacity) {
#endif
		return (_FixedVector<T>::iterator)(((size_t)base) + (i * sizeof(T)));
#ifdef CHECK_BOUNDS
	}

	return NULL;
#endif
	}

	iterator insert(iterator at, T *val) {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		memmove((void*)at(i + 1), (void*)at(i), (1 + (usedCapacity - i)) * sizeof(T));
		(*at) = *val;
		usedCapacity++;
	}

	inline size_t index(iterator it) {
		return (((size_t)it) - ((size_t)base)) / sizeof(T);
	}

	inline size_t size() {
		return usedCapacity;
	}

	inline size_t capacity() {
		return maxCapacity;
	}

	inline bool empty() {
		return 0 == usedCapacity;
	}

	inline T *front() {
		return (T*)base;
	}

	inline T *back() {
		return (T*)(((size_t)base) + (usedCapacity * sizeof(T)));
	}

	inline void push_back(T *value) {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		((T*)base)[++usedCapacity] = *value;
	}

	inline void push_back(T value) {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		((T*)base)[usedCapacity++] = value;
	}

	inline T *push_new() {
		if ((usedCapacity + 1) >= maxCapacity) {
			reserve(maxCapacity + step);
		}

		return &((T*)base)[usedCapacity++];
	}

	inline void pop_back() {
		if (0 != usedCapacity) {
			usedCapacity--;
		}
	}

	inline void clear() {
		usedCapacity = 0;
	}

	inline void remove(size_t i) {
		if (usedCapacity != i) {
			memmove((void*)at(i), (void*)at(i + 1), (1 + (usedCapacity - i)) * sizeof(T));
		}

		usedCapacity--;
	}

	inline void erase(iterator it) {
		remove(index(it));
	}

	inline iterator find(T value) {
		for (iterator it = begin(); it != end(); it++) {
			if (value == *it) {
				return it;
			}
		}

		return NULL;
	}

	inline T operator[](size_t i) {
#ifdef CHECK_BOUNDS
		if (i < usedCapacity) {
#endif
		return ((T*)base)[i];
#ifdef CHECK_BOUNDS
	}

	return NULL;
#endif
	}

	inline _FixedVector<T> *operator=(_FixedVector<T> *v) {
		if (0 != v->usedCapacity) {
			usedCapacity = v->usedCapacity;
			memcpy(base, v->base, v->usedCapacity * sizeof(T));
		} else {
			usedCapacity = 0;
		}
	}
};

#endif
