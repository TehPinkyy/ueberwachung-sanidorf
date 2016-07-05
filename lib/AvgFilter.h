/**
 * This is a general sliding average filter.
 * You can define several buffer sizes for the
 * filter and push values.
 */

#ifndef _AVG_FILTER_H_
#define _AVG_FILTER_H_

#include <inttypes.h>

/**
 * Defines a filter struct with the given size.
 * The new struct is named Controller_Sensor_AvgFilter[size_pow2]_t
 *
 * @param size_pow2 size of the buffer: 2^[size_pow2]
 */
#define DEFINE_FILTER_AVG( name, size_pow2 ) \
	typedef union { \
		struct __attribute__((packed)) { \
			uint8_t pad[ 2 * sizeof( uint8_t ) ]; \
			int32_t buffer[ 1 << size_pow2 ]; \
		}; \
	} name

/**
 * It is not clearly defined in C, which member of a struct
 * comes first and which next, ...
 * To avoid this, we use an union to set defined offsets for
 * member. So we have a dynamic method to use our filter.
 */
typedef union {

	struct __attribute__((packed)) {
		uint8_t pad_index[ 0 ];
		uint8_t index;
	};
	struct __attribute__((packed)) {
		uint8_t pad_size_pow2[ sizeof( uint8_t ) ];
		uint8_t size_pow2;
	};
	struct __attribute__((packed)) {
		uint8_t pad_buffer[ 2 * sizeof( uint8_t ) ];
		int32_t buffer[ 0 ];
	};

} Controller_AvgFilter_t;

/**
 * Init the Sliding average Filter.
 *
 * @param filter The filter struct
 * @param size_pow2 It is important to know the size of the filter
 * @param initValue Init the buffer of the filter with this value
 * @return The initialized filter
 */
Controller_AvgFilter_t *Controller_AvgFilter_init( Controller_AvgFilter_t *, uint8_t, int32_t );

/**
 * Pushes a value into the filter
 *
 * @param filter The filter struct
 * @param value Value to push
 */
void Controller_AvgFilter_push( Controller_AvgFilter_t *, int32_t );

/**
 * Calculates the average of all values of the filter
 *
 * @param filter The filter struct
 * @return Average of the filter values
 */
int32_t Controller_AvgFilter_getValue( Controller_AvgFilter_t * );

#endif

