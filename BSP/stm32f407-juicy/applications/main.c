/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "car.h"


/* defined the LED0 pin: PA6 */
#define LED0_PIN    GET_PIN(A, 6)

#define PS2_CS_PIN  GET_PIN(A, 4)
#define PS2_CLK_PIN  GET_PIN(A, 5)
#define PS2_DO_PIN  GET_PIN(A, 11)
#define PS2_DI_PIN  GET_PIN(C, 8)

#define PWM_DEV_NAME        "pwm4"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */


uint8_t Comd[2]={0x01,0x42};


struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */


static void pwm_led_sample_thread_entry(void *paramter)
{
    rt_uint32_t period, pulse, dir;

    period = 500000;    /* 周期为0.5ms，单位为纳秒ns */
    dir = 1;            /* PWM脉冲宽度值的增减方向 */
    pulse = 0;          /* PWM脉冲宽度值，单位为纳秒ns */

    /* 查找设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        while(1);
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* 使能设备 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

    while (1)
    {
        rt_thread_mdelay(50);

        if (dir)
        {
            pulse += 5000;      /* 从0值开始每次增加5000ns */
        }
        else
        {
            pulse -= 5000;      /* 从最大值开始每次减少5000ns */
        }
        if (pulse >= period)
        {
            dir = 0;
        }
        if (0 == pulse)
        {
            dir = 1;
        }

        /* 设置PWM周期和脉冲宽度 */
        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    }
}

static void spi_ps2_sample(void *param)
{
	int i = 0;
	i = ps2_read_light();
		 rt_kprintf("light = %d\n", i);
	
	
}

MSH_CMD_EXPORT(spi_ps2_sample, spi ps2 sample);










#define PWM_LED_THREAD_STACK_SIZE     1024
#define PWM_LED_THREAD_PRIOR          25
#define PWM_LED_THREAD_TIMESLICE      5

static rt_thread_t tid1 = RT_NULL;

int main(void)
{
   int count = 1;

    
    car_sample();
    ps2_init(PS2_CS_PIN, PS2_CLK_PIN, PS2_DO_PIN, PS2_DI_PIN);

	
    tid1 = rt_thread_create("pwm_led_test", pwm_led_sample_thread_entry, RT_NULL,
        PWM_LED_THREAD_STACK_SIZE, PWM_LED_THREAD_PRIOR, PWM_LED_THREAD_TIMESLICE);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);


    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED0_PIN, PIN_HIGH);
    
    while (count++)
    {
      //  rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
       // rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }


    return RT_EOK;
}
