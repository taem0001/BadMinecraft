#pragma once

#define CASTTOFLOAT(x) static_cast<float>(x)
#define SQUARE(x)	   (x * x)
#define EUCLDIST(coord1, coord2)                                               \
	(SQUARE((coord1.x - coord2.x)) + SQUARE((coord1.z - coord2.z)))