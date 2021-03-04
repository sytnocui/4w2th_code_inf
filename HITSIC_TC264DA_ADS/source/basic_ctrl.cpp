/*该文件包括基础控制，即电机，舵机，陀螺仪，编码器，pid计算等相关函数*/

#include <basic_ctrl.hpp>

/*电机PID参数*/
float motor_kp = 0.0;    //速度环p
float motor_ki = 0.0;    //速度环i
float motor_kd = 0.0;    //速度环d
float motor_err = 0;
float motor_err_pre = 0;
float motor_output = 0;

/*期望速度*/
float speed_dream = 0;
float speed_dream_str = 0;
float speed_dream_turn = 0;

/*车轮速度*/
float speed_actual = 0;

/*编码器返回值*/
int enco_get = 0;
float enco_distance = 0;

/*舵机PID参数*/
float servo_kp = 0.0;      //位置环p
float servo_ki = 0.0;      //位置环i
float servo_kd = 0.0;      //位置环d
float servo_err = 0;
float servo_err_pre = 0;
float servo_pid_output = 0;
float servo_output = SERVO_MID;


int stop_line = 100;

/*内容：其他参数*/
int img_var=0;
int startline_time =0;

void var_init(void)
{
    servo_kp = 0.25;
    servo_kd = 0.5;

    motor_kp = 20;
    motor_ki = 10;

    speed_dream = 1;
    speed_dream_str = 1;
    speed_dream_turn = 1;

    servo_garage_left = 6.6;
    servo_garage_right = 8.5;

    car_state = stop;

    imu_gyro_drift = 0.618;
}

void ctrl_init(void)
{
    pitMgr_t::insert(5U, 0U, motor_ctrl, pitMgr_t::enable);
    pitMgr_t::insert(20U, 1U, servo_ctrl, pitMgr_t::enable);
    pitMgr_t::insert(1000U, 7U, startline_ctrl, pitMgr_t::enable); //斑马线
    //    pitMgr_t::insert(20U, 3U, ADC_Upgrade, pitMgr_t::enable);

    /*初始化赋值*/
    var_init();
}

//TODO:float-->uint32,换通道
void motor_ctrl(void)
{
    Encoder_Update();//更新编码器返回值，计算实际速度
    motor_pid_calculate();//更新电机速度

    /*输出状态判断：是否启动 与 正反转*/
    if(motor_output >= 0)//后轮正转
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,motor_output);
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,0);
    }
    else if(motor_output < 0)//后轮反转
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,0);
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,(-motor_output));
    }
}

//TODO:探讨能不能从定时器移到回调函数里
//TODO:float-->uint32,换通道
void servo_ctrl(void)
{
    /*特殊状态，不需要计算，赋值之后直接跳出*/
    if(stop == car_state)//停车
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,(uint32)SERVO_MID);
        return;
    }
    if(garage == car_state)//出入库
    {
        if(car_direction)
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,(uint32)servo_garage_left);
        }
        else
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,(uint32)servo_garage_right);
        }
        return;
    }

    /*正式计算*/
    servo_pid_calculate();
    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT0_P02_0_OUT,(uint32)servo_output);
}

void servo_pid_calculate(void)
{
    img_var = mid_line[prospect];//前瞻处中线位置
    servo_err = (float)(img_var-94)/10;
    servo_pid_output = servo_kp * servo_err + servo_kd * (servo_err - servo_err_pre);
    servo_output = SERVO_MID + servo_pid_output;
    servo_err_pre = servo_err;

    /*限幅*/
    if(servo_output > SERVO_RIGHT)
        servo_output = SERVO_RIGHT;
    else if(servo_output < SERVO_LEFT)
        servo_output = SERVO_LEFT;
}

void motor_pid_calculate(void)
{
    motor_err_pre = motor_err;
    motor_err = speed_dream - speed_actual;      //计算速度偏差
    motor_output += motor_ki*motor_err + motor_kp* (motor_err-motor_err_pre);    //计算输出

    /*电机限幅*/
    if(motor_output >= 99)
        motor_output = 99;
    else if(motor_output <= -99)
        motor_output = -99;
}

void Encoder_Update(void)
{
    enco_get = -SmartCar_Encoder_Get(GPT12_T2);
    enco_distance += enco_get * 0.000167;       //*0.6 / 3592.4
    speed_actual = enco_get * 0.0334;   //*0.6 / 3592.4 / 0.005   *0.6 / 3592.4为编码器与距离的比例，/0.005为dt，
    SmartCar_Encoder_Clear(GPT12_T2);
    //TODO:
}

void Encoder_Distance_Clear(void)
{
    enco_distance = 0;
}

