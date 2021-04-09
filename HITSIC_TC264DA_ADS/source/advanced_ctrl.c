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
int in_circle_flag = 0;
float servo_angle_circle= 0;

float servo_garage_left= SERVO_MID ;
float servo_garage_right= SERVO_MID ;

int history_done[car_stop+1] = {0};

void my_start(void)
{
    SmartCar_OLED_Printf6x8(0, 2,"ready");
    Delay_ms(STM0,500);
    car_time = 0;//ͣ����ʱ
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
//        circle_out_ctrl();
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
        if(imu_angle_z >=300 ||imu_angle_z <=-300)
        {
            beep_close();
            in_circle_flag = 0;
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
    else if(img_real_trident == img_state)
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

//��ʱ    ����history���£���֤Ҫ���жϲ��ظ�
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
    in_circle_flag = 1;
}

void circle_out_ctrl(void)
{
    beep_open();
}

/*--------------------------history��غ���-------------------------------*/
void History_Clean(void)
{
    for(uint8 i=0;i<=car_garage;i++)
    {
        history_done[i]=0;
    }
}

//ʮ��sb��if elseд����Ŀǰ����
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



