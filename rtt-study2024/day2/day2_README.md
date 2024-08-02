#                                              夏令营day2_线程管理
## 线程的创建 typedef struct rt_thread *rt_thread_t;
##     1.线程的创建：动态创建和静态创建
###                动态创建:
                rt_thread_t rt_thread_create(                   //返回一个指向句柄的指针，所以在此之前需要创建一个指针
                            const char *name,                   //线程的名字，默认最大为8个字符
                             void (*entry)(void *parameter),    //入口函数指针，在线程进入运行状态后进入此函数
                             void       *parameter,             //入口函数参数指针
                             rt_uint32_t stack_size,            //栈空间
                             rt_uint8_t  priority,              //线程优先级
                             rt_uint32_t tick                   //节拍数
                             )
                 示例：rt_thread_t my_thread_1 = NULL; //创建以恶指向线程1句柄的指针

                        static void thread_1_entry(void *parameter)//线程1入口函数
                        {
                            int conunt = 0;
                            while(1)
                            {
                                
                                if(conunt <10)
                                {
                                    conunt++;
                                    rt_kprintf("thread1:%d\n",conunt);
                                    rt_thread_mdelay(1000);
                                    
                                }
                            }
                        }
                            rt_thread_startup(my_thread_1);//开启线程，进入就绪态
 #                   静态创建:
                    rt_err_t rt_thread_init(
                        struct rt_thread *thread,           //句柄指针
                        const char       *name,             //线程的名字，默认最大为8个字符
                        void (*entry)(void *parameter),     //入口函数指针，在线程进入运行状态后进入此函数
                        void             *parameter,        //入口函数参数指针
                        void             *stack_start,      //栈的起始地址
                        rt_uint32_t       stack_size,       //栈空间
                        rt_uint8_t        priority,         //线程优先级
                        rt_uint32_t       tick)             //节拍数

                    示例：struct rt_thread  my_thread_2;  //创建线程2句柄

                        static void thread_2_entry(void *parameter)//线程2入口函数
                        {
                            XXX
                        }

                        rt_thread_init                  //静态创建线程2
                        (&my_thread_2,                  //my_thread_2是句柄，因此需要取地址
                        "thread2",
                        thread_2_entry,
                        NULL,
                        thread2_stack,
                        sizeof(thread2_stack),
                        22,
                        20);

                        rt_thread_startup(&my_thread_2);    //my_thread_2是句柄，因此需要取地址

 #                注意点  ：
                          1.  typedef struct rt_thread *rt_thread_t;     
                                说明rt_thread_t 是句柄结构体 struct rt_thread 的指针在调用一些函数时注意参数的类型
                          2.动态线程是系统自动从动态内存堆上分配栈空间与线程句柄
                            静态线程是由用户分配栈空间与线程句柄

























