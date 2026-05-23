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

  struct LabelIO {
    std::string exp;
  };

  struct DelimiterIO {
    char exp;
  };
}

int main()
{

}
