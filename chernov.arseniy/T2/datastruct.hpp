#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>

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

#endif
