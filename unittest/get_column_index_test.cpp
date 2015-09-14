#include <echo/data_frame/get_column_index.h>
#include <echo/test.h>
using namespace echo;
using namespace echo::data_frame;

struct a {};
struct b {};
struct c {};

TEST_CASE("get_column_index") {
  using Tags1 = htl::Tuple<a, b>;
  using Tags2 = htl::Tuple<a, TagGroup<b, 10>, c>;
  CHECK(get_column_index<Tags1, a>() == 0);
  CHECK(get_column_index<Tags1, b>() == 1);
  CHECK(get_column_index<Tags2, IndexedTag<b, 4>>() == 5);
  CHECK(get_column_index<Tags2, c>() == 11);
}
