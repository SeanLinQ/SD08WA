/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2017, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: protocol.c
**描        述: 下发/上报数据处理函数
**使 用 说 明 :

                  *******非常重要，一定要看哦！！！********

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
** 3、当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
**
**--------------当前版本修订---------------------------------------------------
** 版  本: v1.0
** 日　期: 2017年5月3日
** 描　述: 1:创建涂鸦bluetooth对接MCU_SDK
**
**-----------------------------------------------------------------------------
******************************************************************************/
//#include "include.h"

#include "bluetooth.h"

#include "userApp.h"
  
//extern TYPE_BUFFER_S FlashBuffer;

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的bt_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

         
/******************************************************************************
                              第一步:初始化
1:在需要使用到bt相关文件的文件中include "bt.h"
2:在MCU初始化中调用mcu_api.c文件中的bt_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的bt_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_LED_SWITCH_1, DP_TYPE_BOOL},
  {DPID_BRIGHT_VALUE_1, DP_TYPE_VALUE},
  {DPID_BRIGHTNESS_MIN_1, DP_TYPE_VALUE},
  {DPID_LED_TYPE_1, DP_TYPE_ENUM},
  {DPID_BRIGHTNESS_MAX_1, DP_TYPE_VALUE},
  {DPID_COUNTDOWN, DP_TYPE_VALUE},
  {DPID_WORK_MODE, DP_TYPE_ENUM},
};




/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处理
输入参数 : value:串口收到字节数据
返回参数 : 无
使用说明 : 请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
 //#error "请将MCU串口发送函数填入该函数,并删除该行"
/*
  //示例:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //串口发送函数
*/  
 extern void UART0_SendByte(unsigned char value);
 UART0_SendByte(value); 
}
/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传,实现APP和muc数据同步
输入参数 : 无
返回参数 : 无
使用说明 : 此函数SDK内部需调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数据
*****************************************************************************/
void all_data_update(void)
{
	/*
  #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"
  //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
  mcu_dp_bool_update(DPID_LED_SWITCH_1,当前开关1); //BOOL型数据上报;
  mcu_dp_value_update(DPID_BRIGHT_VALUE_1,当前亮度值1); //VALUE型数据上报;
  mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1,当前最小亮度1); //VALUE型数据上报;
  mcu_dp_enum_update(DPID_LED_TYPE_1,当前光源类型1); //枚举型数据上报;
  mcu_dp_value_update(DPID_BRIGHTNESS_MAX_1,当前最大亮度1); //VALUE型数据上报;
  mcu_dp_value_update(DPID_COUNTDOWN,当前倒计时1); //VALUE型数据上报;
  mcu_dp_enum_update(DPID_WORK_MODE,当前模式); //枚举型数据上报;
*/
  mcu_dp_bool_update(DPID_LED_SWITCH_1,Device_Param.led_switch); //BOOL型数据上报;
  mcu_dp_value_update(DPID_BRIGHT_VALUE_1,Device_Param.bright_value); //VALUE型数据上报;
  mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1,Device_Param.brightness_min); //VALUE型数据上报;
  mcu_dp_enum_update(DPID_LED_TYPE_1,Device_Param.led_type); //枚举型数据上报;

}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_led_switch_1_handle
功能描述 : 针对DPID_LED_SWITCH_1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_led_switch_1_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为BOOL
  unsigned char ret;
  //0:关/1:开
  unsigned char led_switch_1;
  
  led_switch_1 = mcu_get_dp_download_bool(value,length);
  workMode = MODE_NORMAL;
  if(led_switch_1 == 0)
  {
    //开关关
	period_flag = FALSE;
    //开关关
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
		UartB_SendData(UART_SWITCH, 0x00); //发送串口数据 
		//Delay_Ms(20);
		//Dimmer_Save_Prmtr();
  }
  else
  {
    //开关开
	//开关开
		period_flag = TRUE;
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
			// 有调光值，显示对应亮度		
			if(Device_Param.led_angle)
			{
				light_angle = Device_Param.led_angle;
			}
	  }
		UartB_SendData(UART_BRIGHT_VALUE, light_angle); //发送串口数据 
  }
  
  Dimmer_Save_Prmtr();
  Device_Param.led_switch = led_switch_1;
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_bool_update(DPID_LED_SWITCH_1,led_switch_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_bright_value_1_handle
功能描述 : 针对DPID_BRIGHT_VALUE_1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_bright_value_1_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long bright_value_1;
  
  bright_value_1 = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  Device_Param.bright_value = bright_value_1;
	//Device_Param.led_angle = (unsigned char)(Device_Param.bright_value/10 +40);  // 40°导通角为%0  140°导通角为100%
	Device_Param.led_angle = (unsigned char)(Device_Param.bright_value*(MAX_BR-Device_Param.led_angle_min)/1000+Device_Param.led_angle_min);  // 40°导通角为%0  140°导通角为100%

	light_angle = Device_Param.led_angle;
	
	//Dimmer_Save_Prmtr();
	
	UartB_SendData(UART_BRIGHT_VALUE, Device_Param.led_angle); //发送串口数据 
	Dimmer_Save_Prmtr();
	
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_BRIGHT_VALUE_1,bright_value_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_brightness_min_1_handle
功能描述 : 针对DPID_BRIGHTNESS_MIN_1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_brightness_min_1_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long brightness_min_1;
  
  brightness_min_1 = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  //(workMode == MODE_NORMAL) ? (SetFinishFlag = FALSE):(SetFinishFlag = TRUE);
	//if(workMode == MODE_BRIGHT_SET)
	{
		Device_Param.brightness_min = brightness_min_1;
		if(brightness_min_1 == 10)
			Device_Param.led_angle_min = 40;
		else
			Device_Param.led_angle_min = Device_Param.brightness_min/10+40;  //最低亮度导通角为%1  140°导通角为100%
		
		Device_Param.led_angle = Device_Param.led_angle_min;
		light_angle = Device_Param.led_angle;
		
		Dimmer_Save_Prmtr();
		UartB_SendData(UART_BRIGHT_VALUE_MIN, Device_Param.led_angle_min); //发送串口数据 
	}
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1,brightness_min_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_led_type_1_handle
功能描述 : 针对DPID_LED_TYPE_1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_led_type_1_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
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
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_LED_TYPE_1,led_type_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_brightness_max_1_handle
功能描述 : 针对DPID_BRIGHTNESS_MAX_1的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_brightness_max_1_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long brightness_max_1;
  
  brightness_max_1 = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_BRIGHTNESS_MAX_1,brightness_max_1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_countdown_handle
功能描述 : 针对DPID_COUNTDOWN的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_countdown_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为VALUE
  unsigned char ret;
  unsigned long countdown;
  
  countdown = mcu_get_dp_download_value(value,length);
  /*
  //VALUE类型数据处理
  
  */
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_value_update(DPID_COUNTDOWN,countdown);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_work_mode_handle
功能描述 : 针对DPID_WORK_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_work_mode_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char work_mode;
  
  work_mode = mcu_get_dp_download_enum(value,length);
  switch(work_mode)
  {
    case 0:
      
      break;
      
    default:
      
      break;
  }
  
  //处理完DP数据后应有反馈
  ret = mcu_dp_enum_update(DPID_WORK_MODE,work_mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : mcu_firm_update_handle
功能描述 : MCU进入固件升级模式
输入参数 : value:固件缓冲区
           position:当前数据包在于固件位置
           length:当前固件包长度(固件包长度为0时,表示固件包发送完成)
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  #error "请自行完成MCU固件升级代码,完成后请删除该行"
  unsigned long addr;
 
  if(length == 0)
  {
#ifdef ENABLE_BOOT
    //固件数据发送完成
    FlashBuffer.magic_code = FIREWARE_UPDATE_FLAG;
    
    if(Earse_Flash(PARA_ADDR) == ERROR)
      return ERROR;
    
    //写入升级标志
    if(Write_Flash(PARA_ADDR,(unsigned char *)&FlashBuffer,sizeof(FlashBuffer)) == ERROR)
      return ERROR;
    
    Reset();
#endif
  }
  else
  {
    //固件数据处理
    addr = FIREWARE_ADDR_H;
     
    if(position % 1024 == 0)
    {
      if(Earse_Flash(addr + position) == ERROR)
        return ERROR;
    }
    
    if(Write_Flash(addr + position,(unsigned char *)value,length) == ERROR)
      return ERROR;
  }

  return SUCCESS;
}
#endif
/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  当前函数处理可下发/可上报数据调用                    
  具体函数内需要实现下发数据处理
  完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_LED_SWITCH_1:
      //开关1处理函数
      ret = dp_download_led_switch_1_handle(value,length);
      break;
    case DPID_BRIGHT_VALUE_1:
      //亮度值1处理函数
      ret = dp_download_bright_value_1_handle(value,length);
      break;
    case DPID_BRIGHTNESS_MIN_1:
      //最小亮度1处理函数
      ret = dp_download_brightness_min_1_handle(value,length);
      break;
    case DPID_LED_TYPE_1:
      //光源类型1处理函数
      ret = dp_download_led_type_1_handle(value,length);
      break;
    case DPID_BRIGHTNESS_MAX_1:
      //最大亮度1处理函数
      ret = dp_download_brightness_max_1_handle(value,length);
      break;
    case DPID_COUNTDOWN:
      //倒计时1处理函数
      ret = dp_download_countdown_handle(value,length);
      break;
    case DPID_WORK_MODE:
      //模式处理函数
      ret = dp_download_work_mode_handle(value,length);
      break;


  default:
    break;
  }
  return ret;
}
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
