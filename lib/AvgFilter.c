#include <inttypes.h>
#include "Controller_AvgFilter.h"

Controller_AvgFilter_t *Controller_AvgFilter_init( Controller_AvgFilter_t *filter, uint8_t size_pow2, int32_t init ) {

	filter->index = 0;
	filter->size_pow2 = size_pow2;

	int i = 0;
	int size = ( 1 << filter->size_pow2 );
	for( ; i < size; i++ ) {
		filter->buffer[ i ] = init;
	}

	return filter;

}

void Controller_AvgFilter_push( Controller_AvgFilter_t *filter, int32_t value ) {

	filter->buffer[ filter->index++ ] = value;
	if( filter->index >= ( 1 << filter->size_pow2 ) ) {
		filter->index = 0;
	}

}

int32_t Controller_AvgFilter_getValue( Controller_AvgFilter_t *filter ) {

	int i = 0;
	int32_t value = 0;
	uint8_t tmpIndex = filter->index;
	int size = ( 1 << filter->size_pow2 );

	for( ; i < size; i++ ) {
		value += filter->buffer[ tmpIndex++ ];
		if( tmpIndex >= size ) {
			tmpIndex = 0;
		}
	}

	return value >> filter->size_pow2;

}
