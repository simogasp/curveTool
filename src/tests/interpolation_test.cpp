#define BOOST_TEST_MODULE testCurves

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "interpolation.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(test_lagrange)

BOOST_AUTO_TEST_CASE(test_lagrange_values)
{
    const std::vector<Point> points{{0, 1}, {2, 5}, {4, 17}, {6, 7}};
    const std::vector<Point> expected{{2.5, 8.42188},
                                      {0.45, -0.345078},
                                      {10.13, -211.913},
                                      {6.1, 5.38438}};
    for(const auto& p : expected)
    {
        BOOST_CHECK_CLOSE(lagrange(p.x, points), p.y, 0.001);
    }
}

BOOST_AUTO_TEST_SUITE_END()