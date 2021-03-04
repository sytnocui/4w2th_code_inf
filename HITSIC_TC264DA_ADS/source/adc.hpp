/*
 * adc.hpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#ifndef SOURCE_ADC_HPP_
#define SOURCE_ADC_HPP_

#include "everything.hpp"

#define SampleTimes 25
#define ChannelTimes 8

extern float AD[ChannelTimes];
extern float em_error;
void LV_Sample();                             // ad�ɼ�����

void LV_Get_Val();//Լ0.3mS                  //�Բɼ���ֵ�˲�
void ADC_Upgrade(void);
void swap();


#endif /* SOURCE_ADC_HPP_ */
