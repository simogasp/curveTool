#include <curves/Point.h>

#include <gtest/gtest.h>

#include <vector>

TEST(PointTest, Lerp)
{
    const Point a{-1, -1};
    const Point b{1, 1};
    // a map <t, point>, all the points between a and b at time t
    const std::map<double, Point> vec {{.0, {-1., -1}},
                                      {.25, {-.5, -.5}},
                                      {.5, {.0, .0}},
                                      {.75, {.5, .5}},
                                      {1., {1., 1}} };
    for(const auto&[t, p] : vec)
    {
        EXPECT_NEAR(glm::distance(lerp(a, b, t), p), 0, 0.000001);
    }
}

TEST(PointTest, GetClosestPoint)
{
    const std::vector<Point> vec { {-2., -2.}, {-1., -1.}, {.0, .0}, {1., 1.}, {2., 2.} };

    // identity check
    for(const auto& p : vec)
    {
        for(const auto threshold : {.01, .1, 1., 10., 100.})
        {
            const auto res = getClosestPoint(vec, p, threshold);
            EXPECT_TRUE(res.has_value());
            EXPECT_NEAR(glm::distance(res.value(), p), 0, 0.000001);
        }
    }
    // identity check
    for(const auto& idx : {0u, 1u, 2u, 3u, 4u})
    {
        const auto& p = vec[idx];
        for(const auto threshold : {.01, .1, 1., 10., 100.})
        {
            const auto res = getClosestPointIndex(vec, p, threshold);
            EXPECT_TRUE(res.has_value());
            EXPECT_NEAR(glm::distance(vec[res.value()], p), 0, 0.000001);
        }
    }

    const std::vector<Point> midpoint { {-2.5, -2.5}, {-1.5, -1.5}, {.5, .5}, {1.5, 1.5}, {2.5, 2.5} };
    const std::vector<std::size_t> idx_closest{0, 0, 2, 3, 4};
    for(const auto& idx : {0u, 1u, 2u, 3u, 4u})
    {
        const auto& p = midpoint[idx];
        for(const auto threshold : {.01, .1, 1., 10., 100.})
        {
            const auto res = getClosestPointIndex(vec, p, threshold);
            if(threshold > 0.5)
            {
                EXPECT_TRUE(res.has_value());
                EXPECT_EQ(res.value(), idx_closest[idx]);
                EXPECT_TRUE(glm::distance(vec[res.value()], p) < threshold);
            }
            else
            {
                EXPECT_FALSE(res.has_value());
            }
        }
    }
}

TEST(PointTest, UpdatePoint)
{
    const Point p_end{-1.5, 6.5};
    std::vector<Point> vec { {-2., -2.}, {-1., -1.}, {.0, .0}, {1., 1.}, {2., 2.} };

    for(const auto& idx : {0u, 1u, 2u, 3u, 4u})
    {
        for(auto t : {.0, .25, .5, .75, 1.})
        {
            const auto& p_start = vec[idx];
            const auto& p_new = lerp(p_start, p_end, t);
            updatePointAtIndex(vec, idx, p_new);
            EXPECT_NEAR(glm::distance(vec[idx], p_new), 0, 0.000001);
        }
    }
    for(const auto& idx : {10u, 100u, 22u, 43u, 46u})
    {
        EXPECT_THROW(updatePointAtIndex(vec, idx, {}), std::invalid_argument);
    }

}

TEST(PointTest, DeletePoint)
{
    std::vector<Point> vec { {-2., -2.}, {-1., -1.}, {.0, .0}, {1., 1.}, {2., 2.} };

    for(const std::size_t idx : {2u, 3u, 0u})
    {
            const auto p_orig = vec[idx];
            const auto size_orig = vec.size();
            deletePointAtIndex(vec, idx);
            EXPECT_EQ(vec.size(), size_orig-1);
            EXPECT_GT(glm::distance(vec[std::min(idx, vec.size()-1)], p_orig), 1);
    }
    for(const auto& idx : {10u, 100u, 22u, 43u, 46u})
    {
        EXPECT_THROW(deletePointAtIndex(vec, idx), std::invalid_argument);
    }

}
