#ifndef _DEFINES_H_
#define _DEFINES_H_


// Ultrasonic
#define ULTRASONIC_PIN_TRIGGER	2
#define ULTRASONIC_PIN_ECHO		4
#define ULTRASONIC_MAX_DISTANCE	500

// Schwimmschalter
#define SCHWIMMER_PIN			14

// Pumpe Pegel
#define PUMPE_PIN_PEGEL			17

// Circular Buffer
#define CIRCULAR_BUFFER_FILTER_TOP_LIMIT 	500
#define CIRCULAR_BUFFER_SIZE				32
#define CIRCULAR_BUFFER_SHIFTAMOUNT 		5 		// 2^5 = 32


// Basic Defines
#ifndef TRUE
#define TRUE  1
#endif
 
#ifndef FALSE
#define FALSE 0
#endif




#endif 
// _DEFINES_H_