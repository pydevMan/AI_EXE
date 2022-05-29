#include "matrix.hh"
#include "setup_opencl.hh"
#include <iostream>
using namespace ACCELERATED;


Matrix::Matrix(const std::vector<float> data,
               const size_t m_rows,
               const size_t m_cols){
    matrix_data.resize(m_cols);
    cols = m_rows;
    rows = m_cols;
    size_t acc_j = 0; 
    for (size_t i = 0; i < m_cols; ++i){
        for (size_t j = 0; j < m_rows; ++j){
            (m_rows > m_cols) 
                ?matrix_data[i].push_back(data[i+j+acc_j*(
                (m_rows >= m_cols)
                ? m_cols
                : m_rows)])
            :matrix_data[i].push_back(data[j+acc_j*(
                (m_rows >= m_cols)
                ? m_cols
                : m_rows)]);
        }
        ++acc_j;
    }
    return;
}


Matrix::Matrix(const float fill_value, const size_t m_rows, const size_t m_cols){
    matrix_data.resize(m_cols);
    cols = m_rows;
    rows = m_cols;
    for (size_t i = 0; i < m_cols; ++i){
        for (size_t j = 0; j < m_rows; ++j){
            matrix_data[i].push_back(fill_value);
        }
    }
    return;
}

const std::string 
Matrix::to_string()
{
    std::string out = "";

    for (auto &i: matrix_data){
        out += "[";
        for (auto &j: i){
            out += " " + std::to_string(j);
        }
        out += " ]\n";
    }

    return out;
}


const Matrix Matrix::operator+(const Matrix& other)
{
    #ifndef USE_ONLY_ACCELERATOR
    #else

    if (rows != other.rows || cols != other.cols) perror("Matrix dimensions do not match");

    Matrix out = Matrix(0, cols, rows);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            out.matrix_data[i][j] = matrix_data[i][j] + other.matrix_data[i][j];
        }
    }

    return out;

    #endif
}



const Matrix Matrix::operator*(const Matrix& other){
    #ifndef USE_ONLY_ACCELERATOR
    //dping this with open cl
    size_t M=0, N=0, K=0;

    (rows == other.rows) && (cols == other.cols)
        ? (M = rows, N = cols, K = other.cols)
        : (rows == other.cols)
            ? (M = other.rows, N = cols, K = rows)
            : (cols == other.rows)
                ? (M = rows, N = other.cols, K = cols)
                : (M = other.rows, N = other.cols, K = rows);
                
    //use the mat_mull kernak


    float ** matA_ptr = (float **)matrix_data.data();
    float ** matB_ptr = (float **)matrix_data.data();
    float ** matC_ptr[M][N];

    cl_mem A = clCreateBuffer(context, CL_MEM_READ_ONLY, rows*cols*sizeof(float), matA_ptr, NULL);

    cl_mem B = clCreateBuffer(context, CL_MEM_READ_ONLY, other.rows*other.cols*sizeof(float), matB_ptr, NULL);    

    cl_mem C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, N*M*sizeof(float), matC_ptr, NULL);

    clSetKernelArg(matrix_mull_kernel, 0, sizeof(A), A);

    clSetKernelArg(matrix_mull_kernel, 1, sizeof(B), B);

    clSetKernelArg(matrix_mull_kernel, 0, sizeof(C), C);

    clEnqueueNDRangeKernel(command_queue, matrix_mull_kernel, M*N, 0, (size_t*) N, (size_t*)M, K, NULL, NULL);




    //load the a matrix


    #else
    //vector in k, n, m form
    //find the repeating dimentioin k 
    //find the little dimension n in the first matrix
    //find the big dimension m in the seccond matrix

    size_t M=0, N=0, K=0;

    (rows == other.rows) && (cols == other.cols)
        ? (M = rows, N = cols, K = other.cols)
        : (rows == other.cols)
            ? (M = other.rows, N = cols, K = rows)
            : (cols == other.rows)
                ? (M = rows, N = other.cols, K = cols)
                : (M = other.rows, N = other.cols, K = rows);

    auto out = Matrix(0, N, M);


    for (size_t i = 0; i < M; i++){
        for (size_t j = 0; j < N; j++){
            for (size_t k = 0; k < K; k++){
                out.matrix_data[i][j] += matrix_data[i][k] * other.matrix_data[k][j];
            }
        }
    }

    return out;
    #endif
}

void Matrix::operator=(const Matrix other){
    matrix_data = other.matrix_data;
    rows = other.rows;
    cols = other.cols;
}