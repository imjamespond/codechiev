#include "Random.hpp"
#include <boost/random/random_device.hpp>
#include <ctime>

using namespace codechiev::base;

//boost::random::random_device rng;
Random::boost_generator_t __boost_generator__(static_cast<unsigned int>(std::time(0)));

Random::boost_generator_t &
Random::GetGenerator()
{
    return __boost_generator__;
}