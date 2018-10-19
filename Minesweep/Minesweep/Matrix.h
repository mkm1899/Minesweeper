#ifndef MATRIX
#define MATRIX

#include<fstream>

template<class T>
class Matrix
{
public:
	Matrix(int r, int c);
	~Matrix();
	T* get(int r, int c);
	void set(int r, int c, T& x);
	int getRowSize();
	int getColSize();

private:
	T * d2array;
	int colSize;
	int rowSize;
};



template<class T>
Matrix<T>::Matrix(int r, int c)
{
	d2array = new T[r*c];
	colSize = c;
	rowSize = r;
}

template<class T>
Matrix<T>::~Matrix()
{
	delete[] d2array;
}

template<class T>
T* Matrix<T>::get(int r, int c)
{
	if (r >= rowSize || c >= colSize) {
		return NULL;
	}
	return &(d2array[r*colSize + c]);
}

template<class T>
inline void Matrix<T>::set(int r, int c, T& x)
{
	d2array[r*colSize + c] = x;
}

template<class T>
int Matrix<T>::getRowSize()
{
	return rowSize;
}

template<class T>
inline int Matrix<T>::getColSize()
{
	return colSize;
}

#endif // !MATRIX