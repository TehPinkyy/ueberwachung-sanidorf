#include "Controller_Sensor_Buffer.h"

#include <inttypes.h>

void Sensor_Buffer_Init( Sensor_Buffer *buffer)
{
	buffer->start = 0;
	buffer->count = 0;
	buffer->average = 0;
	int i = 0;
	for( ; i < SENSOR_SONIC_BUFFER_SIZE; i++ )
	{
		buffer->data[ i ] = 0;
	}
}

void Sensor_Buffer_Flush( Sensor_Buffer *buffer ) {
	buffer->start = 0;
	buffer->count = 0;
	buffer->average = 0;
	int i = 0;	
	for( ; i < SENSOR_SONIC_BUFFER_SIZE; i++ )
	{
		buffer->data[ i ] = 0;
	}
}

void Sensor_Buffer_PushData( Sensor_Buffer *buffer, uint8_t data )
{

	//push the first value onto every field
	if( buffer->count == 0 )
	{
		int i = 0;
		for ( ; i < SENSOR_SONIC_BUFFER_SIZE; i++ )
		{
			buffer->data[ i ] = data;
		}
		++ buffer->count;
		return;
	} else {

		int buffer_end = ( buffer->start + buffer->count ) % SENSOR_SONIC_BUFFER_SIZE;
		buffer->data[ buffer_end ] = data;
		//the buffer is full, we need to recalculate the start pointer
		if( buffer->count == SENSOR_SONIC_BUFFER_SIZE )
		{
			buffer->start = ( buffer->start + 1 ) % SENSOR_SONIC_BUFFER_SIZE;
		//the buffer is not full, so just set the count + 1
		} else {
			++ buffer->count;
		}
	}
}

int Sensor_Buffer_IsNotEmpty (Sensor_Buffer *buffer )
{
	return buffer->count;
}

void Sensor_Buffer_UpdateAverage( Sensor_Buffer *buffer )
{
	int i = 0;
	uint32_t sum = 0;
	uint8_t tmpAverage = 0;

	for( ; i < buffer->count; i ++ )
	{
		sum += buffer->data[ (buffer->start + i) % SENSOR_SONIC_BUFFER_SIZE ];
	}

	tmpAverage = sum >> SENSOR_SONIC_BUFFER_SHIFTAMOUNT;

	if( tmpAverage > SENSOR_SONIC_FILTER_TOPLIMIT ) {
		buffer->average = 255;
	} else {
		buffer->average = tmpAverage;
	}
}

uint8_t Sensor_Buffer_GetAverage( Sensor_Buffer *buffer )
{
	return buffer->average;
}
