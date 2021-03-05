/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

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

#pragma section all "cpu0_dsram"
//IfxCpu_syncEvent g_cpuSyncEvent;

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
    
    IfxCpu_enableInterrupts();

    /*初始化单片机功能*/
    //PWM初始化
    SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM2_7_TOUT64_P20_8_OUT,50,0);
    //串口初始化
    SmartCar_Uart_Init(IfxAsclin0_TX_P14_0_OUT,IfxAsclin0_RXA_P14_1_IN,921600,0);
    //外部中断初始化
    Eru_Init(CH0_P15_4,RISING);
    Eru_Init(CH4_P15_5,FALLING);
    //GPIO初始化
    GPIO_Init(P20,9,PUSHPULL,0);
    GPIO_Init(P20,8,PUSHPULL,1);//TODO:啥意思
    GPIO_Init(P21,4,PUSHPULL,0);
    GPIO_Init(P21,5,PUSHPULL,0);//注：按键和拨码无上下拉，不需要初始化
    //定时中断初始化
    Pit_Init_ms(CCU6_0,PIT_CH0,1000);
    Pit_Init_ms(CCU6_0,PIT_CH1,5000);
    Pit_Init_ms(CCU6_1,PIT_CH0,2000);
    Pit_Init_ms(CCU6_1,PIT_CH1,3000);//TODO:把控制环的都写在这吧
    //ADC初始化
    ADC_Init(ADC_1,ADC1_CH4_A20);
    ADC_Init(ADC_1,ADC1_CH5_A21);

    /*初始化外设*/
    //总钻风初始化
    SmartCar_MT9V034_Init();
    //OLED初始化
    SmartCar_Oled_Config_Init();
    SmartCar_Oled_Init();

    while(TRUE)
    {
        while(!mt9v034_finish_flag){};
        SmartCar_Show_IMG((uint8*)mt9v034_image,120,188);
        mt9v034_finish_flag = 0;


        //SmartCar_VarUpload(var,10);//todo:封装自己的wifi传图函数
    }
}

#pragma section all restore
