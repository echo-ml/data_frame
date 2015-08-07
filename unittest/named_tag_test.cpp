#include <echo/data_frame/named_tag.h>
#include <echo/test.h>
using namespace echo;
using namespace echo::data_frame;

ECHO_NAMED_TAG("cat", cat_t)
ECHO_NAMED_TAG("dog", dog_t)

TEST_CASE("named_tag") {
  type_equal<cat_t, NamedTag<'c', 'a', 't'>>();
  type_equal<dog_t, NamedTag<'d', 'o', 'g'>>();

  CHECK(to_string(cat_t()) == "cat");
}
