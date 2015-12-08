//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_TYPES_H
#define JULIMIN_TYPES_H

namespace nos {

template <typename T>
T bitmask(T const& nBits) {
	return ((1 << nBits) - 1) ^ (1 << nBits);
}

typedef
void (*VoidFunc)();

}

#endif //JULIMIN_TYPES_H
