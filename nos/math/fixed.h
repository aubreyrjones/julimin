//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_FIXED_H
#define JULIMIN_FIXED_H

#include "fixed_impl/fixed_class.h"
#include "fixed_impl/fixed_func.h"

namespace nos {

template <int precision>
using fixed_point = fixedpoint::fixed_point<precision>;

using fixed = fixedpoint::fixed_point<16>;

}


constexpr nos::fixed_point<12> operator "" _x12(long double d) {
	return nos::fixed_point<12>((int32_t) (d * (1 << nos::fixed::precision)), true);
}

constexpr nos::fixed_point<20> operator "" _x20(long double d) {
	return nos::fixed_point<20>((int32_t) (d * (1 << nos::fixed::precision)), true);
}

constexpr nos::fixed operator "" _f(long double d) {
	return nos::fixed((int32_t) (d * (1 << nos::fixed::precision)), true);
}

constexpr nos::fixed operator "" _f(unsigned long long int i) {
	return nos::fixed((int32_t) (0xffffffff & i));
}

#endif //JULIMIN_FIXED_H
