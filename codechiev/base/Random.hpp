#ifndef Random_hpp
#define Random_hpp

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/random_device.hpp>

namespace codechiev
{
namespace base
{

class Random
{
  public:
    typedef boost::random::mt19937 boost_generator_t;

    template <typename T>
    static inline T GetInt(T min, T max)
    {
        boost::random::uniform_int_distribution<> dist(min, max);
        return dist(Random::GetGenerator());
    }

    template <typename T>
    static inline T GetRand(T min, T max)
    {
        boost::random::uniform_real_distribution<> dist;
        return min + max * dist(Random::GetGenerator());
    }

    template <typename T>
    static inline T GetDevRand(T min, T max)
    {
        boost::random::random_device rng;
        boost::random::uniform_int_distribution<> index_dist(min, max);
        return index_dist(rng);
    }

  private:
    static boost_generator_t &GetGenerator();
};

template <typename T>
inline T random(T min, T max)
{
    return Random::GetRand(min, max);
}

template <>
inline float random(float min, float max)
{
    return Random::GetRand(min, max);
}

template <>
inline long double random(long double min, long double max)
{
    return Random::GetRand(min, max);
}

template <>
inline double random(double min, double max)
{
    return Random::GetRand(min, max);
}

/* GetRandom */
template <typename T>
T GetRandom()
{
    T rand;
    int randfd = ::open("/dev/urandom", O_RDONLY);
    ::read(randfd, &rand, sizeof(int));
    return rand;
}

} // namespace base
} // namespace codechiev

#endif /* Random_hpp */