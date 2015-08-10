#include <echo/data_frame/homogenous_data_frame.h>
#include <echo/data_frame/homogenous_data_frame_view.h>
#include <echo/data_frame/iteration.h>
#include <echo/data_frame/subvector.h>
#include <echo/data_frame/matrix_conversion.h>
#include <echo/data_frame/subdata_frame.h>
#include <echo/test.h>
#include <numeric>

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

TEST_CASE("homogenous_data_frame_view") {
  HomogenousDataFrame<double, htl::Tuple<A, B>> df1(2);
  df1(0, a) = 1;
  df1(0, b) = 2;
  df1(1, a) = 3;
  df1(1, b) = 4;

  auto v1 = make_view(df1);
  auto v2 = make_cview(df1);
  auto v3 = make_view(v1);
  auto v4 = make_cview(v1);

  CHECK(v1(0, a) == 1);
  CHECK(v4(0, a) == 1);

  type_equal<decltype(v1(0, a)), double&>();
  type_equal<decltype(v2(0, a)), const double&>();
  
  v1 = {{3, 4}, {5, 6}};
  CHECK(df1(0, a) == 3);
}

TEST_CASE("homogenous_data_frame_iteration") {
  HomogenousDataFrame<double, htl::Tuple<A, B>> df1(2);
  df1 = {{1, 2}, {3, 4}};

  auto i = all_cbegin(df1);
  CHECK(*i++ == 1);
  CHECK(*i++ == 3);
  CHECK(*i++ == 2);
  CHECK(*i++ == 4);
}

TEST_CASE("subvector") {
  HomogenousDataFrame<double, htl::Tuple<A, B>> df1(2);
  df1 = {{1, 2}, {3, 4}};

  auto col1 = get_column(0, df1);
  CHECK(col1(0) == 1);
  CHECK(col1(1) == 3);

  auto row1 = get_row(1, df1);
  CHECK(row1(a) == 3);
  CHECK(row1(b) == 4);
}

TEST_CASE("to_matrix") {
  HomogenousDataFrame<double, htl::Tuple<A, B>> df1(2);
  df1 = {{1, 2}, {3, 4}};

  auto m1 = to_matrix(df1);
  CHECK(m1(0,0) == 1);
}

TEST_CASE("homogenous_subdata_frame") {
  HomogenousDataFrame<double, htl::Tuple<A, B>> df1(3);
  std::iota(all_begin(df1), all_end(df1), 0);

  auto df2 = make_subdata_frame(df1, 1, 3);
  CHECK(df2(0, a) == 1);
  CHECK(df2(1, a) == 2);
  CHECK(df2(0, b) == 4);
  CHECK(df2(1, b) == 5);
}
