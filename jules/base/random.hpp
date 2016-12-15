// Copyright (c) 2016 Filipe Verri <filipeverri@gmail.com>

#ifndef JULES_BASE_RANDOM_H
#define JULES_BASE_RANDOM_H

#include <random>

namespace jules
{

static thread_local auto random_engine = std::mt19937{};

} // namespace jules

#endif // JULES_BASE_RANDOM_H
