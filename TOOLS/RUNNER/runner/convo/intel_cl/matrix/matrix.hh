#pragma once

/*all the options*/
#define USE_ONLY_CPU /*this is for testing*/ true


#include <vector>
#include <stdlib.h>
#include <string>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


namespace ACCELERATED
{
    class Matrix {
        public: 
        std::vector<std::vector<float>> matrix_data;
        size_t cols, rows;

        Matrix(const std::vector<float> data,
               const size_t m_rows,
               const size_t m_cols);

        Matrix(const float fill_value, const size_t m_rows, const size_t m_cols);

        const Matrix 
        operator+(const Matrix& other);

        const Matrix
        operator*(const Matrix& other);

        const std::string 
        to_string();

        void operator=(const Matrix other);
    }; 
}