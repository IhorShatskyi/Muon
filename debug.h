#ifndef DEBUG_H
#define DEBUG_H

#include "complex.h"
#include "list.h"

extern list Sempls;

extern const struct debugFunctions {

	void (* Show)(complex);

} Debug;

#endif
