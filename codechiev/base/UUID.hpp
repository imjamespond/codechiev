#ifndef UUID_hpp
#define UUID_hpp

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/random/random_device.hpp>

namespace codechiev
{
namespace base
{

typedef struct UUID
{
  typedef boost::uuids::basic_random_generator<boost::mt19937> uuid_gen_t;
  typedef boost::uuids::uuid uuid_t;

  UUID();

  static uuid_t GenUUID();
  static std::string ToString(const uuid_t &);

  struct Ran {
    boost::mt19937 ran;

    Ran();
  };

  Ran ran;
  uuid_gen_t gen;

} uuid;

} // namespace base
} // namespace codechiev

#endif /* UUID_hpp */
