#include "./Error.hpp"

using namespace codechiev::base;

Error::Error(const std::string &text) : text(text) {}
Error::~Error() throw(){}

char const *
Error::what() const throw() { return text.c_str(); }