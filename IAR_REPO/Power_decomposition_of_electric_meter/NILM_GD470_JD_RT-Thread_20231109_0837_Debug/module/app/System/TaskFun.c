//------------------------------------------------------------------------------------------
//	名    称： main.c
//	功    能:  系统入口函数
//	作    者:  wangjunsheng
//	创建时间:  2020.10.28
//	更新时间:
//	备    注:
//	修改记录:
//------------------------------------------------------------------------------------------
#include "AllHead.h"

#include <rtthread.h>

//定时器类
void rt_timer_init(rt_timer_t timer,const char* name,void(*timeout)(void *parameter),void * parameter,rt_tick_t time,rt_uint8_t flag);
rt_err_t rt_timer_delete(rt_timer_t timer);

//队列类
rt_err_t rt_mq_init (rt_mq_t mq, const char *name, void *msgpool, rt_size_t msg_size, rt_size_t pool_size, rt_uint8_t flag);
rt_err_t rt_mq_delete (rt_mq_t mq);

//初始化互斥量
rt_err_t rt_mutex_init (rt_mutex_t mutex, const char *name, rt_uint8_t flag);
rt_err_t rt_mutex_delete (rt_mutex_t mutex);

//信号量
rt_err_t rt_sem_init (rt_sem_t sem, const char *name, rt_uint32_t value, rt_uint8_t flag);
rt_sem_t rt_sem_create (const char *name, rt_uint32_t value, rt_uint8_t flag);
rt_err_t rt_sem_delete (rt_sem_t sem);
rt_err_t rt_sem_take (rt_sem_t sem, rt_int32_t time);
rt_err_t rt_sem_release (rt_sem_t sem);



///////////////////////////////////////////////////////////////
//	函 数 名 : xTaskCreate
//	函数功能 : 创建一个任务
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : int
//	参数说明 : void (*pvTaskCode
///////////////////////////////////////////////////////////////
int xTaskCreate(void (*pvTaskCode)(void *parameter), 
                const char * const pcName,
                int usStackDepth,
                void *pvParameters,
                int uxPriority,
                 int *pxCreatedTask
                )
{
    rt_thread_t tid;
    
    tid = rt_thread_create(pcName,	//线程名
                          pvTaskCode, pvParameters,	//入口函数及参数（一般为RT_NULL）
                          usStackDepth,	//线程栈大小
                          uxPriority, 10);	//优先级  时间片
    
    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
    }
    
    *pxCreatedTask = (int)tid;
    
    return 1;
}


///////////////////////////////////////////////////////////////
//	函 数 名 : xTimerCreate
//	函数功能 : 创建一个定时器
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : void*
//	参数说明 :  const char * const pcTimerName,
//				                           const int xTimerPeriod,
//				                           const int uxAutoReload,
//				                           void * const pvTimerID,
//				                           void(*timeout
///////////////////////////////////////////////////////////////
void* xTimerCreate( const char * const pcTimerName,
                         const int xTimerPeriod,
                         const int uxAutoReload,
                         void * const pvTimerID,
                         void(*timeout)(void *parameter) )
{
       
      return rt_timer_create(pcTimerName, timeout , pvTimerID, xTimerPeriod, RT_TIMER_FLAG_PERIODIC);  //周期性定时器
                
}



///////////////////////////////////////////////////////////////
//	函 数 名 : xTimerStart
//	函数功能 : 启动一个定时器
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : int
//	参数说明 :  void* xTimer,
//				  int xBlockTime 
///////////////////////////////////////////////////////////////
int xTimerStart( void* xTimer,  int xBlockTime )
{
    
      return rt_timer_start(xTimer);

}


///////////////////////////////////////////////////////////////
//	函 数 名 : xTimerStop
//	函数功能 : 停止一个定时器
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : int
//	参数说明 :  void* xTimer,
//				 int xBlockTime 
///////////////////////////////////////////////////////////////
int xTimerStop( void* xTimer, int xBlockTime )
{
    
    return rt_timer_stop(xTimer);	

}

int QueueLen = 0;

///////////////////////////////////////////////////////////////
//	函 数 名 : xQueueCreate
//	函数功能 : 创建一个消息队列
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : void*
//	参数说明 :  int uxQueueLength,
//				 int uxItemSize 
///////////////////////////////////////////////////////////////
void* xQueueCreate( int uxQueueLength, int uxItemSize )
{
  
      QueueLen = uxQueueLength;
      return rt_mq_create("FH_Queue", uxQueueLength, uxItemSize, RT_IPC_FLAG_FIFO);

}



///////////////////////////////////////////////////////////////
//	函 数 名 : xQueueReceive
//	函数功能 : 接收一个消息
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : int
//	参数说明 : void* xQueue,
//				 void *pvBuffer,
//				int xTicksToWait 
///////////////////////////////////////////////////////////////
int xQueueReceive(void* xQueue, void *pvBuffer,int xTicksToWait )
{
      
      return rt_mq_recv (xQueue, pvBuffer, QueueLen, xTicksToWait);
}


///////////////////////////////////////////////////////////////
//	函 数 名 : xQueueSend
//	函数功能 : 发送一个消息
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : int
//	参数说明 : void* xQueue,
//				void *pvBuffer,
//				 int xTicksToWait
///////////////////////////////////////////////////////////////
int xQueueSend(void* xQueue,void *pvBuffer, int xTicksToWait)
{
  
    return rt_mq_send(xQueue, pvBuffer, QueueLen);

}


void* xSemaphoreCreateBinary( void )
{
}


///////////////////////////////////////////////////////////////
//	函 数 名 : xSemaphoreCreateMutex
//	函数功能 : 创建一个互斥信号量
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : void*
//	参数说明 :  void 
///////////////////////////////////////////////////////////////
void* xSemaphoreCreateMutex( void )
{
    return rt_mutex_create ("FH_PV",  RT_IPC_FLAG_FIFO);
 
}

///////////////////////////////////////////////////////////////
//	函 数 名 : xSemaphoreTake
//	函数功能 : 获取一个信号量
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : BOOL
//	参数说明 :  void* xSemaphore,
//				 int xTicksToWait 
///////////////////////////////////////////////////////////////
BOOL xSemaphoreTake( void* xSemaphore, int xTicksToWait )
{
     
      return rt_mutex_take (xSemaphore, xTicksToWait);
 
}



///////////////////////////////////////////////////////////////
//	函 数 名 : xSemaphoreGive
//	函数功能 : 释放一个信号量
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : BOOL
//	参数说明 :  void* xSemaphore 
///////////////////////////////////////////////////////////////
BOOL xSemaphoreGive( void* xSemaphore )
{
    return rt_mutex_release (xSemaphore);
}


///////////////////////////////////////////////////////////////
//	函 数 名 : delay_ms
//	函数功能 : 延时毫秒
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : BOOL
//	参数说明 :  void* xSemaphore 
///////////////////////////////////////////////////////////////
void delay_ms(uint32_t ms)
{
    rt_thread_mdelay(ms);
}
  
///////////////////////////////////////////////////////////////
//	函 数 名 : delay_us
//	函数功能 : 
//	处理过程 : 延时微妙
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : BOOL
//	参数说明 :  uint32_t us 
///////////////////////////////////////////////////////////////

void delay_us(uint32_t us)
{
      api_Delayus(us);
}


///////////////////////////////////////////////////////////////
//	函 数 名 : vCreatTasktoCalc
//	函数功能 : 调用算法库，本函数是一个例程
//	处理过程 : 
//	备    注 : 
//	作    者 : Administrator
//	时    间 : 2023年4月20日
//	返 回 值 : void
//	参数说明 : void
///////////////////////////////////////////////////////////////
void CreateNULMTask(void)
{
	//创建一个队列
	vCreateQueues();
	//创建算法处理的任务
	vCreatTasktoCalc();
  
}