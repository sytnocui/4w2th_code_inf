/*
 * advanced.hpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#ifndef SOURCE_ADVANCED_CTRL_H_
#define SOURCE_ADVANCED_CTRL_H_

#include "common.h"
#include "basic_ctrl.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_Encoder.h"
#include "SmartCar_Systick.h"
#include "image.h"

typedef enum{
    straight,
    rightTurn,
    leftTurn,
    cross,//ʮ��
    roundabout,//����
    ramp,//�µ�
    branch,//����
    zebra,//������
    unknown,
    stop,
    garage,//����
}CAR_STATE;

extern int car_state;
extern int car_state_pre;
extern int car_direction;

extern uint32 servo_garage_left;
extern uint32 servo_garage_right;

void startline_ctrl(void);//��������ʱͣ��
void State_Update(void);//״̬������
void Ctrl_Update();//ѡ��Ŀ���ٶ�
void my_start(void);//��������
void Garage_Enter(void);
void Garage_Quit(void);
void OutTract_Protect(void);

/*HISTORY���*/
boolean History_check(int type);
void History_Update(int type);

extern int history_done[];
extern int history_todo[];


#endif /* SOURCE_ADVANCED_CTRL_H_ */
