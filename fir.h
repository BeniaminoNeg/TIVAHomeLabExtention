/*
 * fir.h
 *
 *  Created on: Dec 24, 2016
 *      Author: albertopetrucci
 */
/* FIR filter */

#ifndef FIR_H_
#define FIR_H_

#include <stdint.h>

#define N_FIR 21 //7 //33 //40
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 2000 Hz

* 0 Hz - 400 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 4.1393894966071585 dB

* 500 Hz - 1000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -40.07355419274887 dB

*/

static double h[N_FIR] = {
  -0.02010411882885732,
  -0.05842798004352509,
  -0.061178403647821976,
  -0.010939393385338943,
  0.05125096443534972,
  0.033220867678947885,
  -0.05655276971833928,
  -0.08565500737264514,
  0.0633795996605449,
  0.310854403656636,
  0.4344309124179415,
  0.310854403656636,
  0.0633795996605449,
  -0.08565500737264514,
  -0.05655276971833928,
  0.033220867678947885,
  0.05125096443534972,
  -0.010939393385338943,
  -0.061178403647821976,
  -0.05842798004352509,
  -0.02010411882885732
};


/*
static double h[N_FIR] = {
		0.0028083001729576657,
		-0.1667994339102878,
		0.42933436197795954,
		0.4321426621509179,
		0.42933436197795954,
		-0.1667994339102878,
		0.0028083001729576657,
};
static double h[N_FIR] = {
		0.004589972864584899,
		0.01708179771754989,
		0.00848286227206236,
		-0.009028258250168864,
		-0.01389446208229176,
		0.0073837949077052226,
		0.022131649890471866,
		-0.0010160148859200805,
		-0.031773595013386044,
		-0.012213811683435565,
		0.041277579305986495,
		0.036864746197326374,
		-0.04931502207050167,
		-0.08869657166183915,
		0.05468201162918769,
		0.3122669399250181,
		0.44342883486942547,
		0.3122669399250181,
		0.05468201162918769,
		-0.08869657166183915,
		-0.04931502207050167,
		0.036864746197326374,
		0.041277579305986495,
		-0.012213811683435565,
		-0.031773595013386044,
		-0.0010160148859200805,
		0.022131649890471866,
		0.0073837949077052226,
		-0.01389446208229176,
		-0.009028258250168864,
		0.00848286227206236,
		0.01708179771754989,
		0.004589972864584899,
};
static double h[N_FIR] = {
		-0.004314029307,-0.013091321622,-0.016515087727,
		-0.006430584433, 0.009817876267, 0.010801880238,
		-0.006567413713,-0.016804829623, 0.000653253913,
		 0.022471280087, 0.010147131468,-0.025657740989,
		-0.026558960619, 0.023048392854, 0.050385290390,
		-0.009291203588,-0.087918503442,-0.033770330014,
		 0.187334796517, 0.401505729848, 0.401505729848,
		 0.187334796517,-0.033770330014,-0.087918503442,
		-0.009291203588, 0.050385290390, 0.023048392854,
		-0.026558960619,-0.025657740989, 0.010147131468,
		 0.022471280087, 0.000653253913,-0.016804829623,
		-0.006567413713, 0.010801880238, 0.009817876267,
		-0.006430584433,-0.016515087727,-0.013091321622,
		-0.004314029307
};
*/

typedef struct FIR_filter {
    int length;
    int index;
    double *h;
    uint32_t delay_line[N_FIR];
    uint8_t valid;
    int last_filtered_sample;
} FIR_filter;

void FIR_init(FIR_filter *filter);
void FIR_put_sample(FIR_filter *filter, uint32_t input);
int FIR_get_sample(FIR_filter *filter);
void FIR_destroy(FIR_filter *filter);

#endif /* FIR_H_ */
