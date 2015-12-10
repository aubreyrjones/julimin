//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_TYPES_H
#define JULIMIN_TYPES_H

namespace nos {

/** Generate a bitmask with the least significant n bits filled. */
template <typename T>
constexpr T bitmask(T const nBits) {
	return ((1 << nBits) - 1) ^ (1 << nBits);
}

/** Raise base to a positive integer exponent. */
template <typename T>
constexpr T positive_power(T const base, int const exponent) {
	return (exponent == 0) ? 1 : base * positive_power(base, exponent - 1);
}

typedef void (*VoidFunc)();

}

#endif //JULIMIN_TYPES_H
