#include "UUID.hpp"
#include "Random.hpp"
#include "Singleton.hpp"

using namespace codechiev::base;

UUID::Ran::Ran()
{
  // ran.seed(time(NULL)); // one should likely seed in a better way
  uint32_t seed = GetRandom<uint32_t>();
  // printf("%u\n", seed);
  ran.seed(seed);
}

UUID::UUID() : ran(), gen(&ran.ran)
{
}

UUID::uuid_t UUID::GenUUID()
{
  UUID *uuid = ThreadSingleton<UUID>::Get();
  return uuid->gen();
}

std::string UUID::ToString(const UUID::uuid_t &uuid)
{
  std::string str = boost::lexical_cast<std::string>(uuid);
  return str;
}