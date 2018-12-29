//
//  GetPitch.hpp
//  J-Tuner
//
//  Created by Joel Tecson on 2018-10-31.
//  Copyright © 2018 bignerdranch. All rights reserved.
//

#ifndef GetPitch_hpp
#define GetPitch_hpp
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
void asdf();
double getPitch(float data[], int size, int Hz);
double getFrequency(float data[], int size, int Hz);
float peakDistance(float data[], int size);
double getNSDF(int time, int tau, int window, float data[]);
double getAcf(int time, int tau, int window, float data[]);
double getMt(int time, int tau, int window, float data[]);
#ifdef __cplusplus
}
#endif

#endif /* GetPitch_hpp */
