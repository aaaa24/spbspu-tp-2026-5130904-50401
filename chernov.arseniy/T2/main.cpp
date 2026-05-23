#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

namespace chernov {
  struct DataStruct {
    long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct LongLongIO {
    long long & ref;
  };

  struct UnsignedLongLongIO {
    unsigned long long & ref;
  };

  struct StringIO {
    std::string & ref;
  };

  struct LabelIO {
    std::string exp;
  };

  struct DelimiterIO {
    char exp;
  };

  class IOguard {
  public:
    explicit IOguard(std::basic_ios< char > & s);
    ~IOguard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  std::istream & operator>>(std::istream & input, LongLongIO && dest);
  std::istream & operator>>(std::istream & input, UnsignedLongLongIO && dest);
  std::istream & operator>>(std::istream & input, StringIO && dest);
  std::istream & operator>>(std::istream & input, LabelIO && dest);
  std::istream & operator>>(std::istream & input, DelimiterIO && dest);
  std::istream & operator>>(std::istream & input, DataStruct & dest);
  std::istream & operator<<(std::ostream & output, const DataStruct & dest);
}

int main()
{
  std::vector< chernov::DataStruct > data;
  std::istringstream iss("(:key1 10ull:key2 ’c’:key3 \"Data\":)");
  {
    using iit_t = std::istream_iterator< chernov::DataStruct >;
    std::copy(iit_t{iss}, iit_t{}, std::back_inserter(data));
  }
  {
    using oit_t = std::istream_iterator< chernov::DataStruct >;
    std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
  }
}

std::istream & chernov::operator>>(std::istream & input, LongLongIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  return input >> dest.ref >> DelimiterIO{'l'} >> DelimiterIO{'l'};
}

std::istream & chernov::operator>>(std::istream & input, UnsignedLongLongIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  return input >> dest.ref >> DelimiterIO{'u'} >> DelimiterIO{'l'} >> DelimiterIO{'l'};
}

std::istream & chernov::operator>>(std::istream & input, StringIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  return std::getline(input >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream & chernov::operator>>(std::istream & input, LabelIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  std::string data = "";
  if ((input >> StringIO{data}) && (data != dest.exp)) {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream & chernov::operator>>(std::istream & input, DelimiterIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  char c = 0;
  input >> c;
  if (input && (c != dest.exp)) {
    input.setstate(std::ios::failbit);
  }
  return input;
}


