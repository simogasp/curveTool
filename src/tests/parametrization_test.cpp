#define BOOST_TEST_MODULE testCurves

#define BOOST_TEST_DYN_LINK
#include <curves/parametrization.h>

#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_SUITE( test_uniformParametrization )

BOOST_AUTO_TEST_CASE(test_uniformParametrization_number_of_elements)
{
    // Test that the function returns the correct number of elements for a given step value
    for(auto step : {5, 10, 13, 25})
    {
        BOOST_CHECK_EQUAL(uniformParametrization(step).size(), step+1);
    }
}

BOOST_AUTO_TEST_CASE(test_uniformParametrization_increment_value)
{
    // Test that the function correctly calculates the increment value
    for(auto step : {5, 10, 13, 25})
    {
        const auto exp_increment = 1.0 / step;
        const auto res = uniformParametrization(step);
        BOOST_CHECK_CLOSE(res[1] - res[0], exp_increment, 0.001);
        BOOST_CHECK_CLOSE(res[2] - res[1], exp_increment, 0.001);
    }
}

BOOST_AUTO_TEST_CASE(test_uniformParametrization_values)
{
    // Test that the function correctly fills the vector with the correct sequence of values
    const auto res = uniformParametrization(5);
    const decltype(res) expected = {0.0, .2, .4, .6, .8, 1.0};
    BOOST_CHECK_EQUAL(res.size(), expected.size());
    for(std::size_t i{0}; i < res.size(); ++i)
    {
        BOOST_CHECK_CLOSE(res[i], expected[i], 0.001);
    }
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_uniformSubdivision )

BOOST_AUTO_TEST_CASE(test_uniformSubdivision_check_output)
{
    const std::size_t nbElem{5};
    const auto increment{.1};
    const auto [T, tToEval] = uniformSubdivision(nbElem, increment);
    BOOST_CHECK_EQUAL(T.size(), nbElem);
    for(std::size_t i{0}; i < T.size(); ++i)
    {
        BOOST_CHECK_EQUAL(T[i], i);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());
    BOOST_CHECK_CLOSE(minVal, 0, 0.001);
    BOOST_CHECK_CLOSE(maxVal, maxVal, 0.001);

    BOOST_CHECK_EQUAL(tToEval.front(), minVal);
    BOOST_CHECK_EQUAL(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        BOOST_CHECK(tToEval[i] >= minVal);
        BOOST_CHECK(tToEval[i] <= maxVal);
        BOOST_CHECK_CLOSE(tToEval[i+1] - tToEval[i], increment, 0.001);
    }
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_distanceSubdivision )

BOOST_AUTO_TEST_CASE(test_distanceSubdivision_check_output)
{
    // all points are separated by a distance 5 (pythagoras! )
    const std::vector<Point> points{ {1.6, 4.25},
                                    {4.6, 8.25},
                                    {1.6, 4.25},
                                    {-2.4, 1.25},
                                    {-8.4, -6.75}};
    const auto nbElem = points.size();
    const auto increment{.1};
    const auto [T, tToEval] = distanceSubdivision(increment, points);
    BOOST_CHECK_EQUAL(T.size(), nbElem);
    const std::vector<double> passage{.0, 5., 10., 15., 25.};
    for(std::size_t i{0}; i < T.size(); ++i)
    {
        BOOST_CHECK_EQUAL(T[i], passage[i]);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());

    BOOST_CHECK_EQUAL(tToEval.front(), minVal);
    BOOST_CHECK_EQUAL(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        BOOST_CHECK(tToEval[i] >= minVal);
        BOOST_CHECK(tToEval[i] <= maxVal);
        const auto diff = tToEval[i+1] - tToEval[i];
        // the last element may be having a shorter increment due to uneven sampling in order to have always the last
        // passage value
        if(i < tToEval.size()-2)
        {
            BOOST_CHECK_CLOSE(diff, increment, 0.001);
        }
        else
        {
            // for the second last element only check that the difference is less than the required step
            BOOST_CHECK(diff < increment);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_rootDistanceSubdivision )

BOOST_AUTO_TEST_CASE(test_rootDistanceSubdivision_check_output)
{
    // all points are separated by a distance 5 (pythagoras! )
    const std::vector<Point> points{ {1.6, 4.25},
                                    {4.6, 8.25},
                                    {1.6, 4.25},
                                    {-2.4, 1.25},
                                    {-8.4, -6.75}};
    const auto nbElem = points.size();
    const auto increment{.1};
    const auto [T, tToEval] = rootDistanceSubdivision(increment, points);
    BOOST_CHECK_EQUAL(T.size(), nbElem);
    const std::vector<double> passage{.0,
                                      std::sqrt(5.),
                                      2. * std::sqrt(5.),
                                      3. * std::sqrt(5.),
                                      (3. * std::sqrt(5.) + std::sqrt(10.)) };

    for(std::size_t i{0}; i < T.size(); ++i)
    {
        BOOST_CHECK_EQUAL(T[i], passage[i]);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());

    BOOST_CHECK_EQUAL(tToEval.front(), minVal);
    BOOST_CHECK_EQUAL(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        BOOST_CHECK(tToEval[i] >= minVal);
        BOOST_CHECK(tToEval[i] <= maxVal);
        const auto diff = tToEval[i+1] - tToEval[i];
        // the last element may be having a shorter increment due to uneven sampling in order to have always the last
        // passage value
        if(i < tToEval.size()-2)
        {
            BOOST_CHECK_CLOSE(diff, increment, 0.001);
        }
        else
        {
            // for the second last element only check that the difference is less than the required step
            BOOST_CHECK(diff < increment);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_chebycheffSubdivision )

BOOST_AUTO_TEST_CASE(test_chebycheffSubdivision_check_output)
{
    // all points are separated by a distance 5 (pythagoras! )
    const std::vector<Point> points{ {1.6, 4.25},
                                    {4.6, 8.25},
                                    {1.6, 4.25},
                                    {-2.4, 1.25},
                                    {-8.4, -6.75},
                                    {-4.4, -3.75}};
    const auto nbElem = points.size();
    const auto increment{.1};
    const auto [T, tToEval] = chebycheffSubdivision(increment, points);
    BOOST_CHECK_EQUAL(T.size(), nbElem);
    const std::vector<double> passage{0.96592582,
                                      0.70710678,
                                      0.258819045,
                                      -0.258819045,
                                      -0.70710678,
                                      -0.96592582 };

    for(std::size_t i{0}; i < T.size(); ++i)
    {
        BOOST_CHECK(T[i] <= 1.);
        BOOST_CHECK(T[i] >= -1.);
        BOOST_CHECK_CLOSE(T[i], passage[i], 0.001);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());

    BOOST_CHECK_EQUAL(tToEval.front(), minVal);
    BOOST_CHECK_EQUAL(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        BOOST_CHECK(tToEval[i] >= minVal);
        BOOST_CHECK(tToEval[i] <= maxVal);
        const auto diff = tToEval[i+1] - tToEval[i];
        // the last element may be having a shorter increment due to uneven sampling in order to have always the last
        // passage value
        if(i < tToEval.size()-2)
        {
            BOOST_CHECK_CLOSE(diff, increment, 0.001);
        }
        else
        {
            // for the second last element only check that the difference is less than the required step
            BOOST_CHECK(diff < increment);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()