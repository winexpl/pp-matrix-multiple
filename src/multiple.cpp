#include <vector>
#include <iostream>
#include <boost/multi_array.hpp>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "consts.h"

inline std::vector<double> multiple_square_matrix_row(const boost::multi_array<double, 2> &a, const boost::multi_array<double, 2> &b, size_t row) {
    auto size = a.shape();
    std::vector<double> result(size[0], 0);
    for (size_t i = 0; i < size[0]; ++i) {
        for (size_t j = 0; j < size[1]; ++j) {
            result[i] += a[row][j] * b[j][i];
        }
    }
    return std::move(result);
}

inline boost::multi_array<double, 2> read_matrix_from_shared(double* ptr, int n) {
    boost::multi_array<double, 2> a(boost::extents[n][n]);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = ptr[i * n + j];
        }
    }
    return std::move(a);
}

inline void write_row_to_shared(double* ptr, std::vector<double> &a, int row) {
    int n = a.size();
    ptr += row * a.size() + n * n * 2;
    auto begin = a.begin();
    auto end = a.end();
    std::copy(begin, end, ptr);
}

int main(int argc, char** argv) {
    int shm_fd;
    double *ptr;

    int n = (argc > 1) ? std::atoi(argv[1]) : 0;
    int row = (argc > 2) ? std::atoi(argv[2]) : 0;

    shm_fd = shm_open(SHARED_NAME, O_RDWR, 0644);
    ptr = static_cast<double*>(mmap(0, sizeof(double) * n * n * 3, PROT_WRITE, MAP_SHARED, shm_fd, 0));

    auto a_matrix_ptr = ptr + n * row;
    auto b_matrix_ptr = ptr + n * n;
    auto c_matrix_ptr = ptr + 2 * n * n + n * row;
    for(int i = 0; i < n; i++) {
        double elem = 0;
        auto c_matrix_current_element_ptr = c_matrix_ptr + i;
        *c_matrix_current_element_ptr = 0;
        for(int j = 0; j < n; j++) {
            auto a_matrix_current_element_ptr = a_matrix_ptr + j;
            auto b_matrix_current_element_ptr = b_matrix_ptr + j * n + i;
            *c_matrix_current_element_ptr += *a_matrix_current_element_ptr * *b_matrix_current_element_ptr;
        }
    }
    munmap(ptr, sizeof(double) * n * n * 3);
}