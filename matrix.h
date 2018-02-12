#include <iostream>
#include <memory>
#include <exception>

using namespace std;

template<class T> class Matrix
{
	private:
		size_t m_row, m_col;
		size_t row_index, col_index;
		unique_ptr<unique_ptr<T []>[]> mat_2D;
		
	public:
		Matrix(size_t row, size_t col) throw();
		Matrix(size_t row, size_t col, initializer_list<T> list) throw();
		Matrix():mat_2D(nullptr) { cout<< "Default constructor is called.\n"; }
		Matrix(const Matrix<T> & rhs); // copy constructor
		
		T& operator()(size_t row_index, size_t col_index) throw();
		T& operator =(const T& t); 
		Matrix<T>& operator =(const Matrix<T>& t );
		Matrix<T>& operator +(const Matrix<T>& m);
		Matrix<T>& operator *(const Matrix<T>& m);
		
};