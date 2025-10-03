#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <memory>
#include <ostream>
#include <type_traits>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include <ctime>
#include <printutils.hpp>
#include <random>

inline double log_base(double a, double base) {
    return std::log(a) / std::log(base);
}

namespace Vector {

    // Forward declarations
    template <typename T>
    class RVector;
    template <typename T>
    RVector<T> RandRVector(const uint32_t n, const T min, const T max);

    // Definitions follow from here
    template <typename T>
    class RVector {
    public:
        bool integral = std::is_integral<T>::value;
        std::uint32_t size;
        std::string value_type =  typeid(T).name();
    private:
        std::unique_ptr<T[]> v;
        template <typename U>
        friend RVector<U> RandRVector(const uint32_t n, const U min, const U max);
    public:
        // Operator overloads
        inline T& operator[](const unsigned int i) {
            if (i<0 || i>=size) {
                throw("Rvector subscript out of bounds");
            }
                return v[i];
            }

        inline const T& operator[](const std::size_t i) const {
            #ifdef NO_CHECKBOUNDS
            return v[i];
            #else
            if (i<0 || i>=size) {
                throw("Rvector subscript out of bounds");
            }
            return v[i];
            #endif
        }

        friend inline std::ostream& operator<<(std::ostream& os, const RVector<T>& vec) {
                for (uint32_t i = 0 ; i<vec.size ; i++) {
                    os << vec[i] << ' ';
            }
            os << std::endl;
            return os;
        }

        // Constructor methods
        RVector<T>() : size(0),v(NULL) {};

        RVector<T>(unsigned int N) : size(N), v(std::make_unique<T[]>(N)) {
            for (std::uint32_t i = 0; i<size; i++) {
                v[i] = 0;
            }
        };

        RVector<T>(unsigned int N, T a) : size(N), v(std::make_unique<T[]>(N))  {
            for (std::uint32_t i = 0; i<size; i++) {
                v[i] = a;
            }
        };

        RVector<T>(const RVector<T> &rhs) : size(rhs.size) , v(std::make_unique<T[]>(rhs.size)) {
            for (std::uint32_t i =0 ; i<size ; i++) {
                v[i] = rhs[i];
            }
        }

        RVector<T>(uint32_t N, const T* arr) : size(N), v(std::make_unique<T[]>(N)) {
            for (uint32_t i = 0 ; i < N ; i++) {
                v[i] = arr[i];
            }
        }

        // Member functions
        T sum() {
            T value = 0;
            for (std::uint32_t i = 0 ; i<size ; i++){
                value += v[i];
            }
            return value;
        }

        double L2norm() {
            double norm = 0.0;
            for (std::uint32_t i =0 ; i< size ; i++) {
                norm += v[i] * v[i];
            }
            norm = std::sqrt(norm);
            return norm;
        }

        RVector<T> sort() {
            RVector<T> copy_vec(*this);
            std::sort(copy_vec.v.get(), copy_vec.v.get() + copy_vec.size);
            return copy_vec;
        }

        T max() {
            T maxval = v[0];
            for (uint32_t i = 1 ; i < size ; i++) {
                if (v[i] > maxval) {
                    maxval = v[i];
                }
            }
            return maxval;
        }

        T min() {
            T minval = v[0];
            for (uint32_t i = 1 ; i < size ; i++) {
                if (v[i] < minval) {
                    minval = v[i];
                }
            }
            return minval;
        }

        inline T* data_ptr() {
            return v.get();
        }

        template<typename U>
        bool isclose(const RVector<U> &rhs, double atol = 0 , double rtol = 1e-3) {
        // Check integral nature at compile time
            if constexpr ( std::is_integral<T>::value && std::is_integral<U>::value){
            // Check type matching at compile time
                // Check if both vectors are of same sub-integral type
                if constexpr (std::is_same<T, U>::value) {
                    // Check sizes at run time
                    if (this->size == rhs.size) {
                        for (std::uint32_t i = 0 ; i< size ; i++) {
                            const T a = this->v[i];
                            const U b = rhs[i];
                            if (a != b){
                                return false;
                            }
                        }
                        return true;
                    }
                    else { throw("Cannot compare RVectors of different sizes");}
                }
                // if not cast both the integral types to int
                else {
                    for (std::uint32_t i = 0 ; i< size ; i++) {
                        const T a = static_cast<int>(this->v[i]);
                        const U b = static_cast<int>(rhs[i]);
                        io_utils::print(a,' ',b);
                        if (a != b){
                            return false;
                        }
                    }
                    return true;
                }
            }
            // if types are very different convert both to double
            else {
                for (uint32_t i =0 ; i< size ; i++){
                    const double a = (double)(this->v[i]);
                    const double b = (double)(rhs[i]);
                    if (std::abs(a - b) > atol + rtol * b){
                        return false;
                        }
                }
                return true;
            }
        }
    };

    template <typename T>
    RVector<T> RandRVector(const uint32_t n, const T min, const T max){
        RVector<T> vec(n);
        std::random_device rd;  // Obtain a random seed
        std::mt19937 gen(rd()); // Mersenne Twister random number engine seeded with rd
        std::uniform_int_distribution<> distrib(min, max); // Uniform distribution between min and max inclusive
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<> distrib(min, max); // Uniform distribution between min and max inclusive
            for (uint32_t i = 0 ; i < n ; i++){
                vec.v[i]= distrib(gen);
            }
        }
        else {
            std::uniform_real_distribution<> distrib(min, max); // Uniform distribution between min and max inclusive
            for (uint32_t i = 0 ; i < n ; i++){
                vec.v[i]= distrib(gen);
            }
        }
    return vec;
    };

    RVector<double> LinSpace(const double min, const double max, const uint32_t n){
        RVector<double> vec(n);
        double spacing = ( max - min ) / (n-1);
            for (uint32_t i = 0 ; i < n ; i++){
                vec[i] = min + i * spacing;
            }
    return vec;
    };

    RVector<double> GeomSpace(const double min, const double max, const uint32_t n, const double base){

        double logmin = log_base(min, base);
        double logmax = log_base(max, base);
        RVector<double> vec(n);
        double spacing = ( logmax - logmin ) / (n-1);
            for (uint32_t i = 0 ; i < n ; i++){
                double _exp = logmin + i * spacing;
                vec[i] = std::pow(base, _exp);
            }
        return vec;
    };

    template <typename T>
    RVector<T> Arange(const T min, const T max, T delta){
        uint32_t n = ( max - min ) / delta;
        RVector<T> vec(n);
            for (uint32_t i = 0 ; i < n ; i++){
                vec[i] = min + i * delta;
            }
    return vec;
    };

};


#ifdef DEBUG
int main() {
    Vector::RVector<double> nullvec ;
    assert(nullvec.size == 0);

    // Checl zero vector
    Vector::RVector<unsigned int> zerovec(3);
    assert(zerovec[0]==0);
    Vector::RVector<double> zerovecD(3);
    assert(zerovecD[0]==0.0);

    // Check if copying worked
    std::random_device rd;  // Obtain a random seed
    std::mt19937 gen(rd()); // Mersenne Twister random number engine seeded with rd
    std::uniform_int_distribution<> distrib(3, 11); // Uniform distribution between min and max inclusive
    int rn = distrib(gen);

    Vector::RVector<int> randomSize(rn,1) ;
    Vector::RVector<int> copyRandomSize(randomSize);
    Vector::RVector<int>* prandSize = &randomSize;
    Vector::RVector<int>* pCopyRandSize = &copyRandomSize;
    assert(randomSize.size == copyRandomSize.size);
    assert(prandSize != pCopyRandSize);

    // Norm checker
    Vector::RVector<int> normchecker(3, 1);
    assert(std::abs(1.0-normchecker.L2norm()/std::sqrt(3))<1e-3);

    // isclose checker
        // int-int
        Vector::RVector<int> vecint(3,1);
        Vector::RVector<int> copyvecint(vecint);
        vecint.isclose(copyvecint);
        // int-double
        Vector::RVector<double> vecdoub(3,1.0);
        assert(vecint.isclose(vecdoub));
        Vector::RVector<double> copyvecdoub(vecdoub);
        // double-double
        vecdoub.isclose(copyvecdoub);

    // testing random vector generator
        auto randVecInt = Vector::RandRVector(3, 1 , 3);
        auto randVecReal = Vector::RandRVector(3, 1.0 , 3.0);
        // std::cout << randVecInt;

    // testing LinSpace
        auto linspaceVec = Vector::LinSpace(1.0, 4.0, 4);
        std::cout << linspaceVec;

    // testing Arange
        auto arangeVec = Vector::Arange(1,3,1);
        std::cout << arangeVec << '\n';
        std::cout << arangeVec.data_ptr() << std::endl;

    // testing GeomSpace
        auto geomSpaceVec = Vector::GeomSpace(1.0, 1000.0,3,10.0);
        std::cout << geomSpaceVec;

    // testing sorting
        int temp[5] = {5,3,2,1,-1};
        Vector::RVector<int> tosortVec(5,temp);
        auto sorted = tosortVec.sort();
        assert(sorted[0]==-1);
    return 0;
}
#endif
