#ifndef LOWPASS_16KFILTER_H_
#define LOWPASS_16KFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 44100 Hz

* 0 Hz - 15000 Hz
  gain = 1
  desired ripple = 1 dB
  actual ripple = 0.7142538833166108 dB

* 16000 Hz - 22000 Hz
  gain = 0
  desired attenuation = -50 dB
  actual attenuation = -50.44713957642392 dB

*/

#define LOWPASS_16KFILTER_TAP_NUM 83

typedef struct {
  float history[LOWPASS_16KFILTER_TAP_NUM];
  unsigned int last_index;
} lowpass_16kFilter;

void lowpass_16kFilter_init(lowpass_16kFilter* f);
void lowpass_16kFilter_put(lowpass_16kFilter* f, float input);
float lowpass_16kFilter_get(lowpass_16kFilter* f);

#endif