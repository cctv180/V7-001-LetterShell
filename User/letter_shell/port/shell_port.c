/**
 * @brief   shell移植到STM32F767时的接口实现
 * @author	mculover666
 * @date    2020/09/13 
*/

#include "shell.h"
#include "bsp.h"
#include "shell_port.h"

/* 1. 创建shell对象，开辟shell缓冲区 */
Shell shell;
char shell_buffer[512];

int test2(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
	printf("input int: %d, %d, %d, %d, %d, %d, %d, %d\r\n", i1, i2, i3, i4, i5, i6, i7, i8);
	return 0;
}
//导出到命令列表里
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test2, test2, test2);

/**
 * @brief shell读取数据函数原型
 *
 * @param char shell读取的字符
 *
 * @return char 0 读取数据成功
 * @return char -1 读取数据失败
 */
signed char userShellRead(char* data)
{
	return (comGetChar(COM1, (uint8_t*)data) == 1)? 0 : -1;
}

/* 2. 自己实现shell写函数 */
void User_Shell_Write(const char ch)
{
	//调用STM32 HAL库 API 使用查询方式发送
//	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
	
	comSendChar(COM1, ch);
}

/* 3. 编写初始化函数 */
void User_Shell_Init(void)
{
	//注册自己实现的写函数
	shell.write = User_Shell_Write;
	shell.read = userShellRead;
		//调用shell初始化函数
    shellInit(&shell, shell_buffer, 512);
}
