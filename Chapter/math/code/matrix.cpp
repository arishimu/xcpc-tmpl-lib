template <class T>
struct Grid {
    int n, m;
    std::vector<std::vector<T>> A;
    Grid(int n) : Grid(n, n) {}
    Grid(int n, int m) : n(n), m(m), A(n, std::vector<T>(m)) {}
    Grid(std::vector<std::vector<T>> const& B) : n(B.size()), m(B[0].size()), A(B) {}
    auto operator[](int i) -> std::vector<T> & { return A[i]; }
    auto operator[](int i) const -> std::vector<T> const& { return A[i]; }
};

template <class T, class Mat>
struct Matrix : Grid<T> {

    using Grid<T>::n, Grid<T>::m, Grid<T>::A, Grid<T>::Grid;
    explicit Matrix(int n) : Grid<T>(n) { for (int i = 0; i < n; i++) A[i][i] = 1; }

    Mat operator*(Mat const& B) const {
        auto that = static_cast<Mat const*>(this);
        Mat C = *that; return C *= B;
    }

    Mat& operator*=(Mat const& B) {
        auto that = static_cast<Mat*>(this);
        Mat C(n, B.m);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < m; k++)
                for (int j = 0; j < B.m; j++)
                    C[i][j] += A[i][k] * B[k][j];
        return *that = std::move(C);
    }

    Mat pow(i64 k) const {
        auto that = static_cast<Mat const*>(this);
        Mat result(n), base(*that);
        for (; k; k >>= 1, base *= base) if (k & 1) result *= base;
        return result;
    }

};

struct FMat : Matrix<double, FMat> {
    using Base = Matrix<double, FMat>;
    using Base::Base;
};

struct ZMat : Matrix<Z, FMat> {
    using Base = Matrix<Z, FMat>;
    using Base::Base;
};