//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_CHIP_H
#define JULIMIN_CHIP_H

#include <chip_support.h>

namespace nos {

	__attribute__ ((section(".ramnvic")))
	extern NVICTable _nvicTable;
}

#endif //JULIMIN_CHIP_H
