#include "matrix.h"
#include <typeinfo>
#include <vector>
#include <utility>

void test(string msg){
  cout << "task1 says: " << msg;
}

template <class T> Matrix<T>:: Matrix(size_t row, size_t col) throw()
{
	m_row = row;
	m_col = col;
	
	try
	{
		if(row <=0 || col <=0) throw(invalid_argument( "received invalid values" ));
	
		mat_2D = make_unique< unique_ptr<T[]>[] >(row);
		unique_ptr<T[]>  smartPtr1D;

		for (int i = 0; i<row; i++)
		{
			smartPtr1D = make_unique<T[]>(col);
			for (int j = 0; j<col; j++)
			{
				smartPtr1D[j] = 0;
			}
			
			mat_2D[i] = move(smartPtr1D);
		}
	}
	catch(const invalid_argument& e )
	{
		cout<< e.what()<<endl;
		exit(-1);
	}
}

template <class T> Matrix<T>:: Matrix(size_t row, size_t col, initializer_list<T> list) throw()
{
	m_row = row;
	m_col = col;
	vector<T> v = list;
	
	try
	{
		if(row <=0 || col <=0) throw(invalid_argument( "received invalid values" ));
	
		mat_2D = make_unique< unique_ptr<T[]>[] >(row);
		unique_ptr<T[]>  smartPtr1D;
		size_t index = 0;
		
		for (int i = 0; i<row; i++)
		{
			smartPtr1D = make_unique<T[]>(col);
			for (int j = 0; j<col; j++)
			{
				smartPtr1D[j] = v.at(index);
				index++;
			}
			
			mat_2D[i] = move(smartPtr1D);
		}
	}
	catch(const invalid_argument& e )
	{
		cout<< e.what()<<endl;
		exit(-1);
	}
}

template <class T> T& Matrix<T> :: operator()(size_t row_index, size_t col_index) throw()
{
	try
	{
		if (row_index <= 0 || col_index <= 0 || row_index > m_row || col_index > m_col)
		{
			throw(invalid_argument("received invalid values" ));
		}
	
		row_index = row_index-1;
		col_index = col_index-1;
	}
	catch(invalid_argument& e)
	{
		cout << e.what() << endl;
		exit(-1);
	}
	
	return mat_2D[row_index][col_index];
}

template <class T> Matrix<T> :: Matrix(const Matrix<T>& rhs)
{
	m_row= rhs.m_row; m_col = rhs.m_col;
	
	mat_2D = make_unique< unique_ptr<T[]>[] >(m_row);
	unique_ptr<T[]>  smartPtr1D;
	
	for (int i = 0; i<m_row; i++)
	{
		smartPtr1D = make_unique<T[]>(m_col);
		
		smartPtr1D = move(rhs.mat_2D[i]);
		
		mat_2D[i] = move(smartPtr1D);
	}
	
}

template <class T> Matrix<T>& Matrix<T> :: operator+ (const Matrix<T>& l)
{
	try
	{
		if(m_row != l.m_row || m_col != l.m_col) throw(invalid_argument("invalid values"));
		
		for(size_t i = 0; i < m_row; i++)
			for(size_t j = 0; j < m_col; j++)
			{
				mat_2D[i][j] = move(mat_2D[i][j] + l.mat_2D[i][j]);
			}
	}
	catch(invalid_argument& e)
	{
		cout<< e.what()<<endl;
		exit(-1);
	}
	
	return *this;
}

template <class T> Matrix<T>& Matrix<T> :: operator* (const Matrix<T>& l)
{
	try
	{
		if(m_row != l.m_row || m_col != l.m_col || m_row != l.m_col) throw(invalid_argument("invalid values"));
		
		for (size_t r = 0; r < l.m_col; r++)
		{
			for(size_t i = 0; i < m_row; i++)
			{
				T m = 0; 
				for(size_t j = 0; j < m_col; j++) 
				{
					m += mat_2D[i][j] * l.mat_2D[j][r];
				
				}
				mat_2D[i][r] = m;
			}
		}
	}
	catch(invalid_argument& e)
	{
		cout<< e.what()<<endl;
		exit(-1);
	}
	
	return *this;
}

template <class T> T& Matrix<T> :: operator = (const T& t)
{
	return (mat_2D[row_index][col_index] = t);
}

template <class T> Matrix<T>& Matrix<T> :: operator = (const Matrix<T>& t)
{
	this->m_row = t.m_row; this->m_col = t.m_col;
	
	return *this;	
}

// g++ -std=c++14 matrix.cpp -pthread -o out

int main()
{
	
	Matrix<float> maf(2,2);
	Matrix<float> mf(2,2);
	
	Matrix<int> mi(2,2, {1,2,3,4});
	cout << mi(1,2)<<endl;
	
	mf(2,2)  = 14.0;
	maf(2,2) -= 1.0;
	mf(1,1) = 2.0;
	
	maf = maf+mf;
	Matrix<float>result(maf);
	
//	cout<< "the value is: "<< maf(2,2);  // commented because of core dump: reason is -- ownership is taken from it in copy constructor.
	cout << "the value is: "<< result(2,2)<<endl; // fine :-)
	
	maf = maf*mf;  //Watch out this, maf is out of scope from the line 190 (Matrix<float>result(maf)), you need to comment line 109 first.
	
	cout << "the value is "<< maf(1,1);
}