#pragma once

#include <echo/preprocessor.h>
#include <echo/expand_evaluate.h>
#include <utility>
#include <iostream>
#include <string>
#include <sstream>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// NamedTag
//------------------------------------------------------------------------------
template <char... Chars>
struct NamedTag {};

//------------------------------------------------------------------------------
// operator<<
//------------------------------------------------------------------------------
template <char... Chars>
std::ostream& operator<<(std::ostream& out, NamedTag<Chars...>) {
  expand_evaluate((out << Chars, 0)...);
  return out;
}

//------------------------------------------------------------------------------
// to_string
//------------------------------------------------------------------------------
template <char... Chars>
std::string to_string(NamedTag<Chars...> tag) {
  std::ostringstream oss;
  oss << tag;
  return oss.str();
}

//------------------------------------------------------------------------------
// ECHO_NAMED_TAG
//------------------------------------------------------------------------------
#define ECHO_NAMED_TAG(STRING, TYPE_NAME)                                   \
  template <std::size_t... Indexes>                                         \
  auto ECHO_CONCATENATE(ECHO_CONCATENATE(detail_named_tag_impl, TYPE_NAME), \
                        __LINE__)(std::index_sequence<Indexes...>)          \
      ->echo::data_frame::NamedTag<(STRING)[Indexes]...>;                   \
  using TYPE_NAME = decltype(ECHO_CONCATENATE(                              \
      ECHO_CONCATENATE(detail_named_tag_impl, TYPE_NAME),                   \
      __LINE__)(std::make_index_sequence<sizeof(STRING) - 1>()));
}
}
