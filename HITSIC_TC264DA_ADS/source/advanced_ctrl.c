/*
 * advanced.cpp
 *
 *  Created on: 2021��3��3��
 *      Author: 10798
 */

#include <advanced_ctrl.h>


int car_state = car_stop;
int car_state_pre = car_stop;
int car_direction = 0;
int car_branch_direction = 0;

float servo_angle_circle= 0;

uint32 servo_garage_left= SERVO_MID ;
uint32 servo_garage_right= SERVO_MID ;

int history_done[car_stop+1] = {0};
int history_todo[car_stop+1] = {0};
int in_circle_flag = 0;
//ʱ�����
int car_time =0;
int circle_time = 0;

void my_start(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"ready");
    Delay_ms(STM0, 1000);
    startline_time = 0;//ͣ����ʱ
//    Garage_Quit();
    car_state = car_straight;
}

void my_stop(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"stop ");
    Delay_ms(STM0,1000);//����
//    Garage_Enter();
    car_state = car_stop;
}

void Ctrl_Update()
{
    //ע��if��˳��
    if(car_stop==car_state)
    {
        speed_dream = 0;
    }
    else if(car_garage == car_state)
    {
        speed_dream = speed_dream_turn;
    }
    else if(car_circle_in == car_state)//��������
    {
        if(circle_time >= 5)
        {
            circle_in_ctrl();
        }
    }
    else if(car_circle_out == car_state)
    {
        circle_out_ctrl();
    }
    /*��ʽ����*/
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
    else if(car_zebra == car_state)
    {
        Garage_Enter();
    }
}

void State_Update(void)
{
    /*��������*/
    OutTract_Protect();

    /*����״ֱ̬������*/
    if(car_stop == car_state || car_garage == car_state)
    {
        //ʲô������
    }
    //����Ԫ���ж�
    else if(car_circle_out ==car_state)//����ڳ�����������������û��������ֱ�������������˾ͻ���ֱ��
    {
        if(imu_angle_z >=70 ||imu_angle_z <=-70)
        {
            car_state = car_straight;
        }
    }
    else if(car_circle_in == car_state)//����ڽ�����������������û�м�������ֱ�������������˾ͻ��ɳ�����
    {
        servo_angle_circle = img_angle;
        if(imu_angle_z >=180 ||imu_angle_z <=-180)
        {
            car_state = car_circle_out;
        }
    }
    else if(img_enter_circle == img_state
            &&circle_time >=5)//�������ж�
    {
        car_state = car_circle_in;
    }
    else if(img_near_cross == img_state
            ||img_far_cross == img_state
            ||img_mid_cross == img_state
            ||img_oblique_cross == img_state)//ʮ��
    {
        car_state=car_cross;
    }
    else if(img_trident == img_state)
    {
        car_state=car_branch;
    }
    else if(img_zebra == img_state)//������
    {
        car_state=car_zebra;
    }
    //һ�������ж�
    else
    {
        car_state=car_straight;
    }
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

void time_ctrl(void)
{
    car_time++;
    circle_time++;
}

void Garage_Enter(void)
{
    car_state = car_garage;
//    beep_Open();
    imu_clear();
    speed_dream = speed_dream_turn;//����״̬��������while������������Ŀ���ٶ�
    while(imu_angle_z < 90 && imu_angle_z>-90);
//    beep_Close();
    car_state = car_stop;
}

void Garage_Quit(void)
{
    car_state = car_garage;
//    beep_open();
    imu_clear();
    speed_dream = speed_dream_turn;
    while(imu_angle_z < 80 && imu_angle_z>-80);
//    beep_close();
    car_state = car_straight;
}

void OutTract_Protect(void)//����������
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
}

void circle_out_ctrl(void)
{
    beep_open();
    imu_clear();
    while(imu_angle_z < 150 && imu_angle_z>-150);
    beep_close();
    car_state = car_straight;
}

/*--------------------------history��غ���-------------------------------*/
boolean History_Check(int type)
{
    if(history_done[type] < history_todo[type])
    {
        return TRUE;
    }
    else if(history_done[type] == history_todo[type])
    {
        return FALSE;
    }
    else
    {
        return FALSE;
        //��Ӵ�����
    }
}

void History_Update(int type)
{
    if(car_state != car_state_pre)
    {
        history_done[type]++;
        car_state_pre = car_state;
    }
}




