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

namespace codechiev {
    namespace base {
        
        class Random
        {
        public:
            typedef boost::random::mt19937 generator_type;
            
            template <typename T>
            static inline T GetInt(T min, T max)
            {
                boost::random::uniform_int_distribution<> dist(min, max);
                return dist(Random::GetGenerator());
            }
            
            template <typename T>
            static inline T GetRandom(T min, T max)
            {
                boost::random::uniform_real_distribution<> dist;
                return min + max * dist(Random::GetGenerator());
            }

            template <typename T>
            static inline T GetDevRandom(T min, T max)
            {
                boost::random::random_device rng;
                boost::random::uniform_int_distribution<> index_dist(min, max);
                return index_dist(rng);
            }
            
        private:
            static generator_type &GetGenerator();
        };
        
        template<typename T>
        inline T random(T min, T max) {
            return Random::GetRandom(min, max);
        }
        
        template<>
        inline float random(float min, float max) {
            return Random::GetRandom(min, max);
        }
        
        template<>
        inline long double random(long double min, long double max) {
            return Random::GetRandom(min, max);
        }
        
        template<>
        inline double random(double min, double max) {
            return Random::GetRandom(min, max);
        }

        /* getRandom */

        inline int getRandom()
        {
            int rand;
            int randfd = open("/dev/urandom", O_RDONLY);
            read(randfd, &rand, sizeof(int));
            return rand;
        }
        
    }
}

#endif /* Random_hpp */