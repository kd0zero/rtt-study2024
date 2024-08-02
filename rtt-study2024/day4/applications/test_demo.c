//自定义驱动
//并完成注册
#include <rtdevice.h>
#include <rtdbg.h>
rt_err_t demo_init(rt_device_t dev)
{
    rt_kprintf("demo_init...\n");
    return 0;
}
rt_err_t demo_open(rt_device_t dev)
{
    rt_kprintf("demo_open...\n");
    return 0;
}
rt_err_t demo_close(rt_device_t dev)
{
    rt_kprintf("demo_close...\n");
    return 0;
}

int rt_demo_init(void)
{
    rt_device_t test_dev;
    test_dev = rt_device_create(RT_Device_Class_Char,32);
    if(!test_dev)
    {
        LOG_E("failed");
        return -RT_ERROR;
    }
    test_dev->init = demo_init;
    test_dev->open = demo_open;
    test_dev->close = demo_close;

    rt_device_register(test_dev,"test",RT_DEVICE_FLAG_RDWR);
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_demo_init);
MSH_CMD_EXPORT(rt_demo_init, rt_demo_init);