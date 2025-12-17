/*
 * @Author: sunzigang 1026733696@qq.com
 * @Date: 2023-11-29 14:38:36
 * @LastEditors: sunzigang 1026733696@qq.com
 * @LastEditTime: 2023-11-30 11:18:14
 * @FilePath: \JIG\Utilities\third_party\RingBuffer\ring_buffer_safe.c
 * @Description:  环形缓冲区 接口适配器， 可以push/pop各种类型的数据
 * 
 */


#include "board.h"

#include "ring_buffer_safe.h"

#define RB_Mutex_lock_Safe()  UBaseType_t uxSavedInterruptStatus ; \
    do { \
        if(rb_handle->xMutex == NULL) break; \
        if(__get_CONTROL() == 0) {uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();} \
        else{ taskENTER_CRITICAL(); }\
    }while (0);

#define RB_Mutex_unlock_Safe()  do { \
        if(rb_handle->xMutex == NULL) break; \
        if(__get_CONTROL() == 0) {  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus); }\
        else { taskEXIT_CRITICAL(); } \
    }while (0);

uint8_t RB_Safe_Init(ring_buffer *rb_handle, void *buffer_addr ,uint32_t buffer_size, uint32_t buffer_Type)
{
    rb_handle->xMutex = xSemaphoreCreateMutex();
    return RB_Init(rb_handle, buffer_addr, buffer_size, buffer_Type);
}

uint8_t RB_Safe_Delete(ring_buffer *rb_handle, uint32_t Length){
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Delete(rb_handle, Length);
    RB_Mutex_unlock_Safe();

    return ret ;  
}

uint8_t RB_Safe_Write_Byte(ring_buffer *rb_handle, void* data){    
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Write_Byte(rb_handle, data);
    RB_Mutex_unlock_Safe();
    return ret ;  
}

uint8_t RB_Safe_Write_String(ring_buffer *rb_handle, void *input_addr, uint32_t write_Length){    
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Write_String(rb_handle, input_addr, write_Length);
    RB_Mutex_unlock_Safe();
    return ret ;  
}

uint8_t RB_Safe_Read_Byte(ring_buffer *rb_handle, void *output_addr){    
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Read_Byte(rb_handle, output_addr);
    RB_Mutex_unlock_Safe();
    return ret ; 
}

uint8_t RB_Safe_Read_String(ring_buffer *rb_handle, void *output_addr, uint32_t read_Length){    
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Read_String(rb_handle, output_addr, read_Length);
    RB_Mutex_unlock_Safe();
    return ret ;  
}

uint8_t RB_Safe_Peek_Byte(ring_buffer *rb_handle, void *output_addr){
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Peek_Byte(rb_handle, output_addr);
    RB_Mutex_unlock_Safe();
    return ret ;  
}

uint8_t RB_Safe_Peek_String(ring_buffer *rb_handle, void *output_addr, uint32_t peek_Length){
    RB_Mutex_lock_Safe();
    uint8_t ret = RB_Peek_String(rb_handle, output_addr, peek_Length);
    RB_Mutex_unlock_Safe();
    return ret ;  
}

uint32_t RB_Safe_Get_Length(ring_buffer *rb_handle){    
    RB_Mutex_lock_Safe();
    uint32_t ret = RB_Get_Length(rb_handle);
    RB_Mutex_unlock_Safe();
    return ret ;  
}

uint32_t RB_Safe_Get_FreeSize(ring_buffer *rb_handle){    
    RB_Mutex_lock_Safe();
    uint32_t ret = RB_Get_FreeSize(rb_handle);
    RB_Mutex_unlock_Safe();
	  return ret ;  
}

