#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace chernov {
  struct Point {
    int x, y;
  };

  struct Polygon {
    std::vector< Point > points;
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

  std::istream & operator>>(std::istream & input, Point & dest);
  std::istream & operator>>(std::istream & input, Polygon & dest);
  std::istream & operator>>(std::istream & input, DelimiterIO && dest);

  std::ostream & operator<<(std::ostream & output, const Point & dest);
  std::ostream & operator<<(std::ostream & output, const Polygon & dest);
}

#endif
