#pragma once
#include <ostream>
#include <random>

// Set the defintion of the type Real
#ifndef REAL_PRECISION
#define REAL_PRECISION 1
#endif

#if REAL_PRECISION == 1
    #define REAL_TYPE double
#elif REAL_PRECISION ==2
    #define REAL_TYPE long double
#endif
using Real = REAL_TYPE;
// end of Real

/* Global forward declarations
declarations added here as when required */
namespace Vector {
    template<typename T>
    class RVector;

    template <typename T>
    RVector<T> RandRVector(const uint32_t n, const T min, const T max);

    RVector<Real> LinSpace(const Real min, const Real max, const uint32_t n);


    RVector<Real> GeomSpace(const Real min, const Real max, const uint32_t n, const Real base);
};
// End of foward declarations

inline Real log_base(Real a, Real base) {
    return std::log(a) / std::log(base);
}

// Struct to infer and return Real or Integer
template <typename A, typename B>
struct RealOrInt{
using type = std::conditional_t<std::is_same_v<A, Real>, Real,
                            std::conditional_t<std::is_same_v<B, Real>, Real, int>>;
};

// New type trait struct for RVector class
template <typename T>
struct is_RVector : std::false_type {};

template <typename T>
struct is_RVector<Vector::RVector<T>> : std::true_type {};
// End of type_trait is_RVector

/*
 * ENUM for different arithmetic modes of operation.
 * Useful to to do compile time generation of operator overloads for
 * arithmetic operations.
 */
enum ArithmeticOpType{ADD,SUB,MUL,DIV};

/* A block of code that inlines a arithmetic operation.
 * Identifies the return type and returns a value based on the 
 * ENUM ArithmeticOpType
 * */
template<typename T,typename U, typename ReturnType, ArithmeticOpType op>
inline ReturnType math_operator(const T& x, const U& y) {
    if constexpr(op==ArithmeticOpType::ADD) {
        return x+y;
    }
    else if constexpr(op==ArithmeticOpType::SUB){
        return x-y;
    }
    else if constexpr(op==ArithmeticOpType::MUL){
        return x*y;
    }
    else if constexpr(op==ArithmeticOpType::DIV){
        return Real(x)/Real(y);
    }
};

namespace Vector {

    template <typename T>
    class RVector {
    public:
        bool integral = std::is_integral<T>::value;
        std::uint32_t size;
        std::string value_type =  typeid(T).name();
        using generic_type = T;
        /* generic_type is accessible at compile time since it is initialized
         * with keyword 'using'*/
    private:
        /* keeping variable v as private member ensures that the user of the
         * libary cannot directly access v[i].
         * This ensures that out-of-bounds error are kept at bay.
         * */
        std::unique_ptr<T[]> v;

        /* Forward declare some functions */
        template <typename U>
        friend RVector<U> RandRVector(const uint32_t n, const U min, const U max);

        template<typename U, ArithmeticOpType op>
        auto _arith_op(const U& rhs) {
            if constexpr(is_RVector<U>::value) {
                // store the return_type for later use
                using return_type = std::conditional_t<op==ArithmeticOpType::DIV, Real,
                                 typename RealOrInt<T, typename U::generic_type>::type>;
                RVector<return_type> res(size);
                if (rhs.size == size) {
                for (uint32_t i =0.0; i<size; i++) {
                    // typename U::generic_type tells the compiler that U::generic_type
                    // is a typename that is a template parameter and is a typename
                    res[i] = math_operator<T,typename U::generic_type, return_type,op>(v[i],  rhs[i]);
                    }
                return res;
                } else {
                throw("Sizes of vectors do not match");
                }
            } else {
                RVector<typename RealOrInt<T,U>::type> res(size);
                for (uint32_t i =0.0; i<size; i++) {
                    res[i] = math_operator<T,U,Real,op>(v[i],  rhs);
                    }
            return res;
            }
        };
    public:
        // begin and end methods for enabling ranged loop
        T* begin() {return v.get();};
        T* end() {return (v.get() + size);};

        const T* begin() const {return v.get();};
        const T* end() const {return (v.get() + size);};
        // end of this section
 
        // Operator overloads
        inline T& operator[](const unsigned int i) {
            if (i<0 || i>=size) {
                throw("Rvector subscript out of bounds");
            }
                return v[i];
            }

        inline const T& operator[](const std::size_t i) const {
            if (i<0 || i>=size) {
                throw("Rvector subscript out of bounds");
            }
            return v[i];
        }

        friend inline std::ostream& operator<<(std::ostream& os, const RVector<T>& vec) {
            for (uint32_t i = 0 ; i<vec.size ; i++) {
                    os << vec[i] << ' ';
            }
            os << std::endl;
            return os;
        }

        template<typename U>
        auto operator+(const U& rhs) {
            auto res = _arith_op<U,ArithmeticOpType::ADD>(rhs);
            return res;
        }

        template<typename U>
        auto operator*(const U& rhs) {
            auto res = _arith_op<U,ArithmeticOpType::MUL>(rhs);
            return res;
        }

        template<typename U>
        auto operator-(const U& rhs) {
            auto res = _arith_op<U,ArithmeticOpType::SUB>(rhs);
            return res;
        }

        template<typename U>
        RVector<Real> operator/(const U& rhs) {
            auto res = _arith_op<U, ArithmeticOpType::DIV>(rhs);
            return res;
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

        RVector<T>(const unsigned int N) : size(N), v(std::make_unique<T[]>(N)) {
            for (std::uint32_t i = 0; i<size; i++) {
                v[i] = 0;
            }
        };

        RVector<T>(const unsigned int N, const T& a) : size(N), v(std::make_unique<T[]>(N))  {
            for (std::uint32_t i = 0; i<size; i++) {
                v[i] = a;
            }
        };

        RVector<T>(const RVector<T> &rhs) : size(rhs.size) , v(std::make_unique<T[]>(rhs.size)) {
            for (std::uint32_t i =0 ; i<size ; i++) {
                v[i] = rhs[i];
            }
        }

        RVector<T>(const uint32_t N, const T* arr) : size(N), v(std::make_unique<T[]>(N)) {
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
