#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>

#ifdef _OPENMP
  #include <omp.h>
#else
  // Si no hay OpenMP, correrá en 1 hilo
  inline int  omp_get_thread_num() { return 0; }
  inline int  omp_get_num_threads() { return 1; }
  inline void omp_set_num_threads(int) {}
#endif

using namespace std;

static void print_sample(const vector<int>& A,
                         const vector<int>& B,
                         const vector<int>& R,
                         size_t k = 10)
{
    size_t n = A.size();
    k = min(k, n);

    cout << "\nPrimeros " << k << " elementos:\n";
    cout << " i |  A |  B |  R\n";
    cout << "------------------\n";
    for (size_t i = 0; i < k; i++) {
        cout << setw(2) << i << " | "
             << setw(2) << A[i] << " | "
             << setw(2) << B[i] << " | "
             << setw(2) << R[i] << "\n";
    }

    if (n > k) {
        cout << "\nUltimos " << k << " elementos:\n";
        cout << " i |  A |  B |  R\n";
        cout << "------------------\n";
        for (size_t i = n - k; i < n; i++) {
            cout << setw(2) << i << " | "
                 << setw(2) << A[i] << " | "
                 << setw(2) << B[i] << " | "
                 << setw(2) << R[i] << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t N;
    int nHilos;

    cout << "Cantidad de elementos N : ";
    cin >> N;

    cout << "Cantidad de hilos : ";
    cin >> nHilos;

#ifdef _OPENMP
    if (nHilos > 0) omp_set_num_threads(nHilos);
#endif

    vector<int> A(N), B(N), R(N);

    std::mt19937 rng(12345); // semilla fija para reproducibilidad
    std::uniform_int_distribution<int> dist(0, 100);

    for (size_t i = 0; i < N; i++) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

#pragma omp parallel for schedule(static)
    for (long long i = 0; i < (long long)N; i++) {
        R[(size_t)i] = A[(size_t)i] + B[(size_t)i];
    }

#ifdef _OPENMP
#pragma omp parallel
    {
#pragma omp single
        cout << "\nOpenMP activo. Hilos usados: " << omp_get_num_threads() << "\n";
    }
#else
    cout << "\nOpenMP NO activo. Corriendo en 1 hilo.\n";
#endif

    // print
    print_sample(A, B, R, 10);

    return 0;
}