/*************************************************
 Copyright (c) 2019
 All rights reserved.
 File name:     sc1667.c
 Description:   
 History:
 1. Version:    
    Date:       2021-12-23
    Author:     llfeng
    Modify:     
*************************************************/
#include "sc1667.h"
#include "dlt645.h"
#include "Delay.h"
#include "bsp_led.h"  
#include "bsp_usart.h"
#include <stdlib.h>
#include "time.h"
#include "stm32f10x.h"
#include <stdarg.h>
#include "./usart/rx_data_queue.h"
#include "./rtc/bsp_rtc.h"
#include "bsp_date.h"
#include "bsp_AdvanceTim.h" 

extern uint32_t time_clock;
extern uint8_t sc1667_mailing_address[6];
extern uint8_t Default_password[9];
extern uint8_t sc1667_search_address[6][6];
extern uint8_t Total_sc1667_num,Selected_sc1667;
extern uint8_t rec_user_data[200];
extern struct rtc_time set_time;
extern uint8_t ShowTime,newTime;
extern uint8_t STA_meter_address[3][37];
extern uint8_t Total_meters_num;
extern uint8_t address[6][6];
extern __IO uint8_t Automatic_data_acquisition[42];
extern uint8_t sc1667_cco_address[6];
extern __IO uint8_t newSTA;
extern volatile uint32_t distance_time;
extern volatile uint32_t distance_index;
extern uint8_t sc1667_Address_Fully_Matched[6];
extern int pirequest;
uint32_t sc1667_send_farme(  uint8_t Ctrl,uint8_t Cmd , uint16_t L , uint8_t Data[])
{
    uint8_t OutBUffer [216];
    int32_t i;
		uint8_t CSUM=0x00, CXOR=0x00;
    
    /*֡��ʼ��*/
    OutBUffer[0] = 0x79;

	  /*����֡����L*/
		OutBUffer[1] = L;
    OutBUffer[2] = L>>8;

	  /*������Ctrl*/
		OutBUffer[3] = Ctrl;
	
	  /*ָ�����б�Cmd*/
		OutBUffer[4] = Cmd;
		
	  /*����Data*/
    for( i = L-1 ; i >= 0 ; i-- ){
			OutBUffer[4+L-i] = Data[i];
    }
		
		for(i=1;i<=4+L;i++){
			CSUM=CSUM+OutBUffer[i];
			CXOR=CXOR^OutBUffer[i];
		}
		/*��У��CSUM*/
		OutBUffer[5+L]=CSUM;

		/*���У��CXOR*/
		OutBUffer[6+L]=CXOR;		
//		printf("\r here6\n");
		LED_YELLOW;
//		printf("\r here7\n");
    for(i=0 ; i<=6+L ; i++){
//				printf("\r i=%d\n",i);
			  HPlC_SendByte(OutBUffer[i]);
    }
//		printf("\r here8\n");
		LED_GREEN;
//		printf("\r\nHPLC����%d���ݳɹ���\r\n",7+L); //ʹ�õ��Դ��ڴ�ӡ������Ϣ���ն�
		switch(Cmd)
			{
				case Cmd_local_Setpassword   :
					{
						if(OutBUffer[5]<0x08){
							printf("\r FzuDq(config)# ����û��������\n");
						}else{
							printf("\r FzuDq(config)# ");
							printf("�����û���������Ϊ %d : ",OutBUffer[5]);
							for(i=6;i<6+OutBUffer[5];i++){
								printf("%c",OutBUffer[i]);
							}
							printf("\n");
							break;
						}
					}
			}
		
    HPLC_DEBUG_SEND((uint8_t*)OutBUffer,7+L);
		return 6+L;
}


uint32_t sc1667_send_data(uint8_t address[] ,uint8_t User_Data_len, uint8_t user_Data[] )
{
    uint8_t OutBUffer [216];
    int32_t i,len ;
		uint8_t CSUM=0x00, CXOR=0x00;
	
    len=10+User_Data_len;
	
    /*֡��ʼ��*/
    OutBUffer[0] = 0x79;

	  /*����֡����L*/
		OutBUffer[1] = len;
    OutBUffer[2] = len>>8;
	
	  /*������Ctrl*/
		OutBUffer[3] = 0x40;
	
	  /*ָ�����б�Cmd*/
		OutBUffer[4] = 0x14;

	  /*����Data*/
		/*Data Ctrl*/
		OutBUffer[5] = 0x33;
		OutBUffer[6] = 0xF8;
		
		/*Data Addr*/
		for(i=0;i<6;i++){
			OutBUffer[7+i] = address[5-i];
		}
		
	  /*User Data LEN*/
		OutBUffer[13] = User_Data_len;
    OutBUffer[14] = User_Data_len>>8;
		
    for( i = 0 ; i < User_Data_len ; i++ ){
			OutBUffer[15+i] = user_Data[User_Data_len-i-1];
    }
		
		for(i=1;i<=len+4;i++){
			CSUM=CSUM+OutBUffer[i];
			CXOR=CXOR^OutBUffer[i];
		}
		/*��У��CSUM*/
		OutBUffer[15+User_Data_len]=CSUM;

		/*���У��CXOR*/
		OutBUffer[16+User_Data_len]=CXOR;		
		
		LED_YELLOW;
		
    for(i=0 ; i<=16+User_Data_len ; i++){
			  HPlC_SendByte(OutBUffer[i]);
    }
		
		LED_GREEN;
		if(debug_index==1){
			printf("HPLC����%d���ݳɹ�!\n",17+User_Data_len); //ʹ�õ��Դ��ڴ�ӡ������Ϣ���ն�

			PRINTF_ARRAY((uint8_t*)OutBUffer,17+User_Data_len);
		}

		return 17+User_Data_len;
}


int8_t sc1667_remote_read_data( uint8_t Cmd , uint8_t Data[] )
{
	uint16_t i;
	uint8_t user_Data[7];
	for( i=0 ; i<6; i++ ){
		user_Data[i]=Data[i];
	}
	user_Data[6]=0x00;
	sc1667_send_farme(Ctrl_initiate,Cmd,7,user_Data);
	return 1;
}

int8_t sc1667_read_data( uint8_t Ctrl,uint8_t Cmd , uint16_t L , uint64_t Data_64 )
{
	uint16_t len , i;
	uint8_t Data[4];
	for( i=0 ; i<L ; i++ ){
		Data[i]=Data_64>>8*(L-i-1);
	}

	sc1667_send_farme(Ctrl,Cmd,L,Data);
	return 6+L;
}


int8_t sc1667_checkout(uint8_t *Recbuf){
	uint8_t CSUM=0x00, CXOR=0x00;
	uint32_t i;
	uint16_t len;
	len=Recbuf[2]*100+Recbuf[1];
//	printf("len=%d Recbuf[2]=%d Recbuf[1]=%d\n",len,Recbuf[2],Recbuf[1]);
	for(i=1;i<=len+4;i++){
		CSUM=CSUM+Recbuf[i];
		CXOR=CXOR^Recbuf[i];
	}
//	printf("CSUM=%x CXOR=%x \n",CSUM,CXOR);
	if((CSUM==Recbuf[len+5])&&(CXOR==Recbuf[len+6])){
//		printf("\r FzuDq(config)# ");
//		printf("CSUM��CXOR Ч����ȷ\n");
		return len+7;
	}else{
		printf("\r FzuDq(config)# ");
		printf("Sc1667Ч��ʧ��,���ݶ���\n");
		
		return -1;
	}
	
}

int8_t sc1667_processing_data(uint8_t *Recbuf){
	
	uint32_t i=0,j,k,L;
	uint8_t Cmd;
	uint16_t len;
	uint8_t Data[200];
	uint8_t Rx_Buffer[42];
	uint8_t temp_address[6];
  len=((Recbuf[2]>>4)*16+(Recbuf[2]&0x0F))*100+(Recbuf[1]>>4)*16+(Recbuf[1]&0x0F);
//	printf("len=%d\n",len);
	Cmd=Recbuf[4];
	printf("\r FzuDq(config)# ");
	switch(Cmd)
			{
			case	Cmd_remote_Readversion:
			{
				printf("��ѯģ��ͨѶ��ַ: %x %x %x %x %x %x \n",Recbuf[10],Recbuf[9],Recbuf[8],Recbuf[7],Recbuf[6],Recbuf[5]);
				i=6;
				printf("\r                ");
			}
			case Cmd_local_Readversion :
				{
					printf("ģ��汾��Ϣ : \n");
					printf("\r                ���̱�ʶ ES-%x%x \n",Recbuf[6+i],Recbuf[5+i]);
					printf("\r                оƬ���� %x%x \n",Recbuf[8+i],Recbuf[7+i]);
					printf("\r                ��Ʒ��Ϣ %x \n",Recbuf[9+i]);
					printf("\r                �汾�� %x-%x \n",Recbuf[11+i],Recbuf[10+i]);
					break;
				}
			case	Cmd_remote_ReadMAC:
			{
				printf("��ѯģ��ͨѶ��ַ: %02x %02x %02x %02x %02x %02x \n",Recbuf[10],Recbuf[9],Recbuf[8],Recbuf[7],Recbuf[6],Recbuf[5]);
				i=6;
				printf("\r                ");
			}
			case Cmd_local_ReadMAC :
				{
					printf("ģ��MAC��ַ: %02x %02x %02x %02x %02x %02x \n",Recbuf[10+i],Recbuf[9+i],Recbuf[8+i],Recbuf[7+i],Recbuf[6+i],Recbuf[5+i]);
					break;
				}
			case Cmd_local_Readpassword  :
				{
					if(Recbuf[5]==0){
						printf("δ�����û����� \n");
					}else{
						printf("�û���������Ϊ %d : ",Recbuf[5]);
						for(i=0;i<Recbuf[5];i++){
							printf("%c",Recbuf[6+i]);
						}
						printf("\n");					
					}
					break;
				}
			case Cmd_local_Readmailingaddress  :
				{
					printf("ģ��ͨ�ŵ�ַ: %02x %02x %02x %02x %02x %02x \n",Recbuf[10],Recbuf[9],Recbuf[8],Recbuf[7],Recbuf[6],Recbuf[5]);
					for(i=0;i<6;i++){
						sc1667_mailing_address[i]=Recbuf[10-i];
					}
					break;
				}
			case	Cmd_remote_Readparameter:
			{
				
				printf("��ѯģ��ͨѶ��ַ: %02x %02x %02x %02x %02x %02x \n",Recbuf[10],Recbuf[9],Recbuf[8],Recbuf[7],Recbuf[6],Recbuf[5]);
				i=6;
				printf("\r                ");
			}
			case Cmd_local_Readparameter  :
				{
					if((Recbuf[5+i]&0xF0)==0x00){
						printf("�豸��ָ��͸���㼶����\n");
					}else{
						printf("�豸͸���㼶%x \n",Recbuf[5+i]&&0xF0);
					}
					
					if((Recbuf[6+i]&(0x03))==0x00){
						printf("\r                1λֹͣλ\n");
					}else if((Recbuf[6+i]&&(0x03))==0x03){
						printf("\r                2λֹͣλ\n");
					}
					
					if((Recbuf[6+i]&(0xC))==0x00){
						printf("\r                ��У��\n");
					}else if((Recbuf[6+i]&0xC)==0x04){
						printf("\r                żУ��\n");
					}else{
						printf("\r                ��У��\n");
					}
					
					if((Recbuf[6+i]&0xF0)==0){
						printf("\r                ������ 600 \n");
					}else if((Recbuf[6+i]&0xF0)==0x10){
						printf("\r                ������ 1200 \n");
					}else if((Recbuf[6+i]&0xF0)==0x20){
						printf("\r                ������ 2400 \n");
					}else if((Recbuf[6+i]&0xF0)==0x30){
						printf("\r                ������ 4800 \n");
					}else if((Recbuf[6+i]&0xF0)==0x40){
						printf("\r                ������ 9600 \n");
					}else if((Recbuf[6+i]&0xF0)==0x50){
						printf("\r                ������ 14400 \n");
					}else if((Recbuf[6+i]&0xF0)==0x60){
						printf("\r                ������ 19200 \n");
					}else if((Recbuf[6+i]&0xF0)==0x70){
						printf("\r                ������ 38400 \n");
					}else if((Recbuf[6+i]&0xF0)==0x80){
						printf("\r                ������ 56000 \n");
					}else if((Recbuf[6+i]&0xF0)==0x90){
						printf("\r                ������ 57600 \n");
					}else if((Recbuf[6+i]&0xF0)==0xa0){
						printf("\r                ������ 115200 \n");
					}
					
					if((Recbuf[7+i]&0x0F)==0x00){
						printf("\r                131���ز�\n");
					}else if((Recbuf[7+i]&0x0F)==0x01){
						printf("\r                411���ز�\n");
					}
					
					printf("\r                ���͹���%d��\n",(Recbuf[7+i]&0xF0)>>4);

					break;
			}
			case Cmd_channel_NotificationSearch  :
			{
				if(len>10){
//					sc1667_read_data( Ctrl_14H_normal_response ,Cmd_channel_NotificationSearch, L_Empty, Data_Empty );
					for(i=0;i<len;i++){
	//					Data[i]=Recbuf[len+4-i];
						Data[i]=Recbuf[4+len-i];
					}
					Data[len-2]=0xff;
					sc1667_send_farme(Ctrl_14H_normal_response ,Cmd_channel_ResponseSearch,len,Data);	
					printf("�յ��豸 %02x %02x %02x %02x %02x %02x������\n" ,Recbuf[12],Recbuf[11],Recbuf[10],Recbuf[9],Recbuf[8],Recbuf[7]);
					for(i=0;i<6;i++){
						sc1667_cco_address[i]=Recbuf[12-i];
					}
					break;
				}
			}
			case Cmd_channel_ResponseSearch  :
			{
				printf("��Ӧ�豸�����ɹ�\n" );
				break;
			}
			case Cmd_local_Setpassword   :
			{
				printf("����ģ���û��������ɹ�\n" );
				break;
			}
			case Cmd_channel_InitiatingSearch:
			{
				printf("�����豸����\n");
				break;
			}
			case Cmd_channel_senddata:
			{
				printf("�������ݳɹ�\n");
				break;
			}
			case Cmd_channel_receivedata :
			{
				len=((Recbuf[15]>>4)*16+(Recbuf[15]&0x0F))*100+(Recbuf[14]>>4)*16+(Recbuf[14]&0x0F);
				printf("���յ�ͨ�ŵ�ַΪ: %02x %02x %02x %02x %02x %02x ��%d�ֽ�����:\n",Recbuf[13],Recbuf[12],Recbuf[11],Recbuf[10],Recbuf[9],Recbuf[8],len);
				printf("\r                ");
				for(i=0;i<len;i++){
					printf("%02x ",Recbuf[15+len-i]);
				}
				printf("\n");
//				printf("Recbuf[24]=%x\n",Recbuf[24]);
				switch(Recbuf[15+len])
				{
					case	User_Cmd_CCORequest_timeaddress:
					{
						for(i=0;i<6;i++){
							sc1667_cco_address[i]=Recbuf[13-i];
						}
					}
					case	User_Cmd_unidied_time:
					{
						set_time.tm_year = Recbuf[22]*100+Recbuf[21];
						set_time.tm_mon = Recbuf[20];
						set_time.tm_mday = Recbuf[19];
						set_time.tm_hour = Recbuf[18];
						set_time.tm_min = Recbuf[17];
						set_time.tm_sec = Recbuf[16];
						/*�ý��յ���ʱ������RTC*/
						Time_Adjust(&set_time);
						
						//�򱸷ݼĴ���д���־
						BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
						ShowTime=1;
						printf("\r FzuDq(config)# ");
						break;
					}
					case	User_Cmd_CCORequest_meteraddress:
					{
						for(i=0;i<6;i++){
							temp_address[i]=Recbuf[13-i];
						}
						Data[0]=User_Cmd_STAResponse_meteraddress;
						Data[1]=6*Total_meters_num+2;
						for(i=0;i<Total_meters_num;i++){
							for(j=0;j<6;j++){
								Data[6*i+j+2]=address[i][j];
							}
						}
						sc1667_send_data(temp_address ,6*Total_meters_num+2, Data );
						break;
					}
					case	User_Cmd_STAResponse_ccoaddress:
					{
						for(i=0;i<6;i++){
							temp_address[i]=Recbuf[13-i];
						}
						for(i=1;i<=Total_sc1667_num&&j!=6;i++){
							for(j=0;j<6;j++){
								if(temp_address[j]!=sc1667_search_address[i][j]){
									break;
								}
							}
						}
						if(j!=6){
							for(i=0;i<6;i++){
								sc1667_search_address[Total_sc1667_num+1][i]=temp_address[i];
							}
							Total_sc1667_num=Total_sc1667_num+1;
							newSTA=1;
//							Data[0]=0x02;
//							Data[1]=0x00;
//							sc1667_send_data(temp_address,2, Data );
//							SysTick_Delay_Ms(500);
//							pull_data_from_queue();
						}
						Data[0]=0x09;
						Data[1]=0x07;
						for(i=0;i<7;i++){
							Data[2+i]=Automatic_data_acquisition[29+i];
						}
						sc1667_send_data(temp_address ,9, Data );
						break;
					}
					case	User_Cmd_STAResponse_meteraddress:
					{
						for(i=1;i<=Total_sc1667_num&&j!=6;i++){
							for(j=0;j<6;j++){
								if(Recbuf[13-j]!=sc1667_search_address[i][j]){
									break;
								}
							}
						}
						if(j==6){
							L=i-1;
						}else{
							Total_sc1667_num=Total_sc1667_num+1;
							L=Total_sc1667_num;
						}
//						printf("\r\n\r\n\r\nL=%d\r\n\r\n\r\n",L);
						for(i=0;i<6;i++){
							STA_meter_address[L][i]=Recbuf[13-i];
						}
						STA_meter_address[L][6]=(Recbuf[14+len]-2)/6;
						for(i=0;i<6*Recbuf[14+len];i++){
							STA_meter_address[L][7+i]=Recbuf[13+len-i];
						}
						printf("\r FzuDq(config)# STAͨ�ŵ�ַΪ: %02x %02x %02x %02x %02x %02x ��%d�����:\n",STA_meter_address[L][0],STA_meter_address[L][1],STA_meter_address[L][2],STA_meter_address[L][3],STA_meter_address[L][4],STA_meter_address[L][5],STA_meter_address[L][6]);
						for(j=0;j<STA_meter_address[L][6];j++){
											printf("\r                ��%d̨����ַ : ",j+1);
											for(i=0;i<6;i++){
												printf(" %02x",STA_meter_address[L][7+i+6*j]);
											}
											printf("\n");
										}
						break;
					}
					case	User_Cmd_CCORequest_meterdata:
					{
							dlt645_data_acquisition(Automatic_data_acquisition,address,Recbuf[13+len]);
							Data[0]=0x05;
							Data[1]=42;				
							for(i=0;i<42;i++){
								Data[i+2]=Automatic_data_acquisition[i];
							}
							sc1667_send_data(sc1667_cco_address,44, Data );
							break;
					}
					case	User_Cmd_STAResponse_meterflash :
					case	User_Cmd_STAResponse_meterdata :
					{
							for(i=0;i<42;i++){
								Automatic_data_acquisition[i]=Recbuf[13+len-i];
							}
							
							if(pirequest==1){
								Data[0]=Cmd_Pi_Readmeterdata;
								Data[1]=42;
								for(i=0;i<42;i++){
									Data[2+i]=Automatic_data_acquisition[i];
								}
								Usart_SendArray(Pi_USART4,Data,44);
								pirequest=0;
							}else{
								dlt645_show_flash_index(Automatic_data_acquisition);
							}
							break;
					}
					case	User_Cmd_CCORequest_linedistance :
					{
							Data[0]=User_Cmd_STAResponse_linedistance;
							Data[1]=0x00;
							sc1667_send_data(sc1667_Address_Fully_Matched ,2, Data );
							break;
					}
					case	User_Cmd_STAResponse_linedistance :
					{
						  if(distance_time!=0){
								
//								printf("distance_index=%d distance_time=%d ms\n",distance_index,distance_time);
								if(distance_index==1000){
									printf("distance_index=%d distance_time=%d ms\n",distance_index,distance_time);
									TIM_Cmd(ADVANCE_TIM, DISABLE);
									distance_time=0;
									distance_index=0;
								}else{
										Data[0]=User_Cmd_CCORequest_linedistance;
										Data[1]=0x00;
										sc1667_send_data(sc1667_search_address[0],2, Data );
										distance_index++;
								}
							}
							break;
					}
					case	User_Cmd_CCORequest_meterflash :
					{
							if(Recbuf[13+len]%2==1){
								SPI_FLASH_BufferRead(Rx_Buffer, 0x00000+((Recbuf[13+len]-1)/2)*0x1000+Recbuf[12+len]*0x55+(Recbuf[11+len]-1)*0x10000, 42);	
							}else
							{
								SPI_FLASH_BufferRead(Rx_Buffer, 0x00000+((Recbuf[13+len]-1)/2)*0x1000+Recbuf[12+len]*0x55+0x800+(Recbuf[11+len]-1)*0x10000, 42);	
							}
							Data[0]=0x07;
							Data[1]=42;				
							for(i=0;i<42;i++){
								Data[i+2]=Rx_Buffer[i];
							}
							sc1667_send_data(sc1667_cco_address,44, Data );
							break;
					}
					default :
					{
							printf("�û�������ƥ��\n" );
					}
			}
			break;
		}
			case Cmd_channel_Reportresults :
			{
				L=0;
				for(j=0;j<Recbuf[5];j++){
					for(i=0;i<6;i++){
						sc1667_search_address[0][i]=Recbuf[11-i+7*j+L];
					}
					i=0;
					for(k=1;(i!=6)&&(k<=Total_sc1667_num);k++){
						for(;i<6;i++){
							if(sc1667_search_address[0][i]!=sc1667_search_address[k][i]){
								break;
							}
						}
					}
					if(i!=6){
						Total_sc1667_num=Total_sc1667_num+1;
						for(i=0;i<6;i++){
							sc1667_search_address[Total_sc1667_num][i]=sc1667_search_address[0][i];
						}
					}
					L=L+Recbuf[6+9*(j+1)+L];
				}
				printf("��STA������%d̨\n",Total_sc1667_num);
				for(j=1;j<=Total_sc1667_num;j++){
					printf("\r                ��%d̨STA��ַ : ",j);
					for(i=0;i<6;i++){
						printf("%02x ",sc1667_search_address[j][i]);
					}
					printf("\n");
				}
				break;
			}
			
			default :
					printf("��ƥ��\n" );
			}


	return 1;
}

