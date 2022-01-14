#ifndef SDATA_ASSERT_HPP
#define SDATA_ASSERT_HPP

#include <cassert>

// NOTE: Internal assertion system, must only be used for inner library errors.
// External errors (bad syntax, parsing exceptions, bad cast, ...) are managed via exceptions

#define SDATA_ASSERT(expression, message) assert(static_cast<bool>(expression) && message)

#endif
