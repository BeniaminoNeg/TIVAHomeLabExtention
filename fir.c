/*
 * fir.c
 *
 *  Created on: Dec 24, 2016
 *      Author: albertopetrucci
 */


/* Implementation of FIR filter */

#include <stdlib.h>
#include <string.h>
#include "fir.h"

int circBufPop(circBuf_t *c)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (c->head == c->tail)
        return -1;  // quit with an error

    *data = c->buffer[c->tail];
    c->buffer[c->tail] = 0;  // clear the data (optional)

    int next = c->tail + 1;
    if(next >= c->maxLen)
        next = 0;

    c->tail = next;

    return 0;
}

int circBufPush(circBuf_t *c, uint32_t data)
{
    int next = c->head + 1;
    if (next >= c->maxLen){
        next = 0;
        circBufPop(c);
        next = 1;
    }

    // Cicular buffer is full
    if (next == c->tail)
        return -1;  // quit with an error

    c->buffer[c->head] = data;
    c->head = next;
    return 0;
}


/* Initialize FIR_filter struct
 * Params:
 *  int length -- number of filter coefficients
 *  double *h  -- array of filter coefficients
 * Returns:
 *  FIR_filter *p -- on success
 *  NULL          -- on error
 */

void FIR_init(FIR_filter *filter) {
	filter->last_filtered_sample = 0;
	filter->valid = 0;
    filter->length = (int)(sizeof(h)/sizeof(double)) ;
    filter->count = 0;
    filter->h = h;
    int i;
    for (i=0; i < filter->length; ++i) {
    		filter->delay_line[i] = 0;
    }

    filter->delay_line_ptr = {filter->delay_line, 0, 0, filter->length};
}

void FIR_put_sample(FIR_filter *filter, uint32_t input)
{
	circBufPush(&filter->delay_line_ptr, input)
	int i = 0;
	//QUEUE
	for (i=0; i < filter->length; ++i) {
	  	filter->delay_line[i] = filter->delay_line[i+1];
	}
	filter->delay_line[filter->length-1] = input;
}

/* Get next filtered sample of input signsl
 * Params:
 *  FIR_filter *filter -- pointer to filter structure
 *  double input       -- input signl sample
 * Returns:
 *  double result -- filtered sample
 */
int FIR_get_sample(FIR_filter *filter) {
	double result = 0.0;
    int i = 0;

    for (i=0; i < filter->length; ++i) {
    	result += filter->h[i] * filter->delay_line[i];
    }
    /*
    filter->delay_line[filter->count] = input;
    for (i=0; i < filter->length; ++i) {
        result += filter->h[i] * filter->delay_line[(filter->length-1) - i];
    }
    if(++filter->count >= filter->length)
        filter->count = 0;
     */
	filter->last_filtered_sample = (int)result;
    filter->valid = 1;
    return (int)result;
}

/* Free memory, allocated by filter
 * Params:
 *  FIR_filter *filter -- pointer to FIR_filter struct
 */
void FIR_destroy(FIR_filter *filter) {
    free(filter->delay_line);
    free(filter);
}

