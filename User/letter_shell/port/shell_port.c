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

#ifdef DEBUG_MODE
void test(int a, float b, int c, float d)
{
	printf("%d, %f, %d, %f \r\n", a, b, c, d);
}
SHELL_EXPORT_CMD_AGENCY(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
	test, test, test,
	p1, SHELL_PARAM_FLOAT(p2), p3, SHELL_PARAM_FLOAT(p4));

int test2(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
    printf("input int: %d, %d, %d, %d, %d, %d, %d, %d\r\n", i1, i2, i3, i4, i5, i6, i7, i8);
    return 0;
}
//�����������б���
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test2, test2, test2);

/* SHELL_USING_CMD_EXPORT == 1��Ҫ������� */
int testfunc(int argc, char* agrv[])
{
    printf("%dparameter(s)\r\n", argc);
    for (char i = 1; i < argc; i++)
    {
        printf("%s\r\n", agrv[i]);
    }
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), testfunc, testfunc, testfunc);

/* ϵͳ��Ϣ */
int sysinfo(int argc, char* agrv[])
{
    uint32_t temp;
    temp = HAL_GetDEVID(); 			    BSP_INFO("HAL_GetDEVID            ------     0x%08x", temp);
    temp = HAL_GetHalVersion(); 		BSP_INFO("HAL_GetHalVersion       ------     0x%08x", temp);
    temp = HAL_GetREVID(); 				BSP_INFO("HAL_GetREVID            ------     0x%08x", temp);
    //temp = HAL_GetUIDw0(); 				BSP_INFO("HAL_GetUIDw0            ------     0x%08x", temp);
    //temp = HAL_GetUIDw1(); 				BSP_INFO("HAL_GetUIDw1            ------     0x%08x", temp);
    //temp = HAL_GetUIDw2(); 				BSP_INFO("HAL_GetUIDw2            ------     0x%08x", temp);
    temp = SystemCoreClock;     		BSP_INFO("SystemCoreClock         ------     0d%08d", temp);
    temp = HAL_RCC_GetHCLKFreq();   	BSP_INFO("HAL_RCC_GetHCLKFreq     ------     0d%08d", temp);
    temp = HAL_RCC_GetPCLK1Freq();  	BSP_INFO("HAL_RCC_GetPCLK1Freq    ------     0d%08d", temp);
    temp = HAL_RCC_GetPCLK2Freq();  	BSP_INFO("HAL_RCC_GetPCLK2Freq    ------     0d%08d", temp);
    temp = HAL_RCC_GetSysClockFreq();	BSP_INFO("HAL_RCC_GetSysClockFreq ------     0d%08d", temp);
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN) | SHELL_CMD_DISABLE_RETURN, sysinfo, sysinfo, sysinfo);

/* ��������� */
//int rand(int argc, char* agrv[])
//{
//    extern RNG_HandleTypeDef hrng;
//    uint32_t temp;
//
//    temp = HAL_RNG_GetRandomNumber(&hrng);
//    printf("Randon Number = 0x%08x\r\n", temp);
//    return 0;
//}
//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN) | SHELL_CMD_DISABLE_RETURN, rand, rand, rand);

#endif

/* ϵͳ���� */
int reboot(int argc, char *agrv[])
{
    printf("%dparameter(s)\r\n", argc);
    for (char i = 1; i < argc; i++)
    {
        printf("%s\r\n", agrv[i]);
    }
    HAL_NVIC_SystemReset();
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), reboot, reboot, reboot);

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
