/**
 * \file ring_buffer.c
 * \brief 简易环形缓冲的实现
 * \author netube_99\netube@163.com
 * \date 2022.08.20
 * \version v0.4.0
*/

#include <stdint.h>
#include <string.h>
#include "ring_buffer.h"

/**
 * \brief 初始化新缓冲区
 * \param[out] rb_handle: 待初始化的缓冲区结构体句柄
 * \param[in] buffer_addr: 外部定义的缓冲区数组，类型必须为 uint8_t
 * \param[in] buffer_size: 外部定义的缓冲区数组空间
 * \return 返回缓冲区初始化的结果
 *      \arg RING_BUFFER_SUCCESS: 初始化成功
 *      \arg RING_BUFFER_ERROR: 初始化失败
*/
uint8_t RB_Init(ring_buffer *rb_handle, void *buffer_addr ,uint32_t buffer_size, uint32_t buffer_Type)
{
    //缓冲区数组空间必须大于2且小于数据类型最大值
    if(buffer_size < 2 || buffer_size == 0xFFFFFFFF)
        return RING_BUFFER_ERROR ; //初始化失败
    rb_handle->head = 0 ; //复位头指针
    rb_handle->tail = 0 ; //复位尾指针
    rb_handle->Length = 0 ; //复位已存储数据长度 
    rb_handle->array_addr = buffer_addr ; //缓冲区储存数组基地址
    rb_handle->max_Length = buffer_size ; //缓冲区最大可储存数据量
    rb_handle->arrayTypeLen = buffer_Type ; //缓冲区储存数据类型的长度   
    return RING_BUFFER_SUCCESS ; //缓冲区初始化成功
}

/**
 * \brief 从头指针开始删除指定长度的数据
 * \param[out] rb_handle: 缓冲区结构体句柄
 * \param[in] Length: 要删除的长度
 * \return 返回删除指定长度数据结果
 *      \arg RING_BUFFER_SUCCESS: 删除成功
 *      \arg RING_BUFFER_ERROR: 删除失败
*/
uint8_t RB_Delete(ring_buffer *rb_handle, uint32_t Length)
{
    if(rb_handle->Length < Length)
        return RING_BUFFER_ERROR ;//已储存的数据量小于需删除的数据量
    else
    {
        if((rb_handle->head + Length) >= rb_handle->max_Length) //掉头
            rb_handle->head = Length - (rb_handle->max_Length - rb_handle->head);
        else
            rb_handle->head += Length ;    //头指针向前推进，抛弃数据
        rb_handle->Length -= Length ;      //重新记录有效数据长度
        return RING_BUFFER_SUCCESS ;//已储存的数据量小于需删除的数据量
    }
}

/**
 * \brief 向缓冲区尾部写一个字节
 * \param[out] rb_handle: 缓冲区结构体句柄
 * \param[in] data: 要写入的字节
 * \return 返回缓冲区写字节的结果
 *      \arg RING_BUFFER_SUCCESS: 写入成功
 *      \arg RING_BUFFER_ERROR: 写入失败
*/
uint8_t RB_Write_Byte(ring_buffer *rb_handle, void* data)
{
    //缓冲区数组已满，产生覆盖错误
    if(rb_handle->Length == (rb_handle->max_Length))
        return RING_BUFFER_ERROR ;
    else
    {
        memcpy((uint8_t*)rb_handle->array_addr + rb_handle->tail*rb_handle->arrayTypeLen, data, rb_handle->arrayTypeLen); //基地址+尾指针偏移量*数据类型长度，存放数据
        rb_handle->Length ++ ;//数据量计数+1
        rb_handle->tail ++ ;//尾指针后移
    }
    //如果尾指针超越了数组末尾，尾指针指向缓冲区数组开头，形成闭环
    if(rb_handle->tail > (rb_handle->max_Length - 1))
        rb_handle->tail = 0 ;
	return RING_BUFFER_SUCCESS ;
}

/**
 * \brief 从缓冲区头指针读取一个字节
 * \param[out] rb_handle: 缓冲区结构体句柄
 * \param[out] output_addr: 读取的字节保存地址
 * \return 返回读取状态
 *      \arg RING_BUFFER_SUCCESS: 读取成功
 *      \arg RING_BUFFER_ERROR: 读取失败
*/
uint8_t RB_Read_Byte(ring_buffer *rb_handle, void *output_addr)
{
    if (rb_handle->Length != 0)//有数据未读出
    {
        memcpy((uint8_t*)output_addr, (uint8_t*)rb_handle->array_addr + rb_handle->head*rb_handle->arrayTypeLen, rb_handle->arrayTypeLen); //基地址+头指针偏移量*数据类型长度，存放数据
        rb_handle->head ++ ;
        rb_handle->Length -- ;//数据量计数-1
        //如果头指针超越了数组末尾，头指针指向数组开头，形成闭环
        if(rb_handle->head > (rb_handle->max_Length - 1))
            rb_handle->head = 0 ;
        return RING_BUFFER_SUCCESS ;
    }
    return RING_BUFFER_ERROR ;
}

/**
 * \brief 向缓冲区尾部写指定长度的数据
 * \param[out] rb_handle: 缓冲区结构体句柄
 * \param[out] input_addr: 待写入数据的基地址
 * \param[in] write_Length: 要写入的字节数
 * \return 返回缓冲区尾部写指定长度字节的结果
 *      \arg RING_BUFFER_SUCCESS: 写入成功
 *      \arg RING_BUFFER_ERROR: 写入失败
*/
uint8_t RB_Write_String(ring_buffer *rb_handle, void *input_addr, uint32_t write_Length)
{
    //如果不够存储空间存放新数据,返回错误
    if((rb_handle->Length + write_Length) > (rb_handle->max_Length))
        return RING_BUFFER_ERROR ;
    else
    {
        //设置两次写入长度
        uint32_t write_size_a, write_size_b ;
        //如果顺序可用长度小于需写入的长度，需要将数据拆成两次分别写入
        if((rb_handle->max_Length - rb_handle->tail) < write_Length)
        {
            write_size_a = rb_handle->max_Length - rb_handle->tail ;//从尾指针开始写到储存数组末尾
            write_size_b = write_Length - write_size_a ;//从储存数组开头写数据
            //分别拷贝a、b段数据到储存数组中
            
            memcpy((uint8_t*)rb_handle->array_addr + rb_handle->tail*rb_handle->arrayTypeLen, input_addr, rb_handle->arrayTypeLen*write_size_a); //基地址+尾指针偏移量*数据类型长度，存放数据
            memcpy((uint8_t*)rb_handle->array_addr , (uint8_t*)input_addr + rb_handle->arrayTypeLen*write_size_a, rb_handle->arrayTypeLen*write_size_b); //基地址+尾指针偏移量*数据类型长度，存放数据
            rb_handle->Length += write_Length ;//记录新存储了多少数据量
            rb_handle->tail = write_size_b ;//重新定位尾指针位置
        }
        else//如果顺序可用长度大于或等于需写入的长度，则只需要写入一次
        {
            write_size_a = write_Length ;//从尾指针开始写到储存数组末尾
            memcpy((uint8_t*)rb_handle->array_addr + rb_handle->tail*rb_handle->arrayTypeLen, input_addr, rb_handle->arrayTypeLen*write_size_a); //基地址+尾指针偏移量*数据类型长度，存放数据
            rb_handle->Length += write_Length ;//记录新存储了多少数据量
            rb_handle->tail += write_size_a ;//重新定位尾指针位置
            if(rb_handle->tail == rb_handle->max_Length)
                rb_handle->tail = 0 ;//如果写入数据后尾指针刚好写到数组尾部，则回到开头，防止越位
        }
        return RING_BUFFER_SUCCESS ;
    }
}

/**
 * \brief 从缓冲区头部读指定长度的数据，保存到指定的地址
 * \param[out] rb_handle: 缓冲区结构体句柄
 * \param[out] output_addr: 读取的数据保存地址
 * \param[in] read_Length: 要读取的字节数
 * \return 返回缓冲区头部读指定长度字节的结果
 *      \arg RING_BUFFER_SUCCESS: 读取成功
 *      \arg RING_BUFFER_ERROR: 读取失败
*/
uint8_t RB_Read_String(ring_buffer *rb_handle, void *output_addr, uint32_t read_Length)
{
    if(read_Length > rb_handle->Length)
        return RING_BUFFER_ERROR ;
    else
    {
        uint32_t Read_size_a, Read_size_b ;
        if(read_Length > (rb_handle->max_Length - rb_handle->head))
        {
            Read_size_a = rb_handle->max_Length - rb_handle->head ;
            Read_size_b = read_Length - Read_size_a ;
            memcpy(output_addr, (uint8_t*)rb_handle->array_addr + rb_handle->head*rb_handle->arrayTypeLen, rb_handle->arrayTypeLen*Read_size_a); //基地址+头指针偏移量*数据类型长度，存放数据
            memcpy((uint8_t*)output_addr + rb_handle->arrayTypeLen*Read_size_a, rb_handle->array_addr, rb_handle->arrayTypeLen*Read_size_b); //基地址+头指针偏移量*数据类型长度，存放数据
            rb_handle->Length -= read_Length ;//记录剩余数据量
            rb_handle->head = Read_size_b ;//重新定位头指针位置
        }
        else
        {
            Read_size_a = read_Length ;
            memcpy(output_addr, (uint8_t*)rb_handle->array_addr + rb_handle->head, rb_handle->arrayTypeLen*Read_size_a);
            rb_handle->Length -= read_Length ;//记录剩余数据量
            rb_handle->head += Read_size_a ;//重新定位头指针位置
            if(rb_handle->head == rb_handle->max_Length)
                rb_handle->head = 0 ;//如果读取数据后头指针刚好写到数组尾部，则回到开头，防止越位
        }
        return RING_BUFFER_SUCCESS ;
    }
}

/**
 * \brief 从缓冲区头部预读（Peek）指定长度的数据，不修改缓冲区状态
 * \param[in] rb_handle: 缓冲区结构体句柄
 * \param[out] output_addr: 预读数据保存地址
 * \param[in] peek_Length: 要预读的元素个数（注意：不是字节数，而是按 arrayTypeLen 为单位的个数）
 * \return 返回预读结果
 *      \arg RING_BUFFER_SUCCESS: 预读成功
 *      \arg RING_BUFFER_ERROR: 预读失败（如数据不足）
*/
uint8_t RB_Peek_String(ring_buffer *rb_handle, void *output_addr, uint32_t peek_Length)
{
    if (peek_Length == 0)
        return RING_BUFFER_SUCCESS; // 空读视为成功

    if (peek_Length > rb_handle->Length)
        return RING_BUFFER_ERROR; // 请求长度超过当前有效数据量

    uint32_t peek_size_a, peek_size_b;
    uint32_t head = rb_handle->head; // 使用局部变量，避免修改原始 head

    if (peek_Length > (rb_handle->max_Length - head))
    {
        // 跨越缓冲区末尾，需分两段读取
        peek_size_a = rb_handle->max_Length - head;
        peek_size_b = peek_Length - peek_size_a;

        memcpy(output_addr,
               (uint8_t*)rb_handle->array_addr + head * rb_handle->arrayTypeLen,
               rb_handle->arrayTypeLen * peek_size_a);

        memcpy((uint8_t*)output_addr + rb_handle->arrayTypeLen * peek_size_a,
               rb_handle->array_addr,
               rb_handle->arrayTypeLen * peek_size_b);
    }
    else
    {
        // 单段读取
        memcpy(output_addr,
               (uint8_t*)rb_handle->array_addr + head * rb_handle->arrayTypeLen,
               rb_handle->arrayTypeLen * peek_Length);
    }

    return RING_BUFFER_SUCCESS;
}

/**
 * \brief 从缓冲区头部预读（Peek）一个数据单元（不修改缓冲区状态）
 * \param[in] rb_handle: 缓冲区结构体句柄
 * \param[out] output_addr: 预读的数据保存地址（需至少 arrayTypeLen 字节空间）
 * \return 返回预读结果
 *      \arg RING_BUFFER_SUCCESS: 预读成功
 *      \arg RING_BUFFER_ERROR: 预读失败（缓冲区为空）
*/
uint8_t RB_Peek_Byte(ring_buffer *rb_handle, void *output_addr)
{
    if (rb_handle->Length == 0)
        return RING_BUFFER_ERROR; // 缓冲区为空，无法预读

    // 仅读取，不修改 head 或 Length
    memcpy(output_addr,
           (uint8_t*)rb_handle->array_addr + rb_handle->head * rb_handle->arrayTypeLen,
           rb_handle->arrayTypeLen);

    return RING_BUFFER_SUCCESS;
}

/**
 * \brief 获取缓冲区里已储存的数据长度
 * \param[in] rb_handle: 缓冲区结构体句柄
 * \return 返回缓冲区里已储存的数据长度
*/
uint32_t RB_Get_Length(ring_buffer *rb_handle)
{
    return rb_handle->Length ;
}

/**
 * \brief 获取缓冲区可用储存空间
 * \param[in] rb_handle: 缓冲区结构体句柄
 * \return 返回缓冲区可用储存空间
*/
uint32_t RB_Get_FreeSize(ring_buffer *rb_handle)
{
    return (rb_handle->max_Length - rb_handle->Length) ;
}

