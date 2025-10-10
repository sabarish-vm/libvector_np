#include "../include/vector.hpp"
#include <cstdint>
#include <random>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <typeinfo>

TEST_CASE("Vector Basic Operations") {
    {
    Vector::RVector<double> nullvec ;
    CHECK(nullvec.size == 0);
    }

    {
    // Check zero vector
    Vector::RVector<unsigned int> zerovec(3);
    CHECK(zerovec[0] == 0);
    Vector::RVector<double> zerovecD(3);
    CHECK(zerovecD[0] == 0.0);
    }

    {
    // Check if copying worked
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(3, 11);
    int rn = distrib(gen);
 
    Vector::RVector<int> randomSize(rn, 1);
    Vector::RVector<int> copyRandomSize(randomSize);
    Vector::RVector<int>* prandSize = &randomSize;
    Vector::RVector<int>* pCopyRandSize = &copyRandomSize;
    CHECK(randomSize.size == copyRandomSize.size);
    CHECK(prandSize != pCopyRandSize);
    }

    {
    // Norm checker
    Vector::RVector<int> normchecker(3, 1);
    CHECK(std::abs(1.0 - normchecker.L2norm() / std::sqrt(3)) < 1e-3);
    }

    {
    // isclose checker
    Vector::RVector<int> vecint(3, 1);
    Vector::RVector<int> copyvecint(vecint);
    CHECK(vecint.isclose(copyvecint));

    Vector::RVector<double> vecdoub(3, 1.0);
    CHECK(vecint.isclose(vecdoub));
    Vector::RVector<double> copyvecdoub(vecdoub);
    CHECK(vecdoub.isclose(copyvecdoub));
    }

    {
    // testing random vector generator
    auto randVecInt = Vector::RandRVector(3, 1, 3);
    auto randVecReal = Vector::RandRVector(3, 1.0, 3.0);
    }

    {
    // testing LinSpace
    auto linspaceVec = Vector::LinSpace(1.0, 4.0, 4);
    CHECK(linspaceVec.size ==4);
    CHECK(linspaceVec[0] == 1.0);
    CHECK(linspaceVec[3] == 4.0);
    }

    {
    // testing Arange
    auto arangeVec = Vector::Arange(1, 3, 1);
    CHECK(arangeVec.size == 2);
    CHECK(arangeVec[0] == 1);
    CHECK(arangeVec[1] == 2);
    }

    {
    // testing GeomSpace
    auto geomSpaceVec = Vector::GeomSpace(1.0, 1000.0, 3, 10.0);
    Vector::RVector<double> resGeomSpace({1,31.6228,1000});
    CHECK(geomSpaceVec.isclose(resGeomSpace));

    {
    // test ranged loop and initializer list
        uint32_t i = 0;
        std::vector<double> resdata({1,31.6228,1000});
        for (const double &elem : resGeomSpace) {
            CHECK(elem == resdata[i] );
            i++;
        }
        CHECK(i==resGeomSpace.size);
    }
    }

    {
    // testing sorting
    Vector::RVector<int> tosortVec({5,3,2,1,-1});
    auto sorted = tosortVec.sort();
    CHECK(sorted[0] == -1);
    }

    {
    // Testing dot product and other math operations
    Vector::RVector<int> v1({1,2,3});

    Vector::RVector<int> v2({3,2,1});

    Vector::RVector<double> vd({3.0,2,1});

    auto vA = v1 + v2 ;
    auto vS = v1 - v2 ;
    auto vM = v1 * vd ;
    auto vD = v1 / v2;

    CHECK(v1.dot(v2)==10u);
    CHECK( *(typeid(v1.dot(v2)).name()) == 'i' );
    CHECK( *(typeid(v1.dot(vd)).name()) == 'd');
    CHECK(vA[0]== 4);
    CHECK(vS[0]==-2);
    CHECK(vS[0]==-2);
    CHECK(vM[0]==3);
    CHECK( *(typeid(vM[0]).name()) == 'd');
    CHECK(vD[0]==1./3.0);
    }
}

