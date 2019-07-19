#ifndef Error_h
#define Error_h

  #include <exception>
  #include <string>

  namespace codechiev
  {
  namespace base
  {

    class Error : public std::exception
    {
    public:
      explicit Error(const std::string &);
      virtual ~Error() throw();
      virtual char const *what() const throw();
      std::string text;
    };
  }
  }

#endif