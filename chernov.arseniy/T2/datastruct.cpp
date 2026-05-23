#include "datastruct.hpp"

#include <cctype>
#include <unordered_set>

std::istream & chernov::operator>>(std::istream & input, LongLongIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  return input >> dest.ref >> DelimitersAnyCaseIO{"ll"};
}

std::istream & chernov::operator>>(std::istream & input, UnsignedLongLongIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  return input >> dest.ref >> DelimitersAnyCaseIO{"ull"};
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

std::istream & chernov::operator>>(std::istream & input, DelimitersIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  char c = 0;
  for (size_t i = 0; input && i < dest.exp.size(); ++i) {
    input >> c;
    if (c != dest.exp[i]) {
      input.setstate(std::ios::failbit);
    }
  }
  return input;
}

std::istream & chernov::operator>>(std::istream & input, DelimitersAnyCaseIO && dest)
{
  std::istream::sentry sentry(input);
  if (!sentry) {
    return input;
  }
  char c = 0;
  for (size_t i = 0; input && i < dest.exp.size(); ++i) {
    input >> c;
    if (std::tolower(c) != std::tolower(dest.exp[i])) {
      input.setstate(std::ios::failbit);
    }
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
  using sep = DelimitersIO;
  using ll = LongLongIO;
  using ull = UnsignedLongLongIO;
  using str = StringIO;

  size_t key = 0;
  std::unordered_set< size_t > entered_keys;

  input >> sep{"(:"};
  for (size_t i = 0; input && i < 3; ++i) {
    input >> sep{"key"} >> key;

    if (entered_keys.count(key)) {
      input.setstate(std::ios::failbit);
      break;
    }
    entered_keys.insert(key);

    switch (key) {
      case 1:
        input >> ll{data_input.key1};
        break;
      case 2:
        input >> ull{data_input.key2};
        break;
      case 3:
        input >> str{data_input.key3};
        break;
      default:
        input.setstate(std::ios::failbit);
    }
    input >> sep{":"};
  }
  input >> sep{")"};

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
  bool result = (lhs.key1 < rhs.key1) && !(lhs.key1 == rhs.key1);
  result = result || ((lhs.key2 < rhs.key2) && (lhs.key1 == rhs.key1) && !(lhs.key2 == rhs.key2));
  result = result || ((lhs.key3.size() < rhs.key3.size()) && (lhs.key1 == rhs.key1) && (lhs.key2 == rhs.key2));
  return result;
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
