#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

//Arduino Legacy
#if defined (ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif



#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include "Defines.h"


typedef struct
{
	uint8_t start;
	uint8_t count;
	uint16_t data[ CIRCULAR_BUFFER_SIZE ];
	uint16_t average;
	uint8_t size;
} Buffer;

/**
 * Initializes the Buffer with the given size
 *
 * @param Buffer the given Buffer
 * @param size the size of the buffer
 */
void CircularBufferInit( Buffer *buffer, uint8_t size );

/**
 * Flush the Buffer
 *
 * The Value 0 will be the default value for the 'buffer was flushed/inited'
 */
void CircularBufferFlush( Buffer *buffer );


/**
 * Adds the given Data to the given Buffer
 *
 * @param Buffer the given Buffer
 * @param edata the given data element
 */
void CircularBufferPushData( Buffer *buffer, uint8_t data );

/**
 * Check if the Buffer has no Elements
 *
 * @param Buffer the given Buffer
 *
 * @return TRUE if the Buffer is empty and FALSE if not
 */
int CircularBufferIsEmpty( Buffer *buffer );

/**
 * Update the Average Value by traversing the whole Buffer
 *
 * @param Buffer the given Buffer
 */
void CircularBufferUpdateAverage( Buffer *buffer );

/**
 * Getter function for the Average Value
 *
 * @param Sensor_Buffer the given Buffer
 *
 * @return the Average Value
 */
uint16_t CircularBufferGetAverage( Buffer *buffer );


#endif
// _CIRCULAR_BUFFER_H_