#include "Random.hpp"
#include <boost/random/random_device.hpp>
#include <ctime>

using namespace codechiev::base;

//boost::random::random_device rng;
Random::generator_type gen(static_cast<unsigned int>(std::time(0)));

Random::generator_type &
Random::GetGenerator()
{
    return gen;
}