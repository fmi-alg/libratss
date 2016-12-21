#ifndef LIB_RATSS_INTERNAL_MATRIX_H
#define LIB_RATSS_INTERNAL_MATRIX_H
#pragma once

#include <libratss/constants.h>
#include <stdexcept>

namespace LIB_RATSS_NAMESPACE {
namespace internal {

template<typename TVALUE>
class Matrix {
public:
	using value_type = TVALUE;
public:
	Matrix(std::size_t dimension);
	Matrix(const Matrix & other);
	Matrix(Matrix && other);
	Matrix & operator=(const Matrix & other);
	Matrix & operator=(Matrix && other);
	~Matrix();
	Matrix operator*(const Matrix< TVALUE >& other) const;
	
	value_type & operator()(std::size_t row, std::size_t column);
	const value_type & operator()(std::size_t row, std::size_t column) const;
public:
	static Matrix identity(std::size_t dimension);
public:
	inline std::size_t dimension() const { return m_dimension; }
private:
	std::size_t m_dimension;
	std::vector<TVALUE> m_d;
};

}}//end namespace LIB_RATSS_NAMESPACE

namespace LIB_RATSS_NAMESPACE {
namespace internal {

template<typename TVALUE>
Matrix<TVALUE>::Matrix(std::size_t dimension) :
m_dimension(dimension),
m_d(dimension*dimension, value_type(0))
{}

template<typename TVALUE>
Matrix<TVALUE>::Matrix(const Matrix & other) :
m_dimension(other.m_dimension),
m_d(other.m_d) {}

template<typename TVALUE>
Matrix<TVALUE>::Matrix(Matrix && other) :
m_dimension(other.m_dimension),
m_d(std::move(other.m_d) )
{}

template<typename TVALUE>
Matrix<TVALUE> &
Matrix<TVALUE>::operator=(const Matrix & other) {
	m_dimension = other.m_dimension;
	m_d = other.m_d;
}

template<typename TVALUE>
Matrix<TVALUE> &
Matrix<TVALUE>::operator=(Matrix && other) {
	m_dimension = other.m_dimension;
	m_d = std::move(other.m_d);
}

template<typename TVALUE>
Matrix<TVALUE>::~Matrix()
{}

template<typename TVALUE>
Matrix<TVALUE>
Matrix<TVALUE>::operator*(const Matrix<TVALUE> & right) const
{
	if (dimension() != right.dimension()) {
		throw std::domain_error("ratss::internal::Matrix: multipyling matrices of different dimensions is not supported");
	}

	Matrix result(dimension());
	const Matrix & left = *this;

	for(std::size_t row(0); row < dimension(); ++row) {
		for(std::size_t col(0); col < dimension(); ++col) {
			for(std::size_t k(0); k < dimension(); ++k) {
				result(row, col) += left(row, k) * right(k, col);
			}
		}
	}
	return result;
}

template<typename TVALUE>
typename Matrix<TVALUE>::value_type &
Matrix<TVALUE>::operator()(std::size_t row, std::size_t column)
{
	return m_d.at(dimension()*row+column);
}

template<typename TVALUE>
const typename Matrix<TVALUE>::value_type &
Matrix<TVALUE>::operator()(std::size_t row, std::size_t column) const
{
	return m_d.at(dimension()*row+column);
}

template<typename TVALUE>
Matrix<TVALUE>
Matrix<TVALUE>::identity(std::size_t dimension)
{
	Matrix result(dimension);
	for(std::size_t i(0); i < dimension; ++i) {
		result(i, i) = value_type(1);
	}
	return result;
}


}}//end namespace LIB_RATSS_NAMESPACE

#endif