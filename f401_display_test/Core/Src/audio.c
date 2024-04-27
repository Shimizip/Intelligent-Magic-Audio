#include <math.h>
#include <stdlib.h>

#include "audio.h"

#define SAMPLE_RATE     44100   // Sample rate in Hz
#define DURATION        0.01      // Duration of the waveform in seconds
#define AMPLITUDE       32767   // Maximum amplitude (16-bit signed)

void generateWaveform(int16_t* samples, uint32_t numSamples) {
    // Generate a simple sine wave
    double frequency = 440.0;   // Frequency of the sine wave in Hz (A4 note)
    double phaseIncrement = 2 * M_PI * frequency / SAMPLE_RATE;
    double phase = 0.0;

    for (uint32_t i = 0; i < numSamples; i++) {
        samples[i] = (int16_t)(AMPLITUDE * sin(phase));
        phase += phaseIncrement;
        if (phase >= 2 * M_PI) {
            phase -= 2 * M_PI;
        }
    }
}