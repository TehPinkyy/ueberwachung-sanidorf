#include "CircularBuffer.h"
#include "Defines.h"
#include <inttypes.h>

void CircularBufferInit( Buffer *buffer, uint8_t size)
{
	buffer->start = 0;
	buffer->count = 0;
	buffer->average = 0;
	buffer->size = size;
	uint8_t i = 0;
	for( ; i < size; i++ )
	{
		buffer->data[ i ] = 0;
	}
}

void CircularBufferFlush( Buffer *buffer ) {
	buffer->start = 0;
	buffer->count = 0;
	buffer->average = 0;
	uint8_t i = 0;	
	for( ; i < buffer->size; i++ )
	{
		buffer->data[ i ] = 0;
	}
}

void CircularBufferPushData( Buffer *buffer, uint16_t data )
{

	// Buffer is empty
	if( buffer->count == 0 )
	{
		//push the first value onto every field
		uint8_t i = 0;
		for ( ; i < buffer->size; i++ )
		{
			buffer->data[ i ] = data;
		}
		++ buffer->count;
		return;
	// Buffer is not empty
	} else {

		uint8_t buffer_end = ( buffer->start + buffer->count ) % buffer->size;
		buffer->data[ buffer_end ] = data;
		// Buffer is full, we need to recalculate the start pointer
		if( buffer->count == buffer->size )
		{
			buffer->start = ( buffer->start + 1 ) % buffer->size;
		// Buffer is not full, so just increment the count
		} else {
			++ buffer->count;
		}
	}
}

int CircularBufferIsEmpty (Buffer *buffer )
{
	if( buffer->count == 0 )
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

void CircularBufferUpdateAverage( Buffer *buffer )
{
	uint8_t i = 0;
	uint32_t sum = 0;
	uint8_t tmpAverage = 0;

	for( ; i < buffer->count; i ++ )
	{
		sum += buffer->data[ (buffer->start + i) % buffer->size ];
	}

	tmpAverage = sum >> CIRCULAR_BUFFER_SHIFTAMOUNT;

	if( tmpAverage > CIRCULAR_BUFFER_FILTER_TOP_LIMIT ) {
		buffer->average = 65535;
	} else {
		buffer->average = tmpAverage;
	}
}

uint16_t CircularBufferGetAverage( Buffer *buffer )
{
	return buffer->average;
}
