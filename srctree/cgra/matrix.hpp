#pragma once

#include <stdexcept>
#include <vector>

namespace cgra {
    template<typename T>
    class Matrix {
        std::vector<T> m_data;
        unsigned int m_nrows;
        unsigned int m_ncols;

    public:

        Matrix(unsigned int size)
            : m_nrows(size), m_ncols(size) {
            m_data.resize(m_nrows * m_ncols);
        }

        Matrix(unsigned int numRows, unsigned int numCols)
            : m_nrows(numRows), m_ncols(numCols) {
            m_data.resize(m_nrows * m_ncols);
        }

        unsigned int numRows() const {
            return m_nrows;
        }

        unsigned int numCols() const {
            return m_ncols;
        }

        void setRow(unsigned int row, std::initializer_list<T> list) {
            T *r = (*this)[row];
            unsigned int n = 0;

            for (T val : list) {
                r[n] = val;
                n++;
                if (n >= m_ncols)
                    break;
            }

            for (; n < m_ncols; n++) {
                r[n] = T();
            }
        }

        const T* operator[](unsigned int row) const {
            if (row >= m_nrows) {
                throw std::out_of_range("row out of range");
            }
            return &m_data[row * m_ncols];
        }

        T* operator[](unsigned int row) {
            if (row >= m_nrows) {
                throw std::out_of_range("row out of range");
            }
            return &m_data[row * m_ncols];
        }

        const T & operator()(unsigned int row, unsigned int col) const {
            if (row >= m_nrows) {
                throw std::out_of_range("row out of range");
            }
            if (col >= m_ncols) {
                throw std::out_of_range("column out of range");
            }
            unsigned int idx = (row * m_ncols) + col;

            return m_data[idx];
        }

        T & operator()(unsigned int row, unsigned int col) {
            if (row >= m_nrows) {
                throw std::out_of_range("row out of range");
            }
            if (col >= m_ncols) {
                throw std::out_of_range("column out of range");
            }
            unsigned int idx = (row * m_ncols) + col;

            return m_data[idx];
        }
    };
}
