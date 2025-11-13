#include <curves/parametrization.h>

#include <gtest/gtest.h>

#include <vector>

TEST(UniformParametrizationTest, NumberOfElements)
{
    // Test that the function returns the correct number of elements for a given step value
    for(auto step : {5u, 10u, 13u, 25u})
    {
        EXPECT_EQ(uniformParametrization(step).size(), step+1);
    }
}

TEST(UniformParametrizationTest, IncrementValue)
{
    // Test that the function correctly calculates the increment value
    for(auto step : {5u, 10u, 13u, 25u})
    {
        const auto exp_increment = 1.0 / step;
        const auto res = uniformParametrization(step);
        EXPECT_NEAR(res[1] - res[0], exp_increment, std::abs(exp_increment * 0.001 / 100.0));
        EXPECT_NEAR(res[2] - res[1], exp_increment, std::abs(exp_increment * 0.001 / 100.0));
    }
}

TEST(UniformParametrizationTest, Values)
{
    // Test that the function correctly fills the vector with the correct sequence of values
    const auto res = uniformParametrization(5);
    const decltype(res) expected = {0.0, .2, .4, .6, .8, 1.0};
    EXPECT_EQ(res.size(), expected.size());
    for(std::size_t i{0}; i < res.size(); ++i)
    {
        EXPECT_NEAR(res[i], expected[i], std::abs(expected[i] * 0.001 / 100.0 + 1e-9));
    }
}

//////////////////////////////////////////////////////////////////////////////////

TEST(UniformSubdivisionTest, CheckOutput)
{
    const std::size_t nbElem{5};
    const auto increment{.1};
    const auto [T, tToEval] = uniformSubdivision(nbElem, increment);
    EXPECT_EQ(T.size(), nbElem);
    for(std::size_t i{0}; i < T.size(); ++i)
    {
        EXPECT_EQ(T[i], i);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());
    EXPECT_NEAR(minVal, 0, 1e-9);
    EXPECT_NEAR(maxVal, maxVal, 1e-9);

    EXPECT_EQ(tToEval.front(), minVal);
    EXPECT_EQ(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        EXPECT_TRUE(tToEval[i] >= minVal);
        EXPECT_TRUE(tToEval[i] <= maxVal);
        EXPECT_NEAR(tToEval[i+1] - tToEval[i], increment, std::abs(increment * 0.001 / 100.0));
    }
}

//////////////////////////////////////////////////////////////////////////////////

TEST(DistanceSubdivisionTest, CheckOutput)
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
    EXPECT_EQ(T.size(), nbElem);
    const std::vector<double> passage{.0, 5., 10., 15., 25.};
    for(std::size_t i{0}; i < T.size(); ++i)
    {
        EXPECT_EQ(T[i], passage[i]);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());

    EXPECT_EQ(tToEval.front(), minVal);
    EXPECT_EQ(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        EXPECT_TRUE(tToEval[i] >= minVal);
        EXPECT_TRUE(tToEval[i] <= maxVal);
        const auto diff = tToEval[i+1] - tToEval[i];
        // the last element may be having a shorter increment due to uneven sampling in order to have always the last
        // passage value
        if(i < tToEval.size()-2)
        {
            EXPECT_NEAR(diff, increment, std::abs(increment * 0.001 / 100.0));
        }
        else
        {
            // for the second last element only check that the difference is less than the required step
            EXPECT_TRUE(diff < increment);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

TEST(RootDistanceSubdivisionTest, CheckOutput)
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
    EXPECT_EQ(T.size(), nbElem);
    const std::vector<double> passage{.0,
                                      std::sqrt(5.),
                                      2. * std::sqrt(5.),
                                      3. * std::sqrt(5.),
                                      (3. * std::sqrt(5.) + std::sqrt(10.)) };

    for(std::size_t i{0}; i < T.size(); ++i)
    {
        EXPECT_EQ(T[i], passage[i]);
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());

    EXPECT_EQ(tToEval.front(), minVal);
    EXPECT_EQ(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        EXPECT_TRUE(tToEval[i] >= minVal);
        EXPECT_TRUE(tToEval[i] <= maxVal);
        const auto diff = tToEval[i+1] - tToEval[i];
        // the last element may be having a shorter increment due to uneven sampling in order to have always the last
        // passage value
        if(i < tToEval.size()-2)
        {
            EXPECT_NEAR(diff, increment, std::abs(increment * 0.001 / 100.0));
        }
        else
        {
            // for the second last element only check that the difference is less than the required step
            EXPECT_TRUE(diff < increment);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

TEST(ChebycheffSubdivisionTest, CheckOutput)
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
    EXPECT_EQ(T.size(), nbElem);
    const std::vector<double> passage{0.96592582,
                                      0.70710678,
                                      0.258819045,
                                      -0.258819045,
                                      -0.70710678,
                                      -0.96592582 };

    for(std::size_t i{0}; i < T.size(); ++i)
    {
        EXPECT_TRUE(T[i] <= 1.);
        EXPECT_TRUE(T[i] >= -1.);
        EXPECT_NEAR(T[i], passage[i], std::abs(passage[i] * 0.001 / 100.0));
    }
    const auto minVal = *std::min_element(T.begin(), T.end());
    const auto maxVal = *std::max_element(T.begin(), T.end());

    EXPECT_EQ(tToEval.front(), minVal);
    EXPECT_EQ(tToEval.back(), maxVal);
    for(std::size_t i{0}; i < tToEval.size()-1; ++i)
    {
        EXPECT_TRUE(tToEval[i] >= minVal);
        EXPECT_TRUE(tToEval[i] <= maxVal);
        const auto diff = tToEval[i+1] - tToEval[i];
        // the last element may be having a shorter increment due to uneven sampling in order to have always the last
        // passage value
        if(i < tToEval.size()-2)
        {
            EXPECT_NEAR(diff, increment, std::abs(increment * 0.001 / 100.0));
        }
        else
        {
            // for the second last element only check that the difference is less than the required step
            EXPECT_TRUE(diff < increment);
        }
    }
}
