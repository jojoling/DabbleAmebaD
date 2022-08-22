
#ifndef _AMEBA_PATCH_h
#define _AMEBA_PATCH_h

//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>

//#include "FreeRTOS.h" //+joehsieh
//#include "task.h"     //+joehsieh
//#include "semphr.h"   //+joehsieh


#if 1//defined(RTL8722DM)



#if 0
#ifdef __cplusplus

#include <algorithm>
#include <cmath>

using std::abs;
using std::isinf;
using std::isnan;
using std::max;
using std::min;
using ::round;

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define max
#define min
#include "wiring_constants.h"
#undef max
#undef min




#ifdef __cplusplus
}
#endif // __cplusplus

#if 0
#include <basic_types.h>

#ifdef ON
#undef ON
#endif
#ifdef OFF
#undef OFF
#endif
#endif



//#define Arduino_STD_PRINTF
//#define STD_PRINTF



#endif // end of #if defined(RTL8722DM)

#endif // end of #ifndef _AMEBA_PATCH_h

