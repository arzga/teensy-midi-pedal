#ifndef TABLEINTERPOLATOR_CPP
#define TABLEINTERPOLATOR_CPP

#include <Arduino.h>

// Tools
#define ARRAYLEN(x) sizeof(x)/sizeof(x[0])

/**
* Returns a value based on a response curve defined by tabular data.
* Some pre-defined graceful curves (gaussian, for example) are pre-defined as part of class
* @author arzga
*
* INTERPOLATE USAGE:
*
* float outputValue = TableInterpolator.interpolateLinearf(TableInterpolator.gaussian, 30f, 0f, 100f);
*
* Example:
*  - Output curve is defined by values in a float array (in this gaussian curve example: values ranging from 0.0f via 1.0f back to 0.0f).
*  - We want the interpolateLinearf to interpolate and return the value at position 30 on the curve
*  - The input value range in this call is defined as 0 to 100.
*  - The output curve is "stretched" to fit the input range. The table defining the curve may have an arbitrary number of values, they just enhance the resolution of the curve.
*  - A linear interpolated value is returned based on two neighbouring tabular values.
*  - If pos < min, first table value is returned
*  - If pos > max, last table value is returned 
*
* EXTRAPOLATE USAGE:
*
* float outputValue = TableInterpolator.extrapolateLinearf(TableInterpolator.gaussian, 30f, 0f, 100f);
*
* To extrapolate output values outside input range, use extrapolateLinearf. Within input range, it functions exactly as interpolateLinearf.
* Output values outside input range are linearly extrapolated based on two first/last values of tabular data.
* 
**/

static const float fadeIn[] = {0.0, 1.0};
static const float fadeOut[] = {1.0, 0.0};
// __..-´´-..__    0..1..0
static const float gaussian[] = {0.0, 0.007822, 0.01286, 0.020581, 0.032067, 0.048637, 0.071812, 0.10322, 0.14443, 0.196733, 0.260872, 0.336749, 0.423169, 0.517666, 0.616473, 0.714672, 0.806545, 0.886091, 0.94767, 0.986653, 1, 0.986653, 0.94767, 0.886091, 0.806545, 0.714672, 0.616473, 0.517666, 0.423169, 0.336749, 0.260872, 0.196733, 0.14443, 0.10322, 0.071812, 0.048637, 0.032067, 0.020581, 0.01286, 0.007822, 0};

// _____-´´´´´´    0..1
static const float easeInOut[] = {0.0, 0.00092, 0.001585, 0.002661, 0.004359, 0.006966, 0.010864, 0.016538, 0.024579, 0.035672, 0.050571, 0.070051, 0.094849, 0.125579, 0.162654, 0.206202, 0.255996, 0.311429, 0.371505, 0.434891, 0.5, 0.565109, 0.628495, 0.688571, 0.744004, 0.793798, 0.837346, 0.874421, 0.905151, 0.929949, 0.949429, 0.964328, 0.975421, 0.983462, 0.989136, 0.993034, 0.995641, 0.997339, 0.998415, 0.99908, 1};

// __--------´´    -1..0..1
static const float hyperbolic[] = {-1, -0.895317, -0.792429, -0.693037, -0.598669, -0.510606, -0.429835, -0.357023, -0.29251, -0.23633, -0.188245, -0.147794, -0.114348, -0.087169, -0.06546, -0.048418, -0.035268, -0.025297, -0.017864, -0.012419, 0.0, 0.012419, 0.017864, 0.025297, 0.035268, 0.048418, 0.06546, 0.087169, 0.114348, 0.147794, 0.188245, 0.23633, 0.29251, 0.357023, 0.429835, 0.510606, 0.598669, 0.693037, 0.792429, 0.895317, 1};

// _.--´´´´--._    0..1..0
static const float ballistic[] = {0.0, 0.36, 0.64, 0.84, 0.96, 1, 0.96, 0.84, 0.64, 0.36, 0};

// _..---´´´´´´    0..1
static const float saturate[] = {0.0, 0.19, 0.36, 0.51, 0.64, 0.75, 0.84, 0.91, 0.96, 0.99, 1};

// _.--´´´´´´´´    0..1
static const float saturate3[] = {0.0, 0.271, 0.488, 0.657, 0.784, 0.875, 0.936, 0.973, 0.992, 0.999, 1};

// ____...---´    0..1
static const float exponential[] = {0.0, 0.01, 0.04, 0.09, 0.16, 0.25, 0.36, 0.49, 0.64, 0.81, 1};

// _____...--´    0..1
static const float exp3[] = {0.0, 0.001, 0.008, 0.027, 0.064, 0.125, 0.216, 0.343, 0.512, 0.729, 1};

// ________..-´    0..1
static const float exp4[] = {0.0, 0.0001, 0.0016, 0.0081, 0.0256, 0.0625, 0.1296, 0.2401, 0.4096, 0.6561, 1};

// ________..-´    0..1
// static const float ledResponse[] = {0.0, 0.02, 0.04, 0.06, 0.08, 0.1, 0.1296, 0.2401, 0.4096, 0.6561, 1};

static const float ledResponse[] = {
  0.000000, 0.006479, 0.011879, 0.020518, 0.032397, 0.048596, 0.069114, 0.095032, 0.125270, 0.163067, 
  0.207343, 0.258099, 0.317495, 0.385529, 0.462203, 0.548596, 0.644708, 0.751620, 0.870410, 1.000000, 
};

// _..------´´    1..127
static const float midiKnee[] = {1.0, 6.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 75.0, 92.0, 127};

// _..------´´    1..127
// static const float midiSuperSoftLinear[] = {1.0, 16, 56, 90, 127};
static const float midiSuperSoftLinear[] = {1.0, 12, 60, 96, 127};

// _..------´´    1..127
static const float midiSoftLinear[] = {1.0, 24, 64, 96, 127};

// __...---´´´    1..127
static const float midiLinear[] = {1.0, 127};

// _____...--´    0..127
static const float midiControl[] = {0.0, 50, 127};

// _____...--´    1..127
static const float midiVelo[] = {1.0, 127};

static const float inflictedXTalk[] = {1.0, 1.0};


class TableInterpolator {
  public:
  
  // New static functions --------------------------------------------------------

  /**
   * Look up a matching y for x from table of "y-values" evenly mapped to x-axis range min..max
   * Linearly interpolate in-between values.
   * Repeat table edge y values for x values outside min..max range
   * @param table  Table of float values of at least 2 values
   * @param x
   * @param min  X coordinate of first value in table. If x < min, return first value in table
   * @param max  X coordinate of last value in table. If x > max, return first last in table
   * @return linearly interpolated value "y" corresponding to tabular data mapped to range min..max
   */

  static float interpolateLinearf(const float table[], int length, float x, float min, float max) {
    if (length == 1) return table[0];
    if (x < min) return table[0];
    if (x > max) return table[length-1];
    
    float step = (max - min) / (length - 1);
    if (step <= 0) return table[0]; 
    
    float index = (x - min) / step;
    int index_floor = floor(index);
    int index_ceil = ceil(index);
    float offset = index - index_floor;
    
    return ((1.0 - offset) * table[index_floor]) + (offset * table[index_ceil]);
  }  
};

#endif
