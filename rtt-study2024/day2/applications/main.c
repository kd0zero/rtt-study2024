#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */


#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)
rt_thread_t my_thread_1 = NULL; //线程1句柄
struct rt_thread  my_thread_2;  //线程2句柄
char thread2_stack[1024];       //线程2栈空间



void my_hoook_1(struct rt_thread *from, struct rt_thread *to)
{
    rt_kprintf("from:%s -- > to:%s \n",from->parent.name,to->parent.name);
}

static void thread_1_entry(void *parameter)//线程1入口函数
{
    char conunt = 'b'-2;
    while(1)
    {
        

             rt_pin_write(GPIO_LED_R, PIN_HIGH);
             rt_thread_mdelay(500);
             rt_pin_write(GPIO_LED_R, PIN_LOW);
             rt_thread_mdelay(500);
        

    }
}

static void thread_2_entry(void *parameter)//线程2入口函数
{
   char conunt = 'a'-2;
    while(1)
    {
        
        if(conunt <='z')
        {
            conunt+=1;
            rt_kprintf("thread2:%c\n",conunt);
            rt_thread_mdelay(1000);
           
        }


       
    }
}           
int main(void)
{
    rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    my_thread_1 = rt_thread_create("thread1",thread_1_entry,NULL,512,22,20);//动态创建线程1
    rt_thread_init(&my_thread_2,"thread2",thread_2_entry,NULL,thread2_stack,sizeof(thread2_stack),22,20);//静态创建线程2\

    rt_scheduler_sethook(my_hoook_1);
    if(my_thread_1 != NULL) //判断是否创建成功
        rt_thread_startup(my_thread_1);
    else
        rt_kprintf("thread1_err\n");

    rt_thread_startup(&my_thread_2);    //此处注意，
   
    return 0;
}