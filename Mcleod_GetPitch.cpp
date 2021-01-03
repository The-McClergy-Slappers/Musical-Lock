//
//  GetPitch.cpp
//  J-Tuner
//
//  Implements Mcleod Pitch Method, by Philip McLeod, Geoff Wyvill.
//  See http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf for details.
//
//  Created by Joel Tecson on 2018-10-31.
//  Copyright © 2018 bignerdranch. All rights reserved.
//

#include "GetPitch.hpp"
#include <stdio.h>
#include <math.h>
#include <vector>


const float freqDenom = 0.0250858; // Used to convert frequency to note on midi scale
const int windowSize = 1024;
const float kFactor = 0.85;

// Returns note of the sampled waveform 0-indexed, starting from C0
// Data      -   Waveform data
// Size      -   Number of samples in data
// sampRate  -   Sampling frequency in Hz.
double getPitch(float data[], int size, int sampRate)
{
    float mpdData[size];
    for(int i = 0; i < size; i++)
    {
        mpdData[i] = getNSDF(0, i, windowSize, data);
    }
    return log10f(getFrequency(mpdData, size, sampRate)/27.5)/freqDenom + 9;
}

// Returns the frequency of the sampled data in Hz.
// Data      -   Waveform data
// Size      -   Number of samples in data
// sampRate  -   Sampling frequency in Hz.
double getFrequency(float data[], int size, int sampRate)
{
    return float(sampRate)/peakDistance(data, size);
}

// Returns average distance between peaks of signal, in number of samples.
// Data      -   Waveform data
// Size      -   Number of samples in data
float peakDistance(float data[], int size)
{
    int searching = 0;
    std::vector<int> indices;
    int index = 0;
    int lastZeroCross = -INT_MAX;
    float max = -INT_MAX;
    // Set a cap to potential tau-values, assuming we are concerned
    // with frequencies >16Hz (C0)
    int maxTau = (size-1 < 750) ? size - 1 : 750;
    for(int i = 1; i < maxTau; i++)
    {
        if(!searching)
        {
            if(data[i] > 0 && data[i-1] <= 0)
            {
                searching = 1;
                lastZeroCross = i;
            }
        }
        if(searching)
        {
            if(data[i] < 0)
            {
                searching = 0;
                lastZeroCross = i;
                max = -INT_MAX;
                if(index != 0)
                {
                    indices.push_back(index);
                }
                index = 0;
            }
            else
            {
                // Slight parabolic interpolation to find true peak
                if(data[i-1] + data[i] + data[i+1] > max)
                {
                    max = data[i-1] + data[i] + data[i+1];
                    index = i;
                }
            }
        }
    }

    // Filters false positive peaks
    double totMax = -INT_MAX;
    for(int j = 0; j < indices.size(); j++)
    {
        if(data[indices.at(j)] > totMax)
        {
            totMax = data[indices.at(j)];
        }
    }
    double threshold = totMax * kFactor;
    for(long k = indices.size() - 1; k >= 0; k--)
    {
        if(data[indices.at(k)] < threshold)
        {
            indices.erase(indices.begin() + k);
        }
    }
    if(indices.size() > 0)
    {
        return indices.front();
    }
    else
    {
        return -1;
    }
}


// Returns the normalized square distance function parameter.
// Time     -   Start time on data
// Tau      -   Delay parameter
// Window   -   Sampling window
// Data     -   Waveform data
double getNSDF(int time, int tau, int window, float data[])
{
    double Mt = getMt(time, tau, window, data);
    double acf = getAcf(time, tau, window, data);
    double nsdf = 1 -  (Mt - 2*acf) / Mt;
    return nsdf;
}

// Returns the type 2 autocorrelation factor.
// Time     -   Start time on data
// Tau      -   Delay parameter
// Window   -   Sampling window
// Data     -   Waveform data
double getAcf(int time, int tau, int window, float data[])
{
    double acf = 0;
    for(int i = time; i < time + window - tau; i++)
    {
        double xj = data[i];
        double xjt = data[i+tau];
        acf += xj * xjt;
    }
    return acf;
}

// Returns square difference term
// Time     -   Start time on data
// Tau      -   Delay parameter
// Window   -   Sampling window
// Data     -   Waveform data
double getMt(int time, int tau, int window, float data[])
{
    double mt = 0;
    for(int i = time; i < time + window - tau; i++)
    {
        double xj = data[i];
        double xjt = data[i+tau];
        mt += pow(xj,2) + pow(xjt,2);
    }
    return mt;
}


