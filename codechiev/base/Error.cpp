#include "./Error.hpp"

using namespace codechiev::base;

char const *
Error::what() const throw() { return text.c_str(); }