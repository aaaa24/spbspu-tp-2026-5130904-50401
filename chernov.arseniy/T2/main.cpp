#include <algorithm>
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

  struct ConstLongLongIO {
    const long long & ref;
  };

  struct ConstUnsignedLongLongIO {
    const unsigned long long & ref;
  };

  struct ConstStringIO {
    const std::string & ref;
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
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::istream & operator>>(std::istream & input, LongLongIO && dest);
  std::istream & operator>>(std::istream & input, UnsignedLongLongIO && dest);
  std::istream & operator>>(std::istream & input, StringIO && dest);
  std::istream & operator>>(std::istream & input, LabelIO && dest);
  std::istream & operator>>(std::istream & input, DelimiterIO && dest);
  std::istream & operator>>(std::istream & input, DataStruct & dest);

  std::ostream & operator<<(std::ostream & output, const ConstLongLongIO & dest);
  std::ostream & operator<<(std::ostream & output, const ConstUnsignedLongLongIO & dest);
  std::ostream & operator<<(std::ostream & output, const ConstStringIO & dest);
  std::ostream & operator<<(std::ostream & output, const DataStruct & dest);

  bool operator<(const DataStruct & lhs, const DataStruct & rhs);
}

int main()
{
  std::vector< chernov::DataStruct > data;
  using iit_t = std::istream_iterator< chernov::DataStruct >;
  using oit_t = std::ostream_iterator< chernov::DataStruct >;

  std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
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
  if ((input >> data) && (data != dest.exp)) {
    std::cout << "data: " << data << "; dest.exp: " << dest.exp << "\n";
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

std::istream & chernov::operator>>(std::istream & input, DataStruct & dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  DataStruct data_input;
  {
    using sep = DelimiterIO;
    using lbl = LabelIO;
    using ll = LongLongIO;
    using ull = UnsignedLongLongIO;
    using str = StringIO;
    input >> sep{'('} >> sep{':'};
    input >> lbl{"key1"} >> ll{data_input.key1};
    input >> sep{':'};
    input >> lbl{"key2"} >> ull{data_input.key2};
    input >> sep{':'};
    input >> lbl{"key3"} >> str{data_input.key3};
    input >> sep{':'} >> sep{')'};
  }
  if (input) {
    dest = data_input;
  }
  return input;
}

std::ostream & chernov::operator<<(std::ostream & output, const ConstLongLongIO & dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry) {
    return output;
  }
  IOguard fmtguard(output);
  return output << dest.ref << "ll";
}

std::ostream & chernov::operator<<(std::ostream & output, const ConstUnsignedLongLongIO & dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry) {
    return output;
  }
  IOguard fmtguard(output);
  return output << dest.ref << "ull";
}

std::ostream & chernov::operator<<(std::ostream & output, const ConstStringIO & dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry) {
    return output;
  }
  IOguard fmtguard(output);
  return output << "\"" << dest.ref << "\"";
}

std::ostream & chernov::operator<<(std::ostream & output, const DataStruct & dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry) {
    return output;
  }
  IOguard fmtguard(output);

  using ll = ConstLongLongIO;
  using ull = ConstUnsignedLongLongIO;
  using str = ConstStringIO;
  output << "(:key1 " << ll{dest.key1} << ":key2 " << ull{dest.key2} << ":key3 " << str{dest.key3} << ":)";
  return output;
}

bool chernov::operator<(const DataStruct & lhs, const DataStruct & rhs)
{
  if (lhs.key1 == rhs.key1) {
    if (lhs.key2 == rhs.key2) {
      return lhs.key3.size() < rhs.key3.size();
    }
    return lhs.key2 < rhs.key2;
  }
  return lhs.key1 < rhs.key1;
}

chernov::IOguard::IOguard(std::basic_ios< char > & s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

chernov::IOguard::~IOguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
