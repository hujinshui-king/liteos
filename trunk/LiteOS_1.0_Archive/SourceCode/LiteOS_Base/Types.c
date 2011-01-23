#include "Types.h"

inline 
result_t rcombine(result_t r1, result_t r2)
{
  return r1 == FAIL ? FAIL : r2;
}


inline 

result_t rcombine3(result_t r1, result_t r2, result_t r3)
{
  return rcombine(r1, rcombine(r2, r3));
}




