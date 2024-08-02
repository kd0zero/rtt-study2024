/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

#define PIN_KEY0      GET_PIN(C, 0)     // PC0:  KEY0         --> KEY

#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2= RT_NULL;
static rt_sem_t my_sem_1 = RT_NULL;

static void key_entry(void *parameter);
static void led_entry(void *parameter);
int main(void)
{
    rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_KEY0, PIN_MODE_INPUT_PULLUP);

    my_sem_1 = rt_sem_create("my_sem_1", 0, RT_IPC_FLAG_PRIO);
    if (my_sem_1 == RT_NULL)
    {
        rt_kprintf("my_sem_1 is not create.\n");
        return -1;
    }
    else
    {
        rt_kprintf("my_sem_1 is not create.\n");
    }

    tid1 = rt_thread_create("key_thread",
                            key_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid2 = rt_thread_create("led_thread",
                            led_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}

static void key_entry(void *parameter)
{
    rt_uint32_t count = 0;
    rt_uint32_t last_count =0;
    while (1)
    {
   
        
        if ((rt_pin_read(PIN_KEY0) == PIN_LOW))
        {
            
            count = rt_tick_get();
            rt_thread_mdelay(100); 
             last_count =    count;
            while (rt_pin_read(PIN_KEY0) == PIN_LOW)
            {}    //按键松开后运行后面代码 
            count = rt_tick_get();
            if(count -last_count>3000)
            {

            }
            else 
            {
                rt_kprintf("mode_1\r\n");
                rt_sem_release(my_sem_1);
            }
            
            
        }

    }
}

static void led_entry(void *parameter)
{
    rt_uint32_t result = 0;
    while (1)
    {
        result = rt_sem_take(my_sem_1, RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            rt_kprintf("LED ON\r\n");
            rt_pin_write(GPIO_LED_R, PIN_LOW); 
            rt_thread_mdelay(1000);
            rt_kprintf("LED OFF\r\n");
            rt_pin_write(GPIO_LED_R, PIN_HIGH);             
        }

    }
}