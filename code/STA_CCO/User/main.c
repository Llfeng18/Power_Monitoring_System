#include "stm32f10x.h"
#include "bsp_usart.h"
#include "rx_data_queue.h"
#include "bsp_led.h"  
#include "bsp_key.h"  
#include "bsp_485.h"
#include "Delay.h"
#include "dlt645.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include "bsp_rtc.h"
#include "bsp_spi_flash.h"
#include "sc1667.h"
#include <stdio.h>
#include <stdlib.h>
#include "bsp_AdvanceTim.h" 

void System_Init(void);
void dlt645_test(void);
void SC1667_test(void);

__IO uint8_t CCO=0;
__IO uint8_t debug_index=0;
uint32_t distance_time = 0; // ms ¼ÆÊ±±äÁ¿ 
uint32_t distance_index = 0; // ¼ÆÊ±´ÎÊı

/* ·¢ËÍ»º³åÇø³õÊ¼»¯ */
uint8_t Automatic_data_acquisition[42];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
extern __IO uint8_t save_data_flash;
__IO uint8_t ShowTime=0,newTime=0,newSTA=0;
__IO uint8_t sc1667_initindex=0,system_initindex=0;
/*addressµÚÒ»¸öÊı×éÓÃÀ´µ±ÖĞ¼ä±äÁ¿,ºóÎå¸öÊı×éÓÃÀ´´æ´¢µç±íµØÖ·*/
uint8_t address[6][6]={{ 0xAA , 0xAA , 0xAA , 0xAA , 0xAA , 0xAA }};
//uint8_t address[6][18]={{ 0xAA , 0xAA , 0xAA , 0xAA , 0xAA , 0xAA }};
//uint8_t address[6][6]={{ 0xAA , 0xAA , 0xAA , 0xAA , 0xAA , 0xAA },{ 0x19 , 0x12 , 0x00 , 0x10 , 0x30 , 0x75 },{ 0x10 , 0x00 , 0x19 , 0x36 , 0x37 , 0x08 },{ 0x19 , 0x12 , 0x00 , 0x10 , 0x30 , 0x76 }};
uint32_t address_Baud_rate[6]={9600,9600,9600,9600,9600,9600};
//uint8_t sc1667_mailing_address[6]={0xfc,0x00,0x00,0xaf,0xbe,0x94};
uint8_t sc1667_mailing_address[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t sc1667_search_address[6][6];
uint8_t STA_meter_address[3][37];
//uint8_t sc1667_cco_address[6]={0xfc,0x00,0x00,0xaf,0xae,0x3e};
uint8_t sc1667_cco_address[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t Total_sc1667_num=0 ,Selected_sc1667=0;
//uint8_t address[6][6]={{ 0xAA , 0xAA , 0xAA , 0xAA , 0xAA , 0xAA }};
uint8_t Total_meters_num=0 ,Selected_meter=1;
uint8_t Address_Fully_Matched[2][6]={{ 0xAA , 0xAA , 0xAA , 0xAA , 0xAA , 0xAA },{ 0xFF , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
uint8_t sc1667_Address_Fully_Matched[6]={ 0xFF , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint32_t time_clock=0;
uint8_t Default_password[9]={0x33,0x32,0x31,0x71,0x64,0x75,0x7A,0x66,0x08}; //³õÊ¼ÍøÂç¿ÚÁîfzudq123
uint8_t Default_password_empty[1]={0x00};                                     //Çå¿ÕÍøÂç¿ÚÁî
uint8_t search_data[13]={0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x0A,0x1F,0x33};
uint8_t rec_user_data[200];
uint8_t send_user_Data[200];
// N = 2^32/365/24/60/60 = 136 Äê

/*Ê±¼ä½á¹¹Ìå£¬Ä¬ÈÏÊ±¼ä2021-12-21 00:00:00*/
struct rtc_time systmtime=
{
	00,40,14,23,12,2021,0
};

//struct rtc_time set_time;
struct rtc_time set_time=
{
	00,30,15,30,03,2022,0
};

extern __IO uint8_t TimeDisplay ;


int main(void)
{	
	uint8_t i=0;
	uint8_t temp_address[6];
	System_Init();
//	Total_meters_num=3;



//				/*ÓÃ½ÓÊÕµ½µÄÊ±¼äÉèÖÃRTC*/
//				Time_Adjust(&set_time);
//				
//				//Ïò±¸·İ¼Ä´æÆ÷Ğ´Èë±êÖ¾
//				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	while(1)
  {
		
		  if (TimeDisplay == 1)
	    {
				/* µ±Ç°Ê±¼ä */
	      Time_Display( RTC_GetCounter(),&systmtime); 		  
	      TimeDisplay = 0;
				if(save_data_flash==1){
					dlt645_Automatic_data_acquisition(Automatic_data_acquisition,address);
					save_data_flash=0;
				}
				if(CCO==1){
					if(newTime==1){
						send_user_Data[0]=0x01;
						send_user_Data[1]=0x07;
						for(i=0;i<7;i++){
							send_user_Data[2+i]=Automatic_data_acquisition[29+i];
						}
						sc1667_send_data(sc1667_Address_Fully_Matched ,9, send_user_Data );
						newTime=0;
					}
				}
	    }
			if(sc1667_initindex==1){
				pull_data_from_queue();
				sc1667_send_farme( Ctrl_initiate ,Cmd_local_Setpassword  , 0, Default_password_empty );     //ÉèÖÃÄ¬ÈÏ¿ÚÁî
				SysTick_Delay_Ms(500);
				pull_data_from_queue();
				sc1667_send_farme( Ctrl_initiate ,Cmd_local_Setpassword  , 9, Default_password );     //ÉèÖÃÄ¬ÈÏ¿ÚÁî
				SysTick_Delay_Ms(500);
				pull_data_from_queue();
				sc1667_read_data( Ctrl_initiate , Cmd_local_Readmailingaddress, L_Empty, Data_Empty );
				SysTick_Delay_Ms(500);
				pull_data_from_queue();
				if(CCO==1){
					sc1667_send_farme( Ctrl_initiate ,Cmd_channel_InitiatingSearch , 13, search_data);   //ËÑË÷STA
					SysTick_Delay_Ms(500);
					pull_data_from_queue();
				}
				sc1667_initindex=2;
			}else if(sc1667_initindex==2){
				if(dlt645_Get_meter_address_data_Init(address,Address_Fully_Matched)==-1){
					LED_RED;
				}else{
					LED_GREEN;
				}
				if(CCO==1){
					send_user_Data[0]=0x02;
					send_user_Data[1]=0x00;
					sc1667_send_data(sc1667_Address_Fully_Matched ,2, send_user_Data );
				}
				sc1667_initindex=3;
			}else if(sc1667_initindex==3){
				if(CCO==0){
//					printf("\r\n \r\n \r\n \r\n \r\n here\r\n\r\n \r\n \r\n \r\n ");
					send_user_Data[0]=0x08;
					send_user_Data[1]=0x00;
					sc1667_send_data(sc1667_Address_Fully_Matched ,2, send_user_Data );
				}
				sc1667_initindex=4;
				TimeDisplay = 1;
			}else if(sc1667_initindex==4)
			{
				if(CCO==1){
					if(newSTA==1){
//						printf("\r\n \r\n \r\n \r\n \r\n here\r\n\r\n \r\n \r\n \r\n ");
						SysTick_Delay_Ms(500);
						pull_data_from_queue();
						send_user_Data[0]=0x02;
						send_user_Data[1]=0x00;
						for(i=0;i<6;i++){
							temp_address[i]=sc1667_search_address[Total_sc1667_num][i];
						}
						sc1667_send_data(temp_address,2, send_user_Data );
						newSTA=0;
						sc1667_initindex=0;
					}
				}else{
					sc1667_initindex=0;
				}
			}


//			dlt645_test();
			SC1667_test();
			pull_data_from_queue();
			pull_data_from_queue_debug();
			pull_data_from_queue_pi();
	}
}


void System_Init(void){
	int8_t flash_index=0;
	ILI9341_Init ();         //LCD ³õÊ¼»¯
	USART_Config();
	LED_GPIO_Config();	 
	LED_YELLOW;
//	ILI9341_Init ();         //LCD ³õÊ¼»¯
	
	/*³õÊ¼»¯HPLC USART ÅäÖÃÄ£Ê½Îª 115200 8-N-1£¬ÖĞ¶Ï½ÓÊÕ*/
	
	/*Ê¹ÄÜ»·ĞÎ¶ÓÁĞ*/
	rx_queue_init();
	rx_queue_debug_init();
	rx_queue_pi_init();
	RS485_Config();
	SPI_FLASH_Init();
	SysTick_Init();
	Key_GPIO_Config();
	/* ·¢ËÍÒ»¸ö×Ö·û´® */

//	RS485_SendString( "RS485_USART2_INIT\n");
//	Usart_SendString( DEBUG_USART3,"DEBUG_USART3_INIT\n");
//	Usart_SendString( HPLC_USART1,"HPLC_USART1_INIT\n");
	LCD_SetFont(&Font16x24);
	LCD_SetColors(RED,BLACK);

	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ÇåÆÁ£¬ÏÔÊ¾È«ºÚ */

	ILI9341_DispStringLine_EN(LINE(0),"      STA");
	LCD_SetFont(&Font8x16);
	
	RTC_NVIC_Config();

	RTC_CheckAndConfig(&systmtime);
	
	

	printf("\r FzuDq(config)# ÇëÊäÈëÏµÍ³Ê±¼ä¸ñÊ½ : time XXXX/XX/XX XX:XX:XX\n");
	LED_GREEN;
	return;
}

void SC1667_test(void){
	/*°´Ò»´Î°´¼ü·¢ËÍÒ»´ÎÊı¾İ*/
	if(	Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
	{

		LED_RED;
		
		ADVANCE_TIM_Init();
		
		send_user_Data[0]=User_Cmd_CCORequest_linedistance;
		send_user_Data[1]=0x00;
//		sc1667_send_data(sc1667_mailing_address,2, send_user_Data );
		sc1667_send_data(sc1667_search_address[0],2, send_user_Data );
		printf("distance_time=%d ms\n",distance_time);
//		sc1667_send_data(sc1667_search_address[0],2, send_user_Data );
		
//		sc1667_send_data(sc1667_cco_address ,6, sc1667_cco_address);
//		sc1667_remote_read_data( Cmd_remote_Readversion , sc1667_cco_address );
//		sc1667_remote_read_data( Cmd_remote_ReadMAC , sc1667_cco_address );
//		sc1667_remote_read_data( Cmd_remote_Readparameter, sc1667_cco_address );

//	sc1667_send_farme( Ctrl_initiate ,Cmd_channel_InitiatingSearch  , 13, search_data);   //ËÑË÷STA
//	sc1667_send_farme( Ctrl_initiate ,Cmd_local_Setpassword  , 9, Default_password );     //ÉèÖÃÄ¬ÈÏ¿ÚÁî
		
//		sc1667_read_data( Ctrl_initiate , Cmd_local_Readversion  , L_Empty, Data_Empty );
//		sc1667_read_data( Ctrl_initiate , Cmd_local_ReadMAC , L_Empty, Data_Empty );
//		sc1667_read_data( Ctrl_initiate , Cmd_local_Readpassword, L_Empty, Data_Empty );
//		sc1667_read_data( Ctrl_initiate , Cmd_local_Readmailingaddress, L_Empty, Data_Empty );
//		sc1667_read_data( Ctrl_initiate , Cmd_local_Readparameter, L_Empty, Data_Empty );


		LED_GREEN;
		
		printf("\r FzuDq(config)# SC1667·¢ËÍÊı¾İ³É¹¦£¡\n"); //Ê¹ÓÃµ÷ÊÔ´®¿Ú´òÓ¡µ÷ÊÔĞÅÏ¢µ½ÖÕ¶Ë

	}

}
void dlt645_test(void){
		uint8_t i,j;
		/*°´Ò»´Î°´¼ü·¢ËÍÒ	»´ÎÊı¾İ*/
		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
//			dlt645_Automatic_data_acquisition(Automatic_data_acquisition,address);
//			for(i=0;i<20;i++)
//			{
//				dlt645_read_flash_data (23,i,1);
//				dlt645_read_flash_data (23,i,2);
//				dlt645_read_flash_data (23,i,3);
//			}

//					dlt645_Get_meter_address_data_Init(address,Address_Fully_Matched);
			/*¼Ó¶ÌÔİÑÓÊ±£¬±£Ö¤485·¢ËÍÊı¾İÍê±Ï*/
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Combined_active_total_EE,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Forward_active_total_EE,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Reverse_active_total_EE,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_A_phase_voltage,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_A_phase_current,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_active_power,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_apparent_power,1);
//			RS485_Modified_baud_rate(9600);	
//			dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Total_power_factor,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_line_frequency,1);
//				dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Temperature_in_the_table,1);
//				
//				dlt645_MR_read_data(Address_Fully_Matched,C_MRF_ReadAddress,L_Empty,Data_Empty,0);
//				dlt645_MR_read_data(address,C_MRF_traffic_Rate,L_MRF_traffic_Rate,Data_MRF_traffic_Rate_1200,1);
//				dlt645_MR_read_data(Address_Fully_Matched,C_MRF_ReadAddress,L_Empty,Data_Empty,0);
//				  dlt645_Get_meter_data(address,2);
//				RS485_Modified_baud_rate(600);
//				dlt645_MR_read_data(address,C_MRF_traffic_Rate,L_MRF_traffic_Rate,Data_MRF_traffic_Rate_9600,2);
			
//				dlt645_MR_read_data(address,C_MRF_traffic_Rate,L_MRF_traffic_Rate,Data_MRF_traffic_Rate_4800,2);
//				printf("\r\n ×Üµç±íÊı %d \r\n",Total_meters_num);	
//				for(i=0;i<Total_meters_num;i++){
//					printf("µÚ%dÌ¨µç±íµØÖ· :  ",i+1);
//					for(j=0;j<6;j++){
//						printf("%02x ",address[i][j]);
//					}
//					printf("  ²¨ÌØÂÊ%d \n",address_Baud_rate[i]);
//				}
		}
		return;

}
/*********************************************END OF FILE**********************/
