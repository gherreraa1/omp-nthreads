#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>

#ifdef _OPENMP
  #include <omp.h>
#else
  inline int omp_get_thread_num() { return 0; }
  inline void omp_set_num_threads(int) {}
#endif

using namespace std;

int main() {
    size_t N;
    int nHilos;

    cout << "Cantidad de elementos N: ";
    cin >> N;

    cout << "Cantidad de hilos: ";
    cin >> nHilos;

#ifdef _OPENMP
    omp_set_num_threads(nHilos);
#endif

    vector<int> A(N), B(N), R(N);

    mt19937 rng(123);
    uniform_int_distribution<int> dist(1, 50);

    for (size_t i = 0; i < N; i++) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

#pragma omp parallel for schedule(static)
    for (long long i = 0; i < (long long)N; i++) {

        int tid = omp_get_thread_num();
        R[i] = A[i] + B[i];

        #pragma omp critical
        {
            cout << "Hilo " << tid
                 << " -> R[" << i << "] = "
                 << A[i] << " + " << B[i]
                 << " = " << R[i] << "\n";
        }
    }

    return 0;
}
