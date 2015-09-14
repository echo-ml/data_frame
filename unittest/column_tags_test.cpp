#include <echo/data_frame/column_tags.h>
#include <echo/test.h>
using namespace echo;
using namespace echo::data_frame;

struct a {};
struct b {};
struct c {};

TEST_CASE("column_tags") {
  using Tags1 = htl::Tuple<a, TagGroup<b, 10>, c>;
  using Tags2 = htl::Tuple<a, b, c>;
  CHECK(get_num_columns<Tags1>() == 12);
  CHECK(get_num_columns<Tags2>() == 3);
}
