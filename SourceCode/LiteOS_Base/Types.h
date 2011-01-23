

#ifndef TYPESH
#define TYPESH


typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef int int16_t;
typedef unsigned int uint16_t;

typedef long int32_t;
typedef unsigned long uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef unsigned int size_t;



typedef unsigned char bool;

enum  {
  FALSE = 0, 
  TRUE = 1
};

enum  {
  FAIL = 0, 
  SUCCESS = 1
};

typedef uint8_t  result_t;

inline 
result_t rcombine(result_t r1, result_t r2);

result_t rcombine3(result_t r1, result_t r2, result_t r3);

enum {
  NULL = 0x0
};

enum {
	 MILLISECOND = 1,
	 SECOND = 1000,
	 MINUTE = 60000
	};
	
	
uint16_t CURRENT_NODE_ID;
	

#endif
