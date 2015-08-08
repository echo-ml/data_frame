#include <echo/data_frame/homogenous_data_frame.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::data_frame;

constexpr struct A {
} a{};
constexpr struct B {
} b{};

TEST_CASE("homogenous_data_frame") {
  HomogenousDataFrame<double, htl::Tuple<A, B>> df1(2);
  df1(0, a) = 1;
  df1(0, b) = 2;
  df1(1, a) = 3;
  df1(1, b) = 4;

  CHECK(df1(0, a) == 1);
  CHECK(df1(0, b) == 2);
  CHECK(df1(1, a) == 3);
  CHECK(df1(1, b) == 4);
}
