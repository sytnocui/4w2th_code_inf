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
    car_straight,
    car_rightTurn,
    car_leftTurn,
    car_cross,//ʮ��
    car_circle_in,//������
    car_circle_out,
    car_ramp,//�µ�
    car_branch,//����
    car_zebra,//������
    car_unknown,
    car_stop,
    car_garage,//����
}CAR_STATE;

extern int car_state;
extern int car_state_pre;
extern int car_direction;

extern float servo_angle_circle;

extern uint32 servo_garage_left;
extern uint32 servo_garage_right;

//ʱ�����
extern int car_time;
extern int circle_time;

void startline_ctrl(void);//��������ʱͣ��
void State_Update(void);//״̬������
void Ctrl_Update(void);//ѡ��Ŀ���ٶ�
void my_start(void);//��������
void my_stop(void);//ͣ������
void Garage_Enter(void);
void Garage_Quit(void);
void OutTract_Protect(void);

/*HISTORY���*/
boolean History_check(int type);
void History_Update(int type);

extern int history_done[];
extern int history_todo[];


#endif /* SOURCE_ADVANCED_CTRL_H_ */
