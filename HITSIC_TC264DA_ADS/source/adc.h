/*
 * adc.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_ADC_H_
#define SOURCE_ADC_H_

#include "common.h"

#define SampleTimes 25
#define ChannelTimes 8

extern float AD[ChannelTimes];
extern float em_error;
void LV_Sample();                             // ad采集函数

void LV_Get_Val();//约0.3mS                  //对采集的值滤波
void ADC_Upgrade(void);
void swap();


#endif /* SOURCE_ADC_H_ */
