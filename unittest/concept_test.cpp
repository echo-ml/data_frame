#include <echo/data_frame/concept.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::data_frame;

static constexpr struct A {} a{};
static constexpr struct B {} b{};
static constexpr struct C {} c{};

TEST_CASE("concept") {
  using Tags = htl::Tuple<A, B>;
  CHECK(data_frame::concept::column_tags<Tags>());
  CHECK(data_frame::concept::column_tag<Tags, A>());
  CHECK(!data_frame::concept::column_tag<Tags, C>());
}
