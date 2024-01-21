#include <iostream>
#include "constructable_with.hpp"

struct Point2D {
  int x, y;
};

struct ColorRGB {
  int red, green, blue;
};

struct InvalidType {
  // This type has more than 12 members, so it doesn't meet the concept requirements.
  int a, b, c, d, e, f, g, h, i, j, k, l, m;
};

int main() {
  constexpr bool isPoint2DAggregate = lib::aggregate_of<Point2D, 2>;  // Should be true
  constexpr bool isColorRGBAggregate = lib::aggregate_of<ColorRGB, 3>;  // Should be true
  constexpr bool isInvalidAggregate = lib::aggregate_of<InvalidType, 12>;  // Should be false

  std::cout << "Point2D is an aggregate: " << isPoint2DAggregate << std::endl;
  std::cout << "ColorRGB is an aggregate: " << isColorRGBAggregate << std::endl;
  std::cout << "InvalidType is an aggregate: " << isInvalidAggregate << std::endl;

  constexpr auto numMembersPoint2D = lib::number_of_aggregate_members<Point2D>;
  constexpr auto numMembersColorRGB = lib::number_of_aggregate_members<ColorRGB>;

  std::cout << "Number of members in Point2D: " << numMembersPoint2D << std::endl;
  std::cout << "Number of members in ColorRGB: " << numMembersColorRGB << std::endl;

  return 0;
}
