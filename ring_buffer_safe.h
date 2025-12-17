/*
 * @Author: sunzigang 1026733696@qq.com
 * @Date: 2023-11-29 14:38:59
 * @LastEditors: sunzigang 1026733696@qq.com
 * @LastEditTime: 2023-11-29 15:21:41
 * @FilePath: \JIG\Utilities\third_party\RingBuffer\ring_buffer_adapter.h
 * @Description:  环形缓冲区 接口适配器， 可以push/pop各种类型的数据
 * 
 */
#ifndef __RING_BUFFER_SAFE_H_
#define __RING_BUFFER_SAFE_H_

#include "ring_buffer.h"

uint8_t RB_Safe_Init(ring_buffer *rb_handle, void *buffer_addr ,uint32_t buffer_size, uint32_t buffer_Type);               //初始化基础环形缓冲区
uint8_t RB_Safe_Delete(ring_buffer *rb_handle, uint32_t Length);                                        //从头指针开始删除指定长度的数据
uint8_t RB_Safe_Write_Byte(ring_buffer *rb_handle, void* data);                                       //向缓冲区尾指针写一个包的数据 (包长度由 buffer_Type 确定)
uint8_t RB_Safe_Write_String(ring_buffer *rb_handle, void *input_addr, uint32_t write_Length);       //向缓冲区尾指针写指定包数量的数据
uint8_t RB_Safe_Read_Byte(ring_buffer *rb_handle, void *output_addr);                                //从缓冲区头指针读一个包的数据
uint8_t RB_Safe_Read_String(ring_buffer *rb_handle, void *output_addr, uint32_t read_Length);        //从缓冲区头指针读指定包数量的数据
uint32_t RB_Safe_Get_Length(ring_buffer *rb_handle);                                                    //获取缓冲区里已储存的包长度
uint32_t RB_Safe_Get_FreeSize(ring_buffer *rb_handle);                                                  //获取缓冲区可用储存空间
uint8_t RB_Safe_Peek_Byte(ring_buffer *rb_handle, void *output_addr);                                   //从缓冲区头指针读写一个包的数据 ,但不修改索引
uint8_t RB_Safe_Peek_String(ring_buffer *rb_handle, void *output_addr, uint32_t peek_Length);           //从缓冲区头指针指定包数量的数据 ,但不修改索引


#endif
