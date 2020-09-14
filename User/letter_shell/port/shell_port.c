/**
 * @brief   shell��ֲ��STM32F767ʱ�Ľӿ�ʵ��
 * @author	mculover666
 * @date    2020/09/13 
*/

#include "shell.h"
#include "bsp.h"
#include "shell_port.h"

/* 1. ����shell���󣬿���shell������ */
Shell shell;
char shell_buffer[512];

int test2(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
	printf("input int: %d, %d, %d, %d, %d, %d, %d, %d\r\n", i1, i2, i3, i4, i5, i6, i7, i8);
	return 0;
}
//�����������б���
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test2, test2, test2);

/**
 * @brief shell��ȡ���ݺ���ԭ��
 *
 * @param char shell��ȡ���ַ�
 *
 * @return char 0 ��ȡ���ݳɹ�
 * @return char -1 ��ȡ����ʧ��
 */
signed char userShellRead(char* data)
{
	return (comGetChar(COM1, (uint8_t*)data) == 1)? 0 : -1;
}

/* 2. �Լ�ʵ��shellд���� */
void User_Shell_Write(const char ch)
{
	//����STM32 HAL�� API ʹ�ò�ѯ��ʽ����
//	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
	
	comSendChar(COM1, ch);
}

/* 3. ��д��ʼ������ */
void User_Shell_Init(void)
{
	//ע���Լ�ʵ�ֵ�д����
	shell.write = User_Shell_Write;
	shell.read = userShellRead;
		//����shell��ʼ������
    shellInit(&shell, shell_buffer, 512);
}
