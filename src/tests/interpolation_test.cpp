#include <curves/interpolation.h>

#include <gtest/gtest.h>

#include <vector>

TEST(LagrangeTest, LagrangeValues)
{
    const std::vector<Point> points{{0, 1}, {2, 5}, {4, 17}, {6, 7}};
    const std::vector<Point> expected{{2.5, 8.42188},
                                      {0.45, -0.345078},
                                      {10.13, -211.913},
                                      {6.1, 5.38438}};
    for(const auto& p : expected)
    {
        EXPECT_NEAR(lagrange(p.x, points), p.y, std::abs(p.y * 0.001 / 100.0));
    }
}
