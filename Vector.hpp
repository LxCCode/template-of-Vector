//感谢《数据结构（C++语言版）》提供的代码

#include<iostream>
using namespace std;

typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T>
static Rank binSearch(T* A, int const& e, Rank lo, Rank hi)
{
	//version1:
	while (lo < hi)
	{
		Rank mi = (lo + hi) >> 1;
		if (e < A[mi]) hi = mi;
		else if (A[mi] < e) lo = mi + 1;
		else return mi;
	}
	return -1;
	//version2:
	//while (1 < hi - lo)
	//{
	//	Rank mi = (lo + hi) >> 1;
	//	(e < A[mi]) ? hi = mi : lo = mi;
	//}
	//return (e == A[lo]) ? lo : -1;
	////version3:
	//while (lo < hi)
	//{
	//	Rank mi = (lo + hi) >> 1;
	//	(e < A[mi]) ? hi = mi : lo = mi + 1;
	//}
	//return --lo;
}

template <typename T> class Vector {
protected:
	Rank _size;
	int _capacity;
	T* _elem;
	void expand();
	void shrink();
	void copyFrom(T const* A, Rank lo, Rank hi);
	T max(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	void mergeSort(Rank lo, Rank hi);
public:
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(Vector<T> const& V)
	{
		if (_elem != NULL) delete[] _elem;
		this->copyFrom(V._elem, 0, V.size());
	}
	Vector(T const* A, Rank  n)
	{
		copyFrom(A, 0, n);
	}
	Vector(T const* A, Rank lo, Rank hi)
	{
		copyFrom(A, lo, hi);
	}
	~Vector()
	{
		if (_elem != NULL)
		{
			delete[] _elem;
			_elem = NULL;
			_size = 0;
		}

	}
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank search(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e) const
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	T& operator[] (Rank r) const;
	Vector<T>& operator= (Vector<T> const& V);
	T remove(Rank r);
	int remove(Rank lo, Rank hi);
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }
	void sort(Rank lo, Rank hi);
	void sort() { sort(0, _size); }
	int deduplicate();
	int uniquify();
};

template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

template <typename T>
Vector<T>& Vector<T>::operator= (Vector<T> const& V)
{
	if (_elem != NULL) delete[] _elem;
	this->copyFrom(V._elem, 0, V.size());
	return *this;
}

template <typename T>
T& Vector<T>::operator[] (Rank r) const
{
	return _elem[r];
}

template <typename T>
void Vector<T>::expand() {
	if (_size < _capacity) return;
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}

template <typename T>
void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1) return;
	if (_size << 2 > _capacity) return;
	T* oldElem = _elem; _elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}

template <typename T>
T Vector<T>::max(Rank lo, Rank hi)
{
	int _max = _elem[lo];
	for (int i = lo + 1; i < hi; i++)
	{
		if (_max < _elem[i])
			_max = _elem[i];
	}
	return _max;
}

template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e)
{
	expand();
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1];
	_elem[r] = e;
	_size++;
	return r;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
	if (lo == hi)
	{
		return 0;
	}
	while (hi < _size) _elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}

template <typename T>
T Vector<T>::remove(Rank r)
{
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}

template <typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while (i < _size)
	{
		(find(_elem[i], 0, i) < 0) ?
			i++ : remove(i);
	}
	return oldSize - _size;
}

template <typename T>
int Vector<T>::uniquify() {
	Rank i = 0, j = 0;
	while (++j < _size)
	{
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];
	}
	_size = ++i;
	shrink();
	return j - i;
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
	return binSearch(_elem, e, lo, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
	T* A = _elem + lo;
	int lb = mi - lo;
	T* B = new T[lb];
	for (Rank i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi;
	T* C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
	{
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
			A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] <= B[j])))
			A[i++] = C[k++];
	}
	delete[] B;
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2) return;
	int mi = (hi + lo) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
	mergeSort(lo, hi);
}
