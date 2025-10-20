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
// end of setting Real
