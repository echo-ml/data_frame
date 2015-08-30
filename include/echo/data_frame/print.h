#pragma once

#include <echo/data_frame/concept.h>
#include <echo/data_frame/homogenous_data_frame_traits.h>
#include <echo/data_frame/matrix_conversion.h>
#include <echo/utility/ascii_table.h>

#define DETAIL_NS detail_print

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// convert_column_tag_to_string
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Tag, CONCEPT_REQUIRES(concept::named_tag<Tag>())>
std::string convert_column_tag_to_string(int i, Tag tag) {
  using std::to_string;
  return to_string(tag);
}

template <class Tag, CONCEPT_REQUIRES(!concept::named_tag<Tag>())>
std::string convert_column_tag_to_string(int i, Tag) {
  return std::to_string(i);
}
}

//------------------------------------------------------------------------------
// convert_column_tags_to_strings
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <std::size_t... Indexes, class... Tags>
auto convert_column_tags_to_strings_impl(std::index_sequence<Indexes...>,
                                         htl::Tuple<Tags...>) {
  std::vector<std::string> result = {
      convert_column_tag_to_string(Indexes, Tags())...};
  return result;
}

template <class... Tags>
auto convert_column_tags_to_strings(htl::Tuple<Tags...> tags) {
  return convert_column_tags_to_strings_impl(std::index_sequence_for<Tags...>(),
                                             tags);
}
}

//------------------------------------------------------------------------------
// make_header_row
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <std::size_t... Indexes, class... Tags>
auto make_header_row_impl(std::index_sequence<Indexes...>,
                          htl::Tuple<Tags...>) {
  using ascii_table::Cell;
  using ascii_table::Row;
  return Row(
      {Cell("#"), Cell(convert_column_tag_to_string(Indexes, Tags()))...});
}

template <class... Tags>
auto make_header_row(htl::Tuple<Tags...> tags) {
  return make_header_row_impl(std::index_sequence_for<Tags...>(), tags);
}
}

//------------------------------------------------------------------------------
// make_row
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class DataFrame,
          CONCEPT_REQUIRES(
              concept::homogenous_data_frame<DataFrame>() &&
              echo::concept::printable<
                  homogenous_data_frame_traits::value_type<DataFrame>>())>
auto make_row(index_t row_index, const DataFrame& data_frame) {
  using ascii_table::Cell;
  using ascii_table::Row;
  std::vector<Cell> cells = {Cell(std::to_string(row_index))};
  auto matrix = make_matrix_cview(data_frame);
  for (index_t column_index = 0; column_index < get_num_columns(data_frame);
       ++column_index) {
    std::ostringstream oss;
    oss << matrix(row_index, column_index);
    cells.emplace_back(Cell(oss.str()));
  }
  return Row(std::move(cells));
}
}

//------------------------------------------------------------------------------
// operator<<
//------------------------------------------------------------------------------
template <class DataFrame,
          CONCEPT_REQUIRES(
              concept::homogenous_data_frame<DataFrame>() &&
              echo::concept::printable<
                  homogenous_data_frame_traits::value_type<DataFrame>>())>
std::ostream& operator<<(std::ostream& out, const DataFrame& data_frame) {
  using namespace DETAIL_NS;
  using ascii_table::Row;
  using ascii_table::Table;
  Table table;
  table.emplace_back(make_header_row(typename DataFrame::column_tags()));
  table.emplace_back(Row());
  for (index_t row_index = 0; row_index < get_num_rows(data_frame);
       ++row_index) {
    table.emplace_back(make_row(row_index, data_frame));
  }
  print_table(out, table);
  return out;
}
}
}

#undef DETAIL_NS
