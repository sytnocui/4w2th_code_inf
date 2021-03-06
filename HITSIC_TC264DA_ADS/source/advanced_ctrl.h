/*
 * advanced.hpp
 *
 *  Created on: 2021年3月3日
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
    cross,//十字
    roundabout,//环岛
    ramp,//坡道
    branch,//三叉
    zebra,//斑马线
    unknown,
    stop,
    garage,//车库
}CAR_STATE;

extern int car_state;
extern int car_state_pre;
extern int car_direction;

extern uint32 servo_garage_left;
extern uint32 servo_garage_right;

void startline_ctrl(void);//斑马线延时停车
void State_Update(void);//状态机更新
void Ctrl_Update();//选择目标速度
void my_start(void);//发车函数
void Garage_Enter(void);
void Garage_Quit(void);
void OutTract_Protect(void);

/*HISTORY相关*/
boolean History_check(int type);
void History_Update(int type);

extern int history_done[];
extern int history_todo[];


#endif /* SOURCE_ADVANCED_CTRL_H_ */
