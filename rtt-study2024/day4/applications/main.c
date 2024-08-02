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
#include <rtdbg.h>

#define UART2
#define UART2_TX    GET_PIN(A, 2)
#define UART2_RX    GET_PIN(A, 3)

struct rt_semaphore sem;
rt_thread_t th1;
rt_device_t u2_dev;
rt_device_t dev_demo;

//定义结构体(默认值)
struct serial_configure u2_config = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_callback(rt_device_t dev , rt_size_t size)
{
    rt_sem_release(&sem);
    return RT_EOK;
}
void u2_entry(void *p)
{
    char buffer;
    while (1)
    {
        /* code */

        while (rt_device_read(u2_dev,0,&buffer,1) != 1)
        {
            rt_sem_take(&sem,RT_WAITING_FOREVER);
        }
        rt_kprintf("%d",buffer);
    }
    
}
int main(void)
{
    rt_err_t ret = 0;
    u2_dev = rt_device_find("uart2");
    dev_demo = rt_device_find("test");
    if(u2_dev == RT_NULL)
    {
        LOG_E("cant find!\n");
        return -ENOMEM;//返回无效码
    }
    ret = rt_device_open(u2_dev,RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    if (ret<0)
    {
        LOG_E("cant open!\n");
        return ret;
    }
    if (dev_demo == RT_NULL)
    {
        LOG_E("device failed");
        return RT_EINVAL;
    }
    rt_device_init(dev_demo);
    rt_device_open(dev_demo,RT_DEVICE_OFLAG_RDWR);
    rt_device_close(dev_demo);
    
    rt_device_control(u2_dev,RT_DEVICE_CTRL_CONFIG,(void *)&u2_config);

    //设置接收回调函数
    rt_device_set_rx_indicate(u2_dev,rx_callback);

    /************设置信号量和线程：***********/
    rt_sem_init(&sem,"sem",0,RT_IPC_FLAG_FIFO);
    th1 = rt_thread_create("th1",u2_entry,RT_NULL,1024,10,5);
    rt_thread_startup(th1);

    rt_device_write(u2_dev , 0 , "hello init ...\n",rt_strlen("hello init ...\n"));

    return RT_EOK;
}