#include <iostream>
#include <boost/multi_array.hpp>
#include <random>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <execution>
#include <sys/wait.h>
#include "consts.h"

std::random_device rd;

std::ostream &operator<< (std::ostream &stream, const boost::multi_array<double, 2> &a) {
    auto size = a.shape();
    for(size_t i = 0; i < size[0]; i++) {
        stream << "\t| ";
        for(size_t j = 0; j < size[1]; j++) {
            stream << a[i][j] << " ";
        }
        stream << " |";
        stream << "\n";
    }
    return stream;
}

std::ostream &operator<< (std::ostream &stream, const std::vector<double> &v) {
    for(size_t i = 0; i < v.size(); ++i) {
        stream << v[i] << " ";
    }
    stream << std::endl;
    return stream;
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

inline boost::multi_array<double, 2> &random_fill_array(boost::multi_array<double,2> &a) {
    std::uniform_int_distribution<> dis(0, 10);
    std::mt19937 gen(rd());
    auto begin = a.data();
    auto end = a.data() + a.num_elements();
    std::generate(std::execution::par, begin, end, [&]() { return dis(gen); });
    return a;
}

double *fill_shared_mem(double* ptr, boost::multi_array<double,2> &a) {
    auto begin = a.data();
    auto end = a.data() + a.num_elements();
    std::copy(begin, end, ptr);
    return ptr + a.num_elements();
}


int main(int argc, char **argv) {
    int n = argc < 2 ? 3: std::atoi(argv[1]);
    int shm_fd;
    double *ptr, *new_ptr;
    std::string n_str, i_str;

    boost::multi_array<double, 2> a(boost::extents[n][n]);
    boost::multi_array<double, 2> b(boost::extents[n][n]);
    random_fill_array(a);
    random_fill_array(b);

    std::cout << "a:\n" << a << "b:\n" << b << std::endl;

    shm_fd = shm_open(SHARED_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(double) * n * n * 3);
    ptr = static_cast<double*>(mmap(0, sizeof(double) * n * n * 3, PROT_WRITE, MAP_SHARED, shm_fd, 0));
    new_ptr = fill_shared_mem(ptr, a);
    new_ptr = fill_shared_mem(new_ptr, b);

    int pids[n];
    n_str = std::to_string(n);
    char *args[] = {"mul", n_str.data(), NULL, NULL};
    for(int i = 0; i < n; ++i) {
        i_str = std::to_string(i);
        args[2] = i_str.data();
        int pid = fork();
        if(pid == 0) std::cout << execvp("./bin/mul", args);
        pids[i] = pid;
    }

    for (int i = 0; i < n; ++i) {
        int status;
        pid_t pid = waitpid(pids[i], &status, 0);
        if (pid > 0) {
            printf("Родительский процесс: дочерний процесс %d завершен\n", pid);
        }
    }
    printf("Все дочерние процессы завершены\n");

    auto result = read_matrix_from_shared(new_ptr, n);
    std::cout << result << std::endl;

    munmap(ptr, sizeof(double) * n * n * 3);
    close(shm_fd);
}

