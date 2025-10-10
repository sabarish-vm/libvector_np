#pragma once
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
#include <random>

#ifndef REAL_PRECISION
#define REAL_PRECISION 1
#endif

#if REAL_PRECISION == 1
    #define REAL_TYPE double
#elif REAL_PRECISION ==2
    #define REAL_TYPE long double
#endif

using Real = REAL_TYPE;
inline Real log_base(Real a, Real base) {
    return std::log(a) / std::log(base);
}

template <typename A, typename B>
struct RealOrInt{
using type = std::conditional_t<std::is_same_v<A, Real>, Real,
                            std::conditional_t<std::is_same_v<B, Real>, Real, int>>;
};

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

        // begin and end methods for using ranged loop
        T* begin() {return v.get();};
        T* end() {return (v.get() + size);};

        const T* begin() const {return v.get();};
        const T* end() const {return (v.get() + size);};
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

        template<typename U>
        RVector<typename RealOrInt<T, U>::type> operator+(const U& scalar) {
            RVector<typename RealOrInt<T,U>::type> res(size);
            for (uint32_t i =0.0; i<size; i++) {
                res[i] = v[i] + scalar;
            }
            return res;
        }
        template<typename U>
        RVector<typename RealOrInt<T, U>::type> operator+(const RVector<U>& vec) {
            if(size==vec.size) {
                RVector<typename RealOrInt<T,U>::type> res(size);
                for (uint32_t i =0.0; i<size; i++) {
                    res[i] = v[i] + vec[i];
                }
                return res;
            }
            else {
                throw("Sizes of vectors do not match");
            }
        }

        template<typename U>
        RVector<typename RealOrInt<T, U>::type> operator-(const U& scalar) {
            RVector<typename RealOrInt<T,U>::type> res(size);
            for (uint32_t i =0.0; i<size; i++) {
                res[i] = v[i] - scalar;
            }
            return res;
        }
        template<typename U>
        RVector<typename RealOrInt<T, U>::type> operator-(const RVector<U>& vec) {
            if(size==vec.size) {
                RVector<typename RealOrInt<T,U>::type> res(size);
                for (uint32_t i =0.0; i<size; i++) {
                    res[i] = v[i] - vec[i];
                }
                return res;
            }
            else {
                throw("Sizes of vectors do not match");
            }
        }

        template<typename U>
        RVector<Real> operator/(const U& scalar) {
            RVector<typename RealOrInt<T,U>::type> res(size);
            for (uint32_t i =0.0; i<size; i++) {
                res[i] = (Real)v[i] / (Real)scalar;
            }
            return res;
        }
        template<typename U>
        RVector<Real> operator/(const RVector<U>& vec) {
            if(size==vec.size) {
                RVector<Real> res(size);
                for (uint32_t i =0.0; i<size; i++) {
                    res[i] = Real(v[i]) / Real(vec[i]);
                }
                return res;
            }
            else {
                throw("Sizes of vectors do not match");
            }
        }


        template<typename U>
        RVector<typename RealOrInt<T, U>::type> operator*(const U& scalar) {
            RVector<typename RealOrInt<T,U>::type> res(size);
            for (uint32_t i =0.0; i<size; i++) {
                res[i] = v[i] * scalar;
            }
            return res;
        }
        template<typename U>
        RVector<typename RealOrInt<T, U>::type> operator*(const RVector<U>& vec) {
            if(size==vec.size) {
                RVector<typename RealOrInt<T,U>::type> res(size);
                for (uint32_t i =0.0; i<size; i++) {
                    res[i] = v[i] * vec[i];
                }
                return res;
            }
            else {
                throw("Sizes of vectors do not match");
            }
        }

        // Constructor methods
        RVector<T>() : size(0),v(NULL) {};

        RVector<T>(std::initializer_list<T> initList) : size(initList.size()), v(std::make_unique<T[]>(initList.size())) {
            uint32_t i = 0;
            for (const auto&elem : initList){
                v[i] = elem;
                i++;
            }
        }

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

        Real L2norm() {
            Real norm = 0.0;
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
        typename RealOrInt<T,U>::type dot(RVector<U> &rhs){
            Real result=0.;
            if (rhs.size == this->size) {
                for (uint32_t i = 0 ; i< size ; i++) {
                    result += v[i] * rhs[i];
                }
            }
            else {
                throw("Rvector, the two vectors have different sizes");
            }
        return result;
        };

        template<typename U>
        bool isclose(const RVector<U> &rhs, Real atol = 0 , Real rtol = 1e-3) {
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
                        if (a != b){
                            return false;
                        }
                    }
                    return true;
                }
            }
            // if types are very different convert both to Real
            else {
                for (uint32_t i =0 ; i< size ; i++){
                    const Real a = (Real)(this->v[i]);
                    const Real b = (Real)(rhs[i]);
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

    RVector<Real> LinSpace(const Real min, const Real max, const uint32_t n){
        RVector<Real> vec(n);
        Real spacing = ( max - min ) / (n-1);
            for (uint32_t i = 0 ; i < n ; i++){
                vec[i] = min + i * spacing;
            }
    return vec;
    };

    RVector<Real> GeomSpace(const Real min, const Real max, const uint32_t n, const Real base){

        Real logmin = log_base(min, base);
        Real logmax = log_base(max, base);
        RVector<Real> vec(n);
        Real spacing = ( logmax - logmin ) / (n-1);
            for (uint32_t i = 0 ; i < n ; i++){
                Real _exp = logmin + i * spacing;
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
