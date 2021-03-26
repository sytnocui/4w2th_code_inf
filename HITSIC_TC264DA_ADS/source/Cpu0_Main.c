#include "Ifx_Types.h"
#include "IfxCpu.h"
#include <ifxCpu_Irq.h>
#include "IfxScuWdt.h"

#include "SmartCar_Uart.h"
#include "SmartCar_Upload.h"
#include "SmartCar_Oled.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_MPU.h"
#include "SmartCar_MT9V034.h"
#include "SmartCar_Systick.h"
#include "SmartCar_GPIO.h"
#include "SmartCar_PIT.h"
#include "SmartCar_ADC.h"
#include "common.h"

#include "menu.h"

#pragma section all "cpu0_dsram"
//IfxCpu_syncEvent g_cpuSyncEvent;

void wifi_upload(void);
void img_upload(void);
void key_temp(void);
void callback_temp(void);
void oled_show_img(void);
void beep_open(void);
void beep_close(void);

int core0_main(void)
{
    IfxCpu_disableInterrupts();
    
    get_clk();
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /*初始化外设*/
    //OLED初始化
    SmartCar_Oled_Init();

    //总钻风初始化
    SmartCar_OLED_Fill(0);
    SmartCar_OLED_Printf6x8(0, 0,"do");
    SmartCar_MT9V034_Init();
    SmartCar_OLED_Printf6x8(0, 1,"over");
    //mpu初始化
//    SmartCar_MPU_Set_DefaultConfig(this_mpu);
//    SmartCar_OLED_Printf6x8(0, 2,"over");
//    SmartCar_MPU_Init2(this_mpu);
//    SmartCar_OLED_Printf6x8(0, 3,"over");
//    SmartCar_GyroOffset(this_mpu);

    //编码器初始化
//    SmartCar_Encoder_Init(GPT12_T5 , IfxGpt120_T5INB_P10_3_IN , IfxGpt120_T5EUDB_P10_1_IN);

    /*初始化单片机功能*/
    //PWM初始化
    SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,50,SERVO_MID);
    SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_6_TOUT6_P02_6_OUT,200,0);
    SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_7_TOUT7_P02_7_OUT,200,0);
    //串口初始化
//    SmartCar_Uart_Init(IfxAsclin0_TX_P14_0_OUT,IfxAsclin0_RXA_P14_1_IN,1152000,0);
    //外部中断初始化
//    Eru_Init(CH0_P15_4,RISING);
//    Eru_Init(CH4_P15_5,FALLING);
    //GPIO初始化
    GPIO_Init(P20,9,PUSHPULL,1);
    GPIO_Init(P20,8,PUSHPULL,1);
    GPIO_Init(P21,4,PUSHPULL,1);
    GPIO_Init(P21,5,PUSHPULL,1);

    GPIO_Init(P33,10,PUSHPULL,0);//蜂鸣器

    GPIO_Init(P22,0,PULLUP,0);
    GPIO_Init(P22,1,PULLUP,0);
    GPIO_Init(P22,2,PULLUP,0);
    GPIO_Init(P22,3,PULLUP,0);
//    GPIO_Init(P33,12,PULLUP,0);
//    GPIO_Init(P33,13,PULLUP,0);
    //定时中断初始化
    Pit_Init_ms(CCU6_0,PIT_CH0,5);
    Pit_Init_ms(CCU6_0,PIT_CH1,20);
//    Pit_Init_ms(CCU6_1,PIT_CH0,10);
    Pit_Init_ms(CCU6_1,PIT_CH1,3000);

    //ADC初始化
//    ADC_Init(ADC_1,ADC1_CH4_A20);
//    ADC_Init(ADC_1,ADC1_CH5_A21);

    IfxCpu_enableInterrupts();

    //代码初始化
    ctrl_init();

    while(TRUE)
    {
        SmartCar_OLED_Printf6x8(100,1,"%.0f",servo_output);

        //摄像头回调
        callback_temp();//阻塞
        //按键检测
        key_temp();
        //oled显示图像
        oled_show_img();
        //wifi传数据
//        wifi_upload();
        //传图
//        img_upload();
    }
}

void callback_temp(void)
{
    while(!mt9v034_finish_flag);
    mt9v034_finish_flag = 0;

    if(stop == car_state)
    {
        speed_dream = 0;
        return;
    }

    image_main();
    State_Update();
    Ctrl_Update();
}

void key_temp(void)
{
    if(!GPIO_Read(P22,0)
    ||!GPIO_Read(P22,1)
    ||!GPIO_Read(P22,2)
    ||!GPIO_Read(P22,3))
    {
        while(1)
        {
            if(!GPIO_Read(P22,0))
            {
                if(stop == car_state)
                {
                    my_start();
                }
                else
                {
                    my_stop();
                }
            }
            else if(!GPIO_Read(P22,1))
            {
                GPIO_Toggle(P21,5);
                Delay_ms(STM0,300);
            }
            else if(!GPIO_Read(P22,2))
            {
                threshold++;
            }
            else if(!GPIO_Read(P22,3))
            {
                threshold--;
            }
            else
            {
                return;
            }
            Delay_ms(STM0,20);
        }

    }
}

void oled_show_img(void)
{
    if(!GPIO_Read(P21,5))
    {
        SmartCar_Show_IMG((uint8*)mt9v034_image,120,188);
    }
}

void wifi_upload(void)
{
    //todo:const能用吗
    const float wifi_data[16] ={
        (float)enco_get,
        (float)speed_dream,
        (float)speed_actual,
        (float)motor_output,
        (float)car_state,
        (float)car_state_pre};
    SmartCar_VarUpload(wifi_data, 16);
}

void img_upload(void)
{
    SmartCar_ImgUpload((uint8*)mt9v034_image,120,188);
}


//PIT中断函数  示例
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)//电机控制
{
    enableInterrupts();//开启中断嵌套
    PIT_CLEAR_FLAG(CCU6_0, PIT_CH0);
    motor_ctrl();

}


IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)//舵机控制
{
    enableInterrupts();//开启中断嵌套
    PIT_CLEAR_FLAG(CCU6_0, PIT_CH1);
    servo_ctrl();

}

IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    enableInterrupts();//开启中断嵌套
    PIT_CLEAR_FLAG(CCU6_1, PIT_CH0);
    imu_update();

}

IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)//目前没用
{
    enableInterrupts();//开启中断嵌套
    PIT_CLEAR_FLAG(CCU6_1, PIT_CH1);
    GPIO_Toggle(P21,4);

}

void beep_open(void)
{
    GPIO_Set(P33,10,1);
}
void beep_close(void)
{
    GPIO_Set(P33,10,0);
}


#pragma section all restore
