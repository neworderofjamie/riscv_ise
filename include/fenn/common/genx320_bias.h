#pragma once

// Standard C includes
#include <cstdint>

#define BIAS_REGISTER_DEF(NAME, ADDRESS, MIN, MAX, DEFAULT) \
	struct NAME	\
	{	\
		static inline const uint16_t address = ADDRESS;	\
		static inline const uint8_t min = MIN;	\
		static inline const uint8_t max = MAX;	\
		static inline const uint8_t def = DEFAULT;	\
	}

namespace FeNN::Common::Bias
{
BIAS_REGISTER_DEF(FO, 		0x1004, 19, 50, 	34);
BIAS_REGISTER_DEF(HPF, 		0x1100, 0, 	127, 	40);
BIAS_REGISTER_DEF(DiffOn, 	0x1104, 24, 78, 	25);
BIAS_REGISTER_DEF(Diff, 	0x1108, 41, 51, 	51);
BIAS_REGISTER_DEF(DiffOff,	0x110C, 19, 127,	28);
BIAS_REGISTER_DEF(Refr, 	0x1114, 0,	127, 	10);
}

#undef BIAS_REGISTER_DEF