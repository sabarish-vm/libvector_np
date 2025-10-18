#include "../include/vector.hpp"

Vector::RVector<Real> Vector::LinSpace(const Real min, const Real max, const uint32_t n){
    RVector<Real> vec(n);
    Real spacing = ( max - min ) / (n-1);
        for (uint32_t i = 0 ; i < n ; i++){
            vec[i] = min + i * spacing;
        }
    return vec;
};

Vector::RVector<Real> Vector::GeomSpace(const Real min, const Real max, const uint32_t n, const Real base){

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

