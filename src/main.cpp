#include <iostream>
#include <boost/multi_array.hpp>
#include <random>

#include <execution>

inline std::random_device rd;

std::ostream &operator<< (std::ostream &stream, const boost::multi_array<double, 2> &a) {
    auto size = a.shape();
    for(size_t i = 0; i < size[0]; i++) {
        for(size_t j = 0; j < size[1]; j++) {
            stream << a[i][j] << " ";
        }
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

inline constexpr boost::multi_array<double, 2> &random_fill_array(boost::multi_array<double,2> &a) {
    std::uniform_int_distribution<> dis(0, 10);
    std::mt19937 gen(rd());
    auto begin = a.data();
    auto end = a.data() + a.num_elements();
    std::generate(std::execution::par, begin, end, [&]() { return dis(gen); });
    return a;
}

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

int main(int argc, char **argv) {
    int n = argc < 2 ? 3: std::atoi(argv[1]);
    boost::multi_array<double, 2> a(boost::extents[n][n]);
    boost::multi_array<double, 2> b(boost::extents[n][n]);
    random_fill_array(a);
    random_fill_array(b);
    std::cout << "a:\n" << a << "b:\n" << b << std::endl;

    for(int i = 0; i < n; i++) {
        std::vector<double> res = multiple_square_matrix_row(a,b,i);
        std::cout << res;
    }
}

