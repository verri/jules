#ifndef JULES_RANGE_RANGE_H
#define JULES_RANGE_RANGE_H

#include "util/type.hpp"

#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/uniqued.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/istream_range.hpp>

#include <range/tokenized.hpp>

#include <iterator>
#include <type_traits>

namespace jules
{

template <typename Range, typename = void> struct range_traits {
};

template <typename Range>
struct range_traits<Range,
                    std::enable_if_t<!std::is_const<Range>::value,
                                     detail::void_t<typename std::remove_reference_t<Range>::iterator>>> {
    using iterator_type = typename std::remove_reference_t<Range>::iterator;
    using iterator_traits = std::iterator_traits<iterator_type>;
};

template <typename Range>
struct range_traits<
    Range, std::enable_if_t<std::is_const<Range>::value,
                            detail::void_t<typename std::remove_reference_t<Range>::const_iterator>>> {
    using iterator_type = typename std::remove_reference_t<Range>::const_iterator;
    using iterator_traits = std::iterator_traits<iterator_type>;
};

template <typename Range> using range_iterator_t = typename range_traits<Range>::iterator_type;
template <typename Range> using range_value_t = typename range_traits<Range>::iterator_traits::value_type;

namespace range
{
using boost::copy;
using boost::make_iterator_range;

using boost::begin;
using boost::end;

using boost::size;

using boost::adaptors::index;
using boost::adaptors::stride;
using boost::adaptors::transform;
using boost::adaptors::unique;
} // namespace range

namespace adaptors
{
using boost::adaptors::indexed;
using boost::adaptors::strided;
using boost::adaptors::transformed;
using boost::adaptors::uniqued;
} // namespace adaptors
} // namespace jules

#endif // JULES_RANGE_RANGE_H
