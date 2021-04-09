/*
 * advanced.cpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#include <advanced_ctrl.h>


int car_state = car_stop;
int car_state_pre = car_stop;
int car_direction = 0;
int car_branch_direction = 0;
int in_circle_flag = 0;
float servo_angle_circle= 0;

float servo_garage_left= SERVO_MID ;
float servo_garage_right= SERVO_MID ;

int history_done[car_stop+1] = {0};

void my_start(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"ready");
    Delay_ms(STM0,500);
    car_time = 0;//停车计时
    Garage_Quit();
}

void my_stop(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"stop ");
    Delay_ms(STM0,500);
    car_state = car_stop;
}

void Ctrl_Update()
{
    //注意if的顺序
    if(car_stop==car_state)
    {
        speed_dream = 0;
    }
    else if(car_garage == car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(car_circle_in == car_state)//环岛更新
    {
        if(circle_time >= 5)
        {
            circle_in_ctrl();
        }
    }
    else if(car_circle_out == car_state)
    {
//        circle_out_ctrl();
    }
    /*正式处理*/
    else if(car_rightTurn==car_state || car_leftTurn==car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(car_straight==car_state)
    {
        speed_dream = speed_dream_str;
    }
    else if(car_cross == car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(car_branch == car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(car_zebra == car_state)
    {
        car_branch_direction = 2;
        if(history_done[car_zebra]>=2)
        {
            Garage_Enter();
        }
    }
}

void State_Update(void)
{
    /*赛道保护*/
    OutTract_Protect();

    /*特殊状态直接跳出*/
    if(car_stop == car_state || car_garage == car_state)
    {
        //什么都不做
    }
    //赛道元素判断
    else if(car_circle_out ==car_state)//如果在出环岛，而且陀螺仪没计满，就直接跳出，计满了就换成直道
    {
        if(imu_angle_z >=300 ||imu_angle_z <=-300)
        {
            beep_close();
            in_circle_flag = 0;
            car_state = car_straight;
        }
    }
    else if(car_circle_in == car_state)//如果在进环岛，而且陀螺仪没有计满，就直接跳出，计满了就换成出环岛
    {
        servo_angle_circle = img_angle;
        if(imu_angle_z >=180 ||imu_angle_z <=-180)
        {
            car_state = car_circle_out;
        }
    }
    else if(img_enter_circle == img_state
            &&circle_time >=5)//进环岛判断
    {
        car_state = car_circle_in;
    }
    else if(img_near_cross == img_state
            ||img_far_cross == img_state
            ||img_mid_cross == img_state
            ||img_oblique_cross == img_state)//十字
    {
        car_state=car_cross;
    }
    else if(img_real_trident == img_state)
    {
        car_state=car_branch;
    }
    else if(img_zebra == img_state)//斑马线
    {
        car_state=car_zebra;
    }
    //一般赛道判断
    else
    {
        car_state=car_straight;
    }
    History_Update();
//    else if(k_mid > k_str && k_mid<2.5)
//    {
//        car_state=rightTurn;
//    }
//    else if(k_mid <-k_str && k_mid>-2.5)
//    {
//        car_state=leftTurn;
//    }
//    else if(k_mid >-k_str && k_mid < k_str)
//    {
//        car_state=straight;
//    }
}

//计时    用于history更新，保证要素判断不重复
int car_time =0;
int circle_time = 0;
int cross_time = 0;
int branch_time = 0;
int zebra_time = 0;
void time_ctrl(void)
{
    car_time++;
    circle_time++;
    cross_time++;
    branch_time++;
    zebra_time++;
}

void Garage_Enter(void)
{
    car_state = car_garage;
    imu_clear();
    speed_dream = speed_dream_turn;
    while(imu_angle_z < 90 && imu_angle_z>-90);
    car_state = car_stop;
}

void Garage_Quit(void)
{
    car_state = car_garage;
    imu_clear();
    speed_dream = speed_dream_turn;
    while(imu_angle_z < 80 && imu_angle_z>-80);
    car_state = car_straight;
}

void OutTract_Protect(void)//出赛道保护
{
//    if(mid_line[img_stop] == MISS)
//    {
//        car_state = stop;
//    }
}

void circle_in_ctrl(void)
{
    circle_time = 0;
    beep_open();
    imu_clear();
    in_circle_flag = 1;
}

void circle_out_ctrl(void)
{
    beep_open();
}

/*--------------------------history相关函数-------------------------------*/
void History_Clean(void)
{
    for(uint8 i=0;i<=car_garage;i++)
    {
        history_done[i]=0;
    }
}

//十分sb的if else写法，目前够用
void History_Update(void)
{
    if(car_state != car_state_pre)
    {
        if(car_cross == car_state)
        {
            if(cross_time >=1)
            {
                cross_time = 0;
                history_done[car_state]++;
                car_state_pre = car_state;
            }
        }
//        else if(circle_in == car_state)
//        {
//            if(circle_time >=1)
//            {
//                circle_time = 0;
//                history_done[car_state]++;
//                car_state_pre = car_state;
//            }
//        }
        else if(car_branch == car_state)
        {
            if(branch_time >=1)
            {
                branch_time = 0;
                history_done[car_state]++;
                car_state_pre = car_state;
            }
        }
        else if(car_zebra == car_state)
        {
            if(zebra_time >=1)
            {
                zebra_time = 0;
                history_done[car_state]++;
                car_state_pre = car_state;
            }
        }
        else
        {
            history_done[car_state]++;
            car_state_pre = car_state;
        }
    }
}



