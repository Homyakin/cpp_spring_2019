#pragma once
#include <cstddef>
#include <stdexcept>


struct Matrix
{
	Matrix(size_t r, size_t c) : rows(r), columns(c)
	{
		matrix = new int[rows * columns];
	}
	Matrix(const Matrix& other): rows(other.getRows()), columns(other.getRows())
	{
		matrix = new int[rows * columns];
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < columns; ++j)
				matrix[i * columns + j] = other[i][j];
	}
	Matrix& operator=(const Matrix& other)
	{
		if(this != &other)
			delete[] matrix;
			rows = other.getRows();
			columns = other.getRows();
			matrix = new int[rows * columns];
			for (size_t i = 0; i < rows; ++i)
				for (size_t j = 0; j < columns; ++j)
					matrix[i * columns + j] = other[i][j];
		return *this;
	}
	size_t getRows() const { return rows; }
	size_t getColumns() const { return columns; }
private:
	struct Proxy;
public:
	Proxy operator[](size_t row)
	{
		if (row >= rows || row < 0)
			throw std::out_of_range("Wrong row");
		return Proxy(matrix, row, columns);
	}
	const Proxy operator[](size_t row) const
	{
		if (row >= rows || row < 0)
			throw std::out_of_range("Wrong row");
		return Proxy(matrix, row, columns);
	}
	Matrix& operator*=(int a)
	{
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < columns; ++j)
				matrix[i * columns + j] *= a;
		return *this;
	}
	Matrix operator*(int a)
	{
		Matrix temp(*this);
		temp *= a;
		return temp;
	}
	bool operator==(const Matrix& other)
	{
		if (this == &other)
			return true;
		if (rows != other.getRows() || columns != other.getColumns())
			return false;
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < columns; ++j)
				if (matrix[i * columns + j] != other[i][j])
					return false;
		return true;
	}
	bool operator!=(const Matrix& other)
	{
		return !(*this == other);
	}
	~Matrix()
	{
		delete[] matrix;
	}

private:
	size_t rows;
	size_t columns;
	int* matrix;

	struct Proxy
	{
		Proxy(int* t, size_t r, size_t mc) : matrix(t), row(r), max_column(mc) {}

		int& operator[](size_t column)
		{
			if (column >= max_column || column < 0)
				throw std::out_of_range("Wrong column");
			return matrix[row * max_column + column];
		}
		int operator[](size_t column) const
		{
			if (column >= max_column || column < 0)
				throw std::out_of_range("Wrong column");
			return matrix[row * max_column + column];
		}

	private:
		int* matrix;
		size_t row;
		size_t max_column;
	};
};
