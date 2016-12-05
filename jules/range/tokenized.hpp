// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#define JULES_RANGE_TOKENIZED_H
#ifndef JULES_RANGE_TOKENIZED_H

#include <boost/range/iterator_range.hpp>
#include <regex>

namespace jules
{
namespace detail
{
using std::regex_constants::match_flag_type;

template <class Submatch> struct regex_holder {
  const std::regex& re;
  Submatch sub;
  match_flag_type flag;

  regex_holder(const std::regex& re, const Submatch& sub, match_flag_type flag) : re(re), sub(sub), flag(flag) {}

private:
  void operator=(const regex_holder&);
};

struct regex_forwarder {
  regex_holder<int> operator()(const std::regex& re, int submatch = 0,
                               match_flag_type f = std::regex_constants::match_default) const
  {
    return regex_holder<int>(re, submatch, f);
  }

  regex_holder<int> operator()(const std::regex&& re, int submatch = 0,
                               match_flag_type f = std::regex_constants::match_default) const = delete;

  template <class Submatch>
  regex_holder<Submatch> operator()(const std::regex& re, const Submatch& sub,
                                    match_flag_type f = std::regex_constants::match_default) const
  {
    return regex_holder<Submatch>(re, sub, f);
  }

  template <class Submatch>
  regex_holder<Submatch> operator()(const std::regex&& re, const Submatch& sub,
                                    match_flag_type f = std::regex_constants::match_default) const = delete;
};
} // namespace detail

namespace adaptors
{
static const detail::regex_forwarder tokenized = {};

template <typename BidirectionalRange, class Submatch>
auto tokenize(BidirectionalRange& rng, const std::regex& re, const Submatch& sub, detail::match_flag_type f)
{
  using BidirIt = typename boost::range_iterator<BidirectionalRange>::type;
  using TokenIt = std::regex_token_iterator<BidirIt>;

  return boost::make_iterator_range(TokenIt(std::begin(rng), std::end(rng), re, sub, f), TokenIt());
}

template <typename BidirectionalRange, class Submatch>
auto tokenize(const BidirectionalRange& rng, const std::regex& re, const Submatch& sub, detail::match_flag_type f)
{
  using BidirIt = typename boost::range_iterator<const BidirectionalRange>::type;
  using TokenIt = std::regex_token_iterator<BidirIt>;

  return boost::make_iterator_range(TokenIt(std::begin(rng), std::end(rng), re, sub, f), TokenIt());
}
} // namespace adaptors

template <class BidirectionalRng, class Submatch> auto operator|(BidirectionalRng&& r, const detail::regex_holder<Submatch>& f)
{
  return adaptors::tokenize(std::forward<BidirectionalRng>(r), f.re, f.sub, f.flag);
}

} // namespace jules

#endif // JULES_RANGE_TOKENIZED_H
