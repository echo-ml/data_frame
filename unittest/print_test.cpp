#include <echo/data_frame/print.h>
#include <echo/data_frame/homogenous_data_frame.h>
#include <echo/data_frame/named_tag.h>
#include <echo/test.h>
#include <iostream>
using namespace echo;
using namespace echo::data_frame;

ECHO_NAMED_TAG("cat", cat);
ECHO_NAMED_TAG("dog", dog);
struct mammal {};

TEST_CASE("print") {
  using Tags = htl::Tuple<cat, dog, mammal>;
  auto df1 = make_numeric_data_frame<double, Tags>(3);
  df1 = {{1,2,3}, {4,5,6}, {7,8,9}};

  std::cout << df1;
}
