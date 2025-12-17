/*
 * @Author: sunzigang 1026733696@qq.com
 * @Date: 2023-11-29 14:37:11
 * @LastEditors: sunzigang 1026733696@qq.com
 * @LastEditTime: 2023-11-29 14:37:31
 * @FilePath: \JIG\Utilities\third_party\RingBuffer\demo.c
 * @Description: 
 * 
 */
//引用相关头文件
#include <stdint.h>
#include <stdio.h>
#include "ring_buffer.h"

//创建一个数组作为数据存储空间
#define BUFFER_SIZE 128
static uint8_t buffer[BUFFER_SIZE];

//创建环形缓冲区句柄
static ring_buffer rb;

int main(void)
{
    //初始化环形缓冲区参数
    RB_Init(&rb, buffer, BUFFER_SIZE);

    //写入向环形缓冲区写入数据
    RB_Write_String(&rb, "hello world", 11);
    RB_Write_Byte(&rb, '!');
    RB_Write_Byte(&rb, 0x00);

    //删除环形缓冲区部分数据
    RB_Delete(&rb, 2);

    //获取已储存的数据长度
    uint32_t num = RB_Get_Length(&rb);

    //读出环形缓冲区中的数据并打印
    uint8_t get[16];
    RB_Read_String(&rb, get, num);
    printf("%s", get);
    
    //控制台输出内容
    //llo world!
    return 0;
}