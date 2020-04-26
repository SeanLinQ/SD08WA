/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: protocol.c
**��        ��: �·�/�ϱ����ݴ�����
**ʹ �� ˵ �� :

                  *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********

** 1���û��ڴ��ļ���ʵ�������·�/�ϱ�����
** 2��DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
** 3������ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v2.5.1
** �ա���: 2018��10��27��
** �衡��: 1:Ĭ�Ϲر���������
           2:����03Э��wifi״̬�궨��
		   3:�������޸Ĳ��ֺ���ע��
		   
** ��  ��: v2.5.0
** �ա���: 2018��4��18��
** �衡��: 1:Э��汾��Ϊ0x03
           2:����WIFIģ�������رչ���
           3:������������

** ��  ��: v2.3.8
** �ա���: 2018��1��17��
** �衡��: 1:�������volatile��ֹ�������Ż�
           2:���#error��ʾ

** ��  ��: v2.3.7
** �ա���: 2017��4��18��
** �衡��: 1:�Ż����ڶ��н��մ���
		   
** ��  ��: v2.3.6
** �ա���: 2016��7��21��
** �衡��: 1:�޸���ȡ����ʱ�����
           2:���hex_to_bcdת������
		   
** ��  ��: v2.3.5
** �ա���: 2016��6��3��
** �衡��: 1:�޸ķ���Э��汾Ϊ0x01
           2:�̼���������ƫ�����޸�Ϊ4�ֽ�

** ��  ��: v2.3.4
** �ա���: 2016��5��26��
** �衡��: 1:�Ż����ڽ�������
           2:�Ż�������������,ȡ��enum���Ͷ���

** ��  ��: v2.3.3
** �ա���: 2016��5��24��
** �衡��: 1:�޸�mcu��ȡ����ʱ�亯��
           2:���wifi���ܲ���

** ��  ��: v2.3.2
** �ա���: 2016��4��23��
** �衡��: 1:�Ż��������ݽ���
           2:�Ż�MCU�̼���������
           3:�Ż��ϱ�����

** ��  ��: v2.3.1
** �ա���: 2016��4��15��
** �衡��: 1:�Ż��������ݽ���

** ��  ��: v2.3
** �ա���: 2016��4��14��
** �衡��: 1:֧��MCU�̼���������

** ��  ��: v2.2
** �ա���: 2016��4��11��
** �衡��: 1:�޸Ĵ������ݽ��շ�ʽ

** ��  ��: v2.1
** �ա���: 2016��4��8��
** �衡��: 1:����ĳЩ��������֧�ֺ���ָ�����ѡ��

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
           2:��ʡRAM�ռ�
**
**-----------------------------------------------------------------------------
******************************************************************************/

#include "wifi.h"

#include "userApp.h"

extern uint8_t PMU_Status;

#ifdef WEATHER_ENABLE
/******************************************************************************
                        �������ݲ���ѡ������
          **�û������Զ�����Ҫ�Ĳ�����ע�ͻ���ȡ��ע�ͼ��ɣ�ע�����**         
******************************************************************************/
const char weather_choose[WEATHER_CHOOSE_CNT][10] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",*/
 };
#endif

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�wifi_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�wifi����ļ����ļ���include "wifi.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�wifi_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�wifi_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_LED_SWITCH_1, DP_TYPE_BOOL},
  {DPID_BRIGHT_VALUE_1, DP_TYPE_VALUE},
  {DPID_BRIGHTNESS_MIN_1, DP_TYPE_VALUE},
  {DPID_LED_TYPE_1, DP_TYPE_ENUM},
};


/******************************************************************************
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/*****************************************************************************
�������� : uart_transmit_output
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
//  #error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"

  //ʾ��:
  //extern void Uart_PutChar(unsigned char value);
  //Uart_PutChar(value);	                                //���ڷ��ͺ���
	extern void UART0_SendByte(unsigned char value);
	UART0_SendByte(value);                                  //���ڷ��ͺ���

}
/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/

//�Զ������������ϱ�����

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
*****************************************************************************/
void all_data_update(void)
{
//  #error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
  /* 
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
  mcu_dp_bool_update(DPID_LED_SWITCH_1,��ǰ����); //BOOL�������ϱ�;
  mcu_dp_value_update(DPID_BRIGHT_VALUE_1,��ǰ����ֵ); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1,��ǰ��С����); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_LED_TYPE_1,��ǰ��Դ����); //ö���������ϱ�;

 */
  mcu_dp_bool_update(DPID_LED_SWITCH_1,Device_Param.led_switch); //BOOL�������ϱ�;
  mcu_dp_value_update(DPID_BRIGHT_VALUE_1,Device_Param.bright_value); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1,Device_Param.brightness_min); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_LED_TYPE_1,Device_Param.led_type); //ö���������ϱ�;
}


/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_led_switch_1_handle
�������� : ���DPID_LED_SWITCH_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_led_switch_1_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char led_switch_1;
  
  led_switch_1 = mcu_get_dp_download_bool(value,length);
  workMode = MODE_NORMAL;
  if(led_switch_1 == 0)
  {
    //���ع�
		Device_Param.brightness_en = FALSE;
		
		if(workMode == MODE_BRIGHT_SET)
		{
			Device_Param.led_angle = angleBackup;
			Device_Param.led_angle_min = angleMinBackup;
			Device_Param.bright_value = brightnessBackup;
    }

		//DIM_L;	
		Device_Param.led_switch = OFF;
	  light_angle = 0;
		PMU_Status = 0;
		UartB_SendData(UART_SWITCH, 0x00); //���ʹ������� 
		//Delay_Ms(20);
		//Dimmer_Save_Prmtr();	
  }
  else
  {
    //���ؿ�
		Device_Param.led_switch = 1;
    Device_Param.brightness_en = TRUE;
		if(workMode == MODE_BRIGHT_SET)
		{
			Device_Param.led_angle = angleBackup;
			Device_Param.led_angle_min = angleMinBackup;
			Device_Param.bright_value = brightnessBackup;
			light_angle = Device_Param.led_angle;
    }
		else
		{
			//DIM_H;		
			// �е���ֵ����ʾ��Ӧ����		
			if(Device_Param.led_angle)
			{
				light_angle = Device_Param.led_angle;
			}
	  }
		UartB_SendData(UART_BRIGHT_VALUE, light_angle); //���ʹ������� 
  }
	Dimmer_Save_Prmtr();
  Device_Param.led_switch = led_switch_1;
	
	//Dimmer_Save_Prmtr();
	
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_LED_SWITCH_1,led_switch_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_bright_value_1_handle
�������� : ���DPID_BRIGHT_VALUE_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_bright_value_1_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long bright_value_1;
  
  bright_value_1 = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */
  
	Device_Param.bright_value = bright_value_1;
	//Device_Param.led_angle = (unsigned char)(Device_Param.bright_value/10 +40);  // 40�㵼ͨ��Ϊ%0  140�㵼ͨ��Ϊ100%
	Device_Param.led_angle = (unsigned char)(Device_Param.bright_value*(MAX_BR-Device_Param.led_angle_min)/1000+Device_Param.led_angle_min);  // 40�㵼ͨ��Ϊ%0  140�㵼ͨ��Ϊ100%

	light_angle = Device_Param.led_angle;
	
	//Dimmer_Save_Prmtr();
	
	UartB_SendData(UART_BRIGHT_VALUE, Device_Param.led_angle); //���ʹ������� 
	Dimmer_Save_Prmtr();
	
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_BRIGHT_VALUE_1,bright_value_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_brightness_min_1_handle
�������� : ���DPID_BRIGHTNESS_MIN_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_brightness_min_1_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long brightness_min_1;
  
  brightness_min_1 = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */
	//(workMode == MODE_NORMAL) ? (SetFinishFlag = FALSE):(SetFinishFlag = TRUE);
	//if(workMode == MODE_BRIGHT_SET)
	{
		Device_Param.brightness_min = brightness_min_1;
		if(brightness_min_1 == 10)
			Device_Param.led_angle_min = 40;
		else
			Device_Param.led_angle_min = Device_Param.brightness_min/10+40;  //������ȵ�ͨ��Ϊ%1  140�㵼ͨ��Ϊ100%
		
		Device_Param.led_angle = Device_Param.led_angle_min;
		light_angle = Device_Param.led_angle;
		
		Dimmer_Save_Prmtr();
		UartB_SendData(UART_BRIGHT_VALUE_MIN, Device_Param.led_angle_min); //���ʹ������� 
	}
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1,brightness_min_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_led_type_1_handle
�������� : ���DPID_LED_TYPE_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_led_type_1_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char led_type_1;
  
  led_type_1 = mcu_get_dp_download_enum(value,length);
  switch(led_type_1)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    default:
      
      break;
  }
	
	Device_Param.led_type = led_type_1;
	
	Dimmer_Save_Prmtr();
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_LED_TYPE_1,led_type_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  #error "���������RTCʱ��д�����,��ɾ������"
  /*
  time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
  time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
  time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
  time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
  time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
  time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
 */
  if(time[0] == 1)
  {
    //��ȷ���յ�wifiģ�鷵�صı���ʱ������ 
	 
  }
  else
  {
  	//��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
�������� : wifi_test_result
�������� : wifi���ܲ��Է���
������� : result:wifi���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//  #error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
  if(result == 0)
  {
    //����ʧ��
    if(rssi == 0x00)
    {
      //δɨ�赽����Ϊtuya_mdev_test·����,����
    }
    else if(rssi == 0x01)
    {
      //ģ��δ��Ȩ
    }
  }
  else
  {
    //���Գɹ�
    //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
  }
  
}
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
�������� : mcu_firm_update_handle
�������� : MCU����̼�����ģʽ
������� : value:�̼�������
           position:��ǰ���ݰ����ڹ̼�λ��
           length:��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  #error "���������MCU�̼���������,��ɺ���ɾ������"
  if(length == 0)
  {
    //�̼����ݷ������
    
  }
  else
  {
    //�̼����ݴ���
  }
  
  return SUCCESS;
}
#endif
/******************************************************************************
                                WARNING!!!                     
���º����û������޸�!!
******************************************************************************/

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
           value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  ��ǰ����������·�/���ϱ����ݵ���                    
  ���庯������Ҫʵ���·����ݴ���
  �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_LED_SWITCH_1:
      //���ش�����
      ret = dp_download_led_switch_1_handle(value,length);
      break;
    case DPID_BRIGHT_VALUE_1:
      //����ֵ������
      ret = dp_download_bright_value_1_handle(value,length);
      break;
    case DPID_BRIGHTNESS_MIN_1:
      //��С���ȴ�����
      ret = dp_download_brightness_min_1_handle(value,length);
      break;
    case DPID_LED_TYPE_1:
      //��Դ���ʹ�����
      ret = dp_download_led_type_1_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}

#ifdef WEATHER_ENABLE

/*****************************************************************************
�������� : weather_open_return
�������� : ���������ܷ����û��Դ�����
������� : 1:res:���������ܷ��ؽ����1:�ɹ���0��ʧ��

���ز��� : ��
*****************************************************************************/
void weather_open_return_handle(unsigned char res, unsigned char err)
{
  #error "���������M���������ܷ������ݴ������,��ɺ���ɾ������"
  unsigned char err_num = 0;
  
  if(res == 1)
  {
    //���������سɹ�
  }
  else if(res == 0)
  {
    //����������ʧ��
    err_num = err;//��ȡ������
  }
}

/*****************************************************************************
�������� : weather_data_user_handle
�������� : ���������û��Դ�����
������� : name:������
           type:�������ͣ�0��int�ͣ�1��string��
           data:����ֵ�ĵ�ַ
���ز��� : ��
*****************************************************************************/
void weather_data_user_handle(char *name, unsigned char type, char *data)
{
  #error "���������ʾ���������������������ݴ������,��ɺ���ɾ������"
  int value_int;
  char value_string[50];//�����еĲ������ݽ϶࣬����Ĭ��Ϊ50�������Ը��ݶ���Ĳ����������ʵ����ٸ�ֵ
  
  my_memset(value_string, '/0', 50);
  
  //���Ȼ�ȡ��������
  if(type == 0)//������INT��
  {
    value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
  }
  else if(type == 1)
  {
    my_strcpy(value_string, data);
  }
  
  //ע��Ҫ������ѡ������������ò���ֵ������
  if(my_strcmp(name, "temp") == 0)
  {
    //printf("temp value is:%d", value_int);            //int��
  }
  else if(my_strcmp(name, "humidity") == 0)
  {
    //printf("humidity value is:%d", value_int);        //int��
  }
  else if(my_strcmp(name, "pm25") == 0)
  {
    //printf("pm25 value is:%d", value_int);            //int��
  }
  else if(my_strcmp(name, "condition") == 0)
  {
    //printf("condition value is:%s", value_string);    //string��
  }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/*****************************************************************************
�������� : stream_file_trans
�������� : �������ļ�����
������� : id:ID��
          buffer:���Ͱ��ĵ�ַ
          buf_len:���Ͱ�����
���ز��� : ��
*****************************************************************************/
unsigned char stream_file_trans(unsigned int id, unsigned char *buffer, unsigned long buf_len)
{
  #error "���������ʾ�������������������������,��ɺ���ɾ������"
  unsigned short length = 0;
  unsigned long map_offset = 0;
  unsigned int pack_num = 0;
  unsigned int rest_length = 0;

  if(stop_update_flag == ENABLE)
    return SUCCESS;

  pack_num = buf_len / STREM_PACK_LEN;
  rest_length = buf_len - pack_num * STREM_PACK_LEN;
  if (rest_length > 0)
  {
    pack_num++;
  }

  int this_len = STREM_PACK_LEN;
  for (int cnt = 0; cnt < pack_num; cnt++)
  {
    if (cnt == pack_num - 1 && rest_length > 0)
    {
      this_len = rest_length;
    }
    else
    {
      this_len = STREM_PACK_LEN;
    }

    stream_trans(id, map_offset, buffer + map_offset, this_len);

    //while(stream_status == 0xff);//�յ�����
    
    if(stream_status != 0)
    {
      return ERROR;
    }
  }
  
  return SUCCESS;
}

#endif
