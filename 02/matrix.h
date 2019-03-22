#pragma once
#include <cstddef>
#include <stdexcept>


struct Matrix
{
	Matrix(size_t r, size_t c) : rows(r), columns(c)
	{
		m = new int[rows * columns];
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
		try
		{
			return Proxy(m, row, columns);
		}
		catch (std::out_of_range& t)
		{
			throw t;
		}
	}
	const Proxy operator[](size_t row) const
	{
		if (row >= rows || row < 0)
			throw std::out_of_range("Wrong row");
		try
		{
			return Proxy(m, row, columns);
		}
		catch (std::out_of_range& t)
		{
			throw t;
		}
	}
	Matrix& operator*=(int a)
	{
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < columns; ++j)
				m[i * columns + j] *= a;
		return *this;
	}
	bool operator==(const Matrix& other)
	{
		if (this == &other)
			return true;
		if (rows != other.getRows() || columns != other.getColumns())
			return false;
		for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < columns; ++j)
				if (m[i * columns + j] != other[i][j])
					return false;
		return true;
	}
	bool operator!=(const Matrix& other)
	{
		return !(*this == other);
	}

private:
	size_t rows;
	size_t columns;
	int* m;

	struct Proxy
	{
		Proxy(int* t, size_t r, size_t mc) : m(t), row(r), max_column(mc) {}

		int& operator[](size_t column)
		{
			if (column >= max_column || column < 0)
				throw std::out_of_range("Wrong column");
			return m[row * max_column + column];
		}
		int operator[](size_t column) const
		{
			if (column >= max_column || column < 0)
				throw std::out_of_range("Wrong column");
			return m[row * max_column + column];
		}

	private:
		int* m;
		size_t row;
		size_t max_column;
	};
};
