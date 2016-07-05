#ifndef _SENSOR_BUFFER_H_
#define _SENSOR_BUFFER_H_


#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#define SENSOR_SONIC_FILTER_TOPLIMIT 100
#define SENSOR_SONIC_BUFFER_SIZE 32
#define SENSOR_SONIC_BUFFER_SHIFTAMOUNT 5

typedef struct
{
	int start;
	int count;
	uint8_t data[ SENSOR_SONIC_BUFFER_SIZE ];
	uint8_t average;
} Sensor_Buffer;

/**
 * Initializes the Buffer with the given size
 *
 * @param Sensor_Buffer the given Buffer
 * @param size the size of the buffer
 */
void Sensor_Buffer_Init( Sensor_Buffer *buffer );

/**
 * Flush the Buffer
 * just a feature for Algo to handle failsafes
 *
 * From now on the Value 0 will be the default value for the buffer was flushed/inited
 *
 * @param port_id The Sensor to which the Buffer belongs
 */
void Sensor_Buffer_Flush( Sensor_Buffer *buffer );


/**
 * Adds the given Element to the given Buffer
 *
 * @param Sensor_Buffer the given Buffer
 * @param element the given data element
 */
void Sensor_Buffer_PushData( Sensor_Buffer *buffer, uint8_t data );

/**
 * Check if the Buffer has no Elements
 *
 * @param Sensor_Buffer the given Buffer
 *
 * @return 0 if the Buffer is empty
 */
int Sensor_Buffer_IsNotEmpty( Sensor_Buffer *buffer );

/**
 * Update the Average Value by traversing the whole Buffer
 *
 * @param Sensor_Buffer the given Buffer
 */
void Sensor_Buffer_UpdateAverage( Sensor_Buffer *buffer );

/**
 * Getter function for the Average Value
 *
 * @param Sensor_Buffer the given Buffer
 *
 * @return the Average Value
 */
uint8_t Sensor_Buffer_GetAverage( Sensor_Buffer *buffer );

#endif // _SENSOR_BUFFER_H_
