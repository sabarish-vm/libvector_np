// Copyright (c) 2025 Sabarish. All Rights Reserved.
// This file is part of libvector_np, licensed under the GNU General
// Public License. See the LICENSE file included with this distribution
// for details.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cmath>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "../include/vector.hpp"
const double PI = 3.14159265358979323846264338;

template <typename T, typename U>
double isclose(T const& a1, U const& a2) {
  double a = a1;
  double b = a2;
  double atol = 1e-16;
  double rtol = 1e-16;
  auto diff = std::abs(a - b);
  bool ret = diff < (atol + rtol * std::abs(b));
  return ret;
}

TEST_CASE("Vector Basic Operations") {
  {  // Check type boundness
     // Vector::RVector<char> nullvec;
  }
  {
    Vector::RVector<double> nullvec;
    CHECK_EQ(nullvec.size, 0);
  }

  {
    // Check zero vector
    Vector::RVector<unsigned int> zerovec(3);
    CHECK_EQ(zerovec[0], 0);
    Vector::RVector<double> zerovecD(3);
    CHECK_EQ(zerovecD[0], 0.0);
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
    CHECK_EQ(randomSize.size, copyRandomSize.size);
    CHECK_NE(prandSize, pCopyRandSize);
  }

  {
    // Ensure that for a raw pointer, the contents are copied and moved
    int* raw_ptr = new int[2];
    raw_ptr[0] = 1;
    raw_ptr[1] = 2;
    // Ensure that move does nothing
    Vector::RVector<int> vec(2, std::move(raw_ptr));
    Vector::RVector<int> vec2(2, raw_ptr);
    CHECK(raw_ptr[0] == vec[0]);
    CHECK(raw_ptr[0] == vec2[0]);
  }
  {
    // Ensure that unique_ptr copy constructor works
    auto raw_ptr = std::make_unique<int[]>(2);
    raw_ptr[0] = 1;
    raw_ptr[1] = 2;
    Vector::RVector<int> vec(2, raw_ptr);
    CHECK(raw_ptr[0] == vec[0]);
  }
  {
    // Ensure that unique_ptr move constructor works
    auto raw_ptr = std::make_unique<int[]>(2);
    raw_ptr[0] = 1;
    raw_ptr[1] = 2;
    Vector::RVector<int> vec(2, std::move(raw_ptr));
    CHECK_EQ(raw_ptr, nullptr);
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
    CHECK_EQ(linspaceVec.size, 4);
    CHECK_EQ(linspaceVec[0], 1.0);
    CHECK_EQ(linspaceVec[3], 4.0);
  }

  {
    // testing Arange
    auto arangeVec = Vector::Arange(1, 3, 1);
    CHECK_EQ(arangeVec.size, 2);
    CHECK_EQ(arangeVec[0], 1);
    CHECK_EQ(arangeVec[1], 2);
  }

  {
    // testing GeomSpace
    auto geomSpaceVec = Vector::GeomSpace(1.0, 1000.0, 3, 10.0);
    Vector::RVector<double> resGeomSpace({1, 31.6228, 1000});
    CHECK(geomSpaceVec.isclose(resGeomSpace));

    {
      // test ranged loop and initializer list
      uint32_t i = 0;
      std::vector<double> resdata({1, 31.6228, 1000});
      for (const double& elem : resGeomSpace) {
        CHECK(elem == resdata[i]);
        i++;
      }
      CHECK(i == resGeomSpace.size);
    }
  }

  {
    // testing sorting
    Vector::RVector<int> tosortVec({5, 3, 2, 1, -1});
    auto sorted = tosortVec.sort();
    CHECK_EQ(sorted[0], -1);
  }

  {
    // Testing dot product and other math operations
    Vector::RVector<int> v1({1, 2, 3});

    Vector::RVector<int> v2({3, 2, 1});

    Vector::RVector<double> vd({3.0, 2, 1});

    auto vA = v1 + v2;
    auto vS = v1 - v2;
    auto vM = v1 * vd;
    auto vD = v1 / v2;

    CHECK_EQ(v1.dot(v2), 10u);
    CHECK_EQ(*(typeid(v1.dot(v2)).name()), 'i');
    CHECK_EQ(*(typeid(v1.dot(vd)).name()), 'd');
    CHECK_EQ(vA[0], 4);
    CHECK_EQ(vS[0], -2);
    CHECK_EQ(vS[0], -2);
    CHECK_EQ(vM[0], 3);
    CHECK_EQ(*(typeid(vM[0]).name()), 'd');
    CHECK(vD[0] == 1. / 3.0);

    // Power function
    auto viPi = v1.pow(3);
    auto viPd = v1.pow(static_cast<double>(3));
    CHECK_EQ(viPi[1], 8);
    CHECK_EQ(viPi.value_type, "i");
    CHECK_EQ(viPd.value_type, "d");
  }
  {
    // Apply method
    auto x =
        Vector::RVector<double>({0., PI / 4, PI / 2, 3 * PI / 2, PI, 2 * PI});
    auto sinx = Vector::RVector<double>({0, 0.70710678, 1.0, -1.0, 0.0, 0.0});
    // Uses lambda function
    auto fsin = [](const double& a) -> double { return std::sin(a); };
    auto y = x.apply<+fsin>();
    // Static cast
    constexpr auto f3 = static_cast<double (*)(const double)>(std::sin);
    auto y2 = x.apply<f3>();

    CHECK(y.isclose(sinx, 1e-6, 1e-3));
    CHECK(y2.isclose(sinx, 1e-6, 1e-3));
  }
  {  // Test Concatenation
    auto x = Vector::RVector<int>({0, 1, 2, 3});
    auto y = Vector::RVector<double>({4, 5, 6, 7});
    auto z = Vector::Concat(x, y);
    CHECK(::isclose(*(x.begin()), *(z.begin())));
    CHECK(::isclose(*(y.end()), *(z.end())));
  }
}
