#define BOOST_TEST_MODULE testCurves

#define BOOST_TEST_DYN_LINK
#include <curves/Point.h>

#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_SUITE( test_point )

BOOST_AUTO_TEST_CASE(test_point_lerp)
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
        BOOST_CHECK_SMALL( glm::distance(lerp(a, b, t), p), 0.000001 );
    }
}

BOOST_AUTO_TEST_CASE(test_point_getClosestPoint)
{
    const std::vector<Point> vec { {-2., -2.}, {-1., -1.}, {.0, .0}, {1., 1.}, {2., 2.} };

    // identity check
    for(const auto& p : vec)
    {
        for(const auto threshold : {.01, .1, 1., 10., 100.})
        {
            const auto res = getClosestPoint(vec, p, threshold);
            BOOST_CHECK(res.has_value());
            BOOST_CHECK_SMALL(glm::distance(res.value(), p), 0.000001);
        }
    }
    // identity check
    for(const auto& idx : {0, 1, 2, 3, 4})
    {
        const auto& p = vec[idx];
        for(const auto threshold : {.01, .1, 1., 10., 100.})
        {
            const auto res = getClosestPointIndex(vec, p, threshold);
            BOOST_CHECK(res.has_value());
            BOOST_CHECK_SMALL(glm::distance(vec[res.value()], p), 0.000001);
        }
    }

    const std::vector<Point> midpoint { {-2.5, -2.5}, {-1.5, -1.5}, {.5, .5}, {1.5, 1.5}, {2.5, 2.5} };
    const std::vector<std::size_t> idx_closest{0, 0, 2, 3, 4};
    for(const auto& idx : {0, 1, 2, 3, 4})
    {
        const auto& p = midpoint[idx];
        for(const auto threshold : {.01, .1, 1., 10., 100.})
        {
            const auto res = getClosestPointIndex(vec, p, threshold);
            if(threshold > 0.5)
            {
                BOOST_CHECK(res.has_value());
                BOOST_CHECK_EQUAL(res.value(), idx_closest[idx]);
                BOOST_CHECK(glm::distance(vec[res.value()], p) < threshold);
            }
            else
            {
                BOOST_CHECK(!res.has_value());
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(test_point_updatePoint)
{
    const Point p_end{-1.5, 6.5};
    std::vector<Point> vec { {-2., -2.}, {-1., -1.}, {.0, .0}, {1., 1.}, {2., 2.} };

    for(const auto& idx : {0, 1, 2, 3, 4})
    {
        for(auto t : {.0, .25, .5, .75, 1.})
        {
            const auto& p_start = vec[idx];
            const auto& p_new = lerp(p_start, p_end, t);
            updatePointAtIndex(vec, idx, p_new);
            BOOST_CHECK_SMALL(glm::distance(vec[idx], p_new), 0.000001);
        }
    }
    for(const auto& idx : {10, 100, 22, 43, 46})
    {
        BOOST_CHECK_THROW(updatePointAtIndex(vec, idx, {}), std::invalid_argument);
    }

}

BOOST_AUTO_TEST_CASE(test_point_deletePoint)
{
    std::vector<Point> vec { {-2., -2.}, {-1., -1.}, {.0, .0}, {1., 1.}, {2., 2.} };

    for(const std::size_t idx : {2, 3, 0})
    {
            const auto p_orig = vec[idx];
            const auto size_orig = vec.size();
            deletePointAtIndex(vec, idx);
            BOOST_CHECK_EQUAL(vec.size(), size_orig-1);
            BOOST_CHECK_GT(glm::distance(vec[std::min(idx, vec.size()-1)], p_orig), 1);
    }
    for(const auto& idx : {10, 100, 22, 43, 46})
    {
        BOOST_CHECK_THROW(deletePointAtIndex(vec, idx), std::invalid_argument);
    }

}

BOOST_AUTO_TEST_SUITE_END()