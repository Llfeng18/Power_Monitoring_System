/*************************************************
 Copyright (c) 2019
 All rights reserved.
 File name:     dlt645.c
 Description:   
 History:
 1. Version:    
    Date:       2021-12-13
    Author:     llfeng
    Modify:     
*************************************************/
#include "dlt645.h"
#include "bsp_485.h"
#include "Delay.h"
#include "bsp_led.h"  
#include "bsp_usart.h"
#include <stdlib.h>
#include "bsp_ili9341_lcd.h"
#include "time.h"
#include <string.h>
#include "bsp_spi_flash.h"
#include "./usart/rx_data_queue.h"

extern __IO uint8_t address[6][6];
extern __IO uint8_t Address_Fully_Matched[2][6];
extern __IO uint8_t Total_meters_num ,Selected_meter;
extern __IO uint32_t address_Baud_rate[6];
extern __IO uint8_t Automatic_data_acquisition[42];
//extern __IO uint8_t Rx_Buffer[42];
extern uint32_t time_clock;

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/**
 * Name:    dlt645_send_farme
 * Brief:   645Э���װ����֡
 * Input:
 *  @temp_address[][6]:   ����ַ
 *  @C:                   ���Ʒ���
 *  @L:                   �����򳤶� 
 *  @Data[]:              ������
 *  @Selected_meter:      �����ĵ��
 * Output:  ʵ�ʷ��͵��ֽ���
 */

uint32_t dlt645_send_farme(uint8_t address[][6] , uint8_t C, uint8_t L , uint8_t Data[] , uint8_t Selected_meter)
{
    uint32_t CS = 0xD0;
    uint8_t OutBUffer [216];
    int32_t i;
    /*ǰ���ֽ�*/
    OutBUffer[0] = 0xFE;
    OutBUffer[1] = 0xFE;
    OutBUffer[2] = 0xFE;
    OutBUffer[3] = 0xFE;
    
    /*֡��ʼ��*/
    OutBUffer[4] = 0x68;

    /*��ַ��*/
    for( i = 0 ; i < 6 ; i++){
        OutBUffer[5+i]=address[Selected_meter][5-i];
        CS = CS + OutBUffer[5+i];
    }

    /*֡��ʼ��*/
    OutBUffer[11] = 0x68;
    
    /*������*/
    OutBUffer[12] = C;
    CS = CS + OutBUffer[12];

    /*����֡����*/
    OutBUffer[13] = L;
    CS = CS + OutBUffer[13];

    /*����*/
    for( i = L-1 ; i >= 0 ; i-- ){
        OutBUffer[13+L-i] = Data[i] +0x33;
        CS = CS + OutBUffer[13+L-i];
    }

    /*У����*/
    OutBUffer[14+L]=CS%256;

    /*������*/
    OutBUffer[15+L]=0x16;
		
		
		RS485_TX_EN();
		LED_YELLOW;
		
    for(i=0 ; i<=15+L ; i++){
			  RS485_SendByte(OutBUffer[i]);
    }
		
		Delay(0xFFFF);
		RS485_RX_EN();
		LED_GREEN;
//		printf("\r\n����%d���ݳɹ���\r\n",16+L); //ʹ�õ��Դ��ڴ�ӡ������Ϣ���ն�
    RS485_DEBUG_SEND((uint8_t*)OutBUffer,16+L);
		return 16+L;
}

/**
 * Name:    dlt645_MR_read_data
 * Brief:   645Э����վ������
 * Input:
 *  @address[]:   ����ַ
 *  @C:         ���Ʒ���
 *  @L:         �����򳤶� 
 *  @Data_32:    ������
 *  @Selected_meter:      �����ĵ��
 * Output:  ʵ�ʷ��͵��ֽ���
 * Output:  -1δ�յ��ַ���
 * Output:  -2�յ�֡Ч��ʧ��
 */
int8_t dlt645_MR_read_data(uint8_t address[][6] , uint8_t C, uint8_t L , uint32_t Data_32 , uint8_t Selected_meter)
{
	uint8_t *pbuf;
	uint16_t len , i;
	uint8_t Data[4];
	for( i=0 ; i<L ; i++ ){
		Data[i]=Data_32>>8*(L-i-1);
	}
//	Data[0]=Data_32>>24;
//	Data[1]=Data_32>>16;
//	Data[2]=Data_32>>8;
//	Data[3]=Data_32;
	dlt645_send_farme(address,C,L,Data,Selected_meter);
	srand(time_clock);
	SysTick_Delay_Ms((rand()%100 +100));
//	pull_data_from_queue();
//	LED_BLUE;	
	pbuf = get_rebuff(&len);
//	LED_GREEN;
//	printf("\r\n���յ�����Ϊ%d������\r\n",len);	
	RS485_DEBUG_ARRAY((uint8_t*)pbuf,len);
	SysTick_Delay_Ms(rand()%50 +100);
	if(len<3){
		if(len==0){
			printf("\r FzuDq(config)# ");
			printf("dlt645δ�յ�֡\n");	
		}else{
			printf("\r FzuDq(config)# ");
			printf("dlt645�յ�����С��3�ķǷ�֡\n");	
			PRINTF_ARRAY((uint8_t*)pbuf,len);
		}
			
		
		return -1;
	}
	else if(dlt645_checkout(pbuf)==-1){
		PRINTF_ARRAY((uint8_t*)pbuf,len);
		return -2;
	}	
	dlt645_processing_data(pbuf,len,Selected_meter);
	clean_rebuff();
	return 15+L+1;
}
/**
 * Name:    dlt645_checkout
 * Brief:   645Э�����֡Ч��(ֻУ���յ��ĵ�һ������֡0x16��ֹ)
 * Input:
 *  @Recbuf:   ���յ�������
 * Output:  ʵ���յ���һ������֡���ֽ���
 * Output:  -1֡Ч��ʧ��
 */
int8_t dlt645_checkout(uint8_t *Recbuf){
	uint32_t i=0,CS=0x00;
	uint32_t delay_time=0;
	while(((Recbuf)[i]==0xfe)||((Recbuf)[i+1]==0xfe)||((Recbuf)[i+2]==0xfe)||((Recbuf)[i+3]==0xfe)){
		i++;
	}
	for(;i<11||Recbuf[i]!=0x16;i++){
		CS=CS+Recbuf[i];
	}
	CS=CS-Recbuf[i-1];
	if(CS%256==Recbuf[i-1]){
//		printf("\r\n CS Ч����ȷ\r\n");
		return i+1;
	}else{
		printf("\r FzuDq(config)# ");
		printf("CS dlt645Ч��ʧ��,���ݶ���\n");

		clean_rebuff();
		return -1;
	}
	
	
}
/**
 * Name:    dlt645_processing_data
 * Brief:   645Э�鴦������
 * Input:
 *  @Recbuf:   ���յ�������
 *  @len:         �����򳤶� 
 *  @Selected_meter:      �����ĵ��
 * Output:   1����֡��������
 * Output:  -1����֡���ͳ���
 * Output:  -2���ݿ������쳣
 */
int8_t dlt645_processing_data(uint8_t *Recbuf,uint8_t len, uint8_t Selected_meter){
	uint32_t i=0,j,Sent_Data=0x00,Data,Integer,Decimals,C ;
	while(((Recbuf)[i]==0xfe)||((Recbuf)[i+1]==0xfe)||((Recbuf)[i+2]==0xfe)||((Recbuf)[i+3]==0xfe)){
		i++;
	}
	C=Recbuf[8+i];
	printf("\r FzuDq(config)# ");
	if(C==0xD1){
		printf("����֡���ͳ���C=0xD1\n");
		return -1;
	}
	else if(C==0x93){
//		printf("��ַΪ\n ");
		for( j = 0 ; j < 6 ; j++){
			address[Selected_meter][j]=Recbuf[i+6-j];
//			printf("%02x ",address[Selected_meter][j]);
		}
//		printf("\n");
	}
	else if(C==0x91){
		Sent_Data=((Recbuf[13+i]-0x33)<<24)+((Recbuf[12+i]-0x33)<<16)+((Recbuf[11+i]-0x33)<<8)+(Recbuf[10+i]-0x33);
//		printf("\r\n ��������֡���ͳɹ�����֡%08x C=%02x\r\n",Sent_Data,C);
    switch(Sent_Data)
        {
        case Data_MRF1_Combined_active_total_EE :
                        Data=((Recbuf[17+i]-0x33)<<24)+((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>8;
                        Decimals=Data&0xFF;
//                        printf("����й��ܵ��� %x.%x kWh\n" ,Integer,Decimals);
												Automatic_data_acquisition[0]=Recbuf[17+i]-0x33;
												Automatic_data_acquisition[1]=Recbuf[16+i]-0x33;
												Automatic_data_acquisition[2]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[3]=Recbuf[14+i]-0x33;
                        break;
        case Data_MRF1_Forward_active_total_EE :
                        Data=((Recbuf[17+i]-0x33)<<24)+((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>8;
                        Decimals=Data&0xFF;
												Automatic_data_acquisition[4]=Recbuf[17+i]-0x33;
												Automatic_data_acquisition[5]=Recbuf[16+i]-0x33;
												Automatic_data_acquisition[6]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[7]=Recbuf[14+i]-0x33;
//                        printf("�����й��ܵ��� %x.%x kWh\n" ,Integer,Decimals);
                        break;
        case Data_MRF1_Reverse_active_total_EE :
                        Data=((Recbuf[17+i]-0x33)<<24)+((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>8;
                        Decimals=Data&0xFF;
												Automatic_data_acquisition[8]=Recbuf[17+i]-0x33;
												Automatic_data_acquisition[9]=Recbuf[16+i]-0x33;
												Automatic_data_acquisition[10]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[11]=Recbuf[14+i]-0x33;
//                        printf("�����й��ܵ��� %x.%x kWh\n" ,Integer,Decimals);
                        break;
        case Data_MRF1_A_phase_voltage :
                        Data=((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>4;
                        Decimals=Data&0x0F;
//                        printf("A���ѹ %x.%x V\n" ,Integer,Decimals);
											
												Automatic_data_acquisition[12]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[13]=Recbuf[14+i]-0x33;
                        break;
        case Data_MRF1_A_phase_current :
                        Data=((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>12;
                        Decimals=Data&0xFFF;
//                        printf("A����� %x.%x A\n" ,Integer,Decimals);
												Automatic_data_acquisition[14]=Recbuf[16+i]-0x33;
												Automatic_data_acquisition[15]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[16]=Recbuf[14+i]-0x33;
                        break;
        case Data_MRF1_Instantaneous_total_active_power :
                        Data=((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>16;
                        Decimals=Data&0xFFFF;
//                        printf("˲ʱ���й����� %x.%x kW\n" ,Integer,Decimals);
												Automatic_data_acquisition[17]=Recbuf[16+i]-0x33;
												Automatic_data_acquisition[18]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[19]=Recbuf[14+i]-0x33;
                        break;
//        case Data_MRF1_Instantaneous_total_reactive_power :
//                        Data=((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
//                        Integer=Data>>16;
//                        Decimals=Data&0xFFFF;
//                        printf("˲ʱ���޹����� %x.%x kvar\n" ,Integer,Decimals);
//                        break;
        case Data_MRF1_Instantaneous_total_apparent_power :
                        Data=((Recbuf[16+i]-0x33)<<16)+((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>16;
                        Decimals=Data&0xFFFF;
//                        printf("˲ʱ�����ڹ��� %x.%x kVA\n" ,Integer,Decimals);
												Automatic_data_acquisition[20]=Recbuf[16+i]-0x33;
												Automatic_data_acquisition[21]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[22]=Recbuf[14+i]-0x33;
                        break;
        case Data_MRF1_Total_power_factor :
                        Data=((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>12;
                        Decimals=Data&0xFFF;
//                        printf("�ܹ������� %x.%x \n" ,Integer,Decimals);
												Automatic_data_acquisition[23]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[24]=Recbuf[14+i]-0x33;
                        break;
				
//        case Data_MRF1_A_phase_Angle :
//                        Data=((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
//                        Integer=Data>>4;
//                        Decimals=Data&0xF;
//                        printf("A��� %x.%x ��\n" ,Integer,Decimals);
//                        break;   
        case Data_MRF1_line_frequency :
                        Data=((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>8;
                        Decimals=Data&0xFF;
//                        printf("��λ %x.%x Hz\n" ,Integer,Decimals);
												Automatic_data_acquisition[25]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[26]=Recbuf[14+i]-0x33;
                        break;   
        case Data_MRF1_Temperature_in_the_table  :
                        Data=((Recbuf[15+i]-0x33)<<8)+(Recbuf[14+i]-0x33);
                        Integer=Data>>4;
                        Decimals=Data&0xF;
//                        printf("�¶� %x.%x ��\n" ,Integer,Decimals);
                        Automatic_data_acquisition[27]=Recbuf[15+i]-0x33;
												Automatic_data_acquisition[28]=Recbuf[14+i]-0x33;
                        break;  
        default :
                        printf("��ƥ��\n" );
        }
			}else if(C==0x97){
				Sent_Data=Recbuf[10+i]-0x33;
				switch(Sent_Data)
					{
					case Data_MRF_traffic_Rate_19200 :
//						printf("������ʳɹ��޸�Ϊ19200\n" );
						address_Baud_rate[Selected_meter]=19200;
						break;
					case Data_MRF_traffic_Rate_9600 :
//						printf("������ʳɹ��޸�Ϊ9600\n" );
						address_Baud_rate[Selected_meter]=9600;
						break;
					case Data_MRF_traffic_Rate_4800 :
//						printf("������ʳɹ��޸�Ϊ4800\n" );
						address_Baud_rate[Selected_meter]=4800;
						break;
					case Data_MRF_traffic_Rate_2400 :
//						printf("������ʳɹ��޸�Ϊ2400\n" );
						address_Baud_rate[Selected_meter]=2400;
						break;
					case Data_MRF_traffic_Rate_1200 :
//						printf("������ʳɹ��޸�Ϊ1200\n" );
						address_Baud_rate[Selected_meter]=1200;
						break;
					case Data_MRF_traffic_Rate_600 :
//						printf("������ʳɹ��޸�Ϊ600\n" );
						address_Baud_rate[Selected_meter]=600;
						break;
					default :
//						printf("��ƥ��ɹ��޸ĵĲ�����\n" );
				}
						
			}else{
				printf("�쳣�Ŀ�����C=%02x,���󷵻�\n",C);
				for(j=i;j<10||Recbuf[j]!=0x16;j++){
					printf(" %x",Recbuf[j]);
				}
				printf(" %x\n",Recbuf[j]);
				return -2;
			}
	return 1;
}

/**
 * Name:    dlt645_Get_meter_data
 * Brief:   645Э���ȡĳ����ַ�Ĳ�����
 * Input:
 *  @address:   ��ַ��
 *  @Selected_meter:      �����ĵ��
 * Output:   1��ȡ���õ�ַ�������
 * Output:  -1δ��ȡ���õ�ַ�������
 */

int8_t dlt645_Get_meter_data(uint8_t address[][6], uint8_t Selected_meter){
	uint32_t Baud_rate[6]={19200,9600,4800,2400,1200,600},i;
	uint8_t Data;
	for(i=0;i<6;i++){
		RS485_Modified_baud_rate(Baud_rate[i]);
		if(dlt645_MR_read_data(address,C_MRF_ReadAddress,L_Empty,Data_Empty,Selected_meter)!=-1){
//			printf("�������Ϊ%d\n", Baud_rate[i]);
			return 1;
		}else{
//			printf("������ʲ�Ϊ%d\n", Baud_rate[i]);
		}
	}
	return -1;
}
/**
 * Name:    dlt645_Get_meter_address_data_Init
 * Brief:   645Э���Զ���ȡģ�������ӵĵ�����������ǵĵ�ַ
 * Input:
 *  @address:   ��ַ��
 *  @Address_Fully_Matched:         ȫƥ���ַ�� 
 * Output:   1����֡��������
 * Output:  -1�Զ���ȡ��ַ�б�ʧ�� 
 */
int8_t dlt645_Get_meter_address_data_Init(uint8_t address[][6],uint8_t Address_Fully_Matched[][6]){
	int Baud_rate[5]={19200,4800,2400,1200,600};
	uint16_t rate[5]={0x40,0x10, 0x08,0x04,0x02};
	int8_t index=0,circulation=0,Rate_index=-1,Rate_circulation=0,index_Rate=1;
	uint8_t i,j,k,l=0;
	uint8_t str[30]; // �ַ����ݴ� 
	Total_meters_num=1;
	RS485_Modified_baud_rate(9600);
	printf("\r FzuDq(config)# ");
	printf("��ʼ����Ӧ��ȡģ�������ӵĵ���ַ\n ");	
	while(index!=-1){
		index=dlt645_MR_read_data(Address_Fully_Matched,C_MRF_ReadAddress,L_Empty,Data_Empty,0);
		if(index>0){
			circulation=0;
//			printf("\r\n ����� %d ������%d\r\n ",index_Rate,rate[index_Rate]);	
			Rate_index=dlt645_MR_read_data(address,C_MRF_traffic_Rate,L_MRF_traffic_Rate,rate[index_Rate],0);
//			index_Rate=index_Rate%3+1;
		}else if(index==-2){
			circulation=circulation+1;
			srand(time_clock);
//			SysTick_Delay_Ms((rand()%100+100));
			if(circulation>20){
				printf("\r FzuDq(config)# ");
				printf("�Զ���ȡ��ַ�б�ʧ�� 9600 �����ʶδ��ڶ������ͻ �볢�Ը�λ\n ");	
				LCD_ERROR_RESET(Baud_rate[i]);
				return -1;
			}
		}
	}
	for(i=0;i<5;i++){
		index=0;
		circulation=0;
//		Rate_index=-1;
//		Rate_circulation=0;
		RS485_Modified_baud_rate(Baud_rate[i]);
		while(index!=-1){
				index=dlt645_MR_read_data(Address_Fully_Matched,C_MRF_ReadAddress,L_Empty,Data_Empty,0);
				if(index>0){
					circulation=0;
//					Rate_index=-1;
//					Rate_circulation=0;
					Rate_index=dlt645_MR_read_data(address,C_MRF_traffic_Rate,L_MRF_traffic_Rate,Data_MRF_traffic_Rate_9600,0);
					l=0;
					for(k=1;(k<Total_meters_num)&&(l!=6);k++){
						for(l=0;l<6;l++){
							if(address[k][l]!=address[0][l]){
								break;
							}
						}
					}
					if(l!=6)
					{
						for(j=0;j<6;j++){
							address[Total_meters_num][j]=address[0][j];
						}
						Total_meters_num=Total_meters_num+1;
					}
				}else if(index==-2){
					circulation=circulation+1;
//					srand(time_clock);
//					SysTick_Delay_Ms((rand()%50+50));
					if(circulation>20){
						printf("\r FzuDq(config)# ");
						printf("\r\n �Զ���ȡ��ַ�б�ʧ�� %d �����ʶδ��ڶ������ͻ �볢�Ը�λ\r\n ",Baud_rate[i]);	
						LCD_ERROR_RESET(Baud_rate[i]);
						return -1;
				}
			}
		}
	}
	RS485_Modified_baud_rate(9600);
	Total_meters_num=Total_meters_num-1;
	printf("\r FzuDq(config)# ");
	printf("�ܵ���� %d \n",Total_meters_num);	
	sprintf((char *)str," Total_meters_num: %d ",Total_meters_num);
	ILI9341_DispStringLine_EN(LINE(6),(char*)str);
	for(i=0;i<Total_meters_num;i++){
	
		printf("\r                ��%d̨����ַ :  ",i+1);
		for(j=0;j<6;j++){
			printf("%02x ",address[i][j]);
		}
		sprintf((char *)str," meters %d address :",i+1);
		ILI9341_DispStringLine_EN(LINE(7+4*i),(char*)str);
		sprintf((char *)str,"  %02x %02x %02x %02x %02x %02x",address[i][0],address[i][1],address[i][2],address[i][3],address[i][4],address[i][5]);
		ILI9341_DispStringLine_EN(LINE(8+4*i),(char*)str);
		printf("  ������%d \n",address_Baud_rate[i]);
		sprintf((char *)str," Baud rate : %d",address_Baud_rate[i]);
		ILI9341_DispStringLine_EN(LINE(9+4*i),(char*)str);
	}
	return 1;
}

int8_t dlt645_Automatic_data_acquisition(uint8_t Automatic_data_acquisition[42],uint8_t address[][6]){
	int i,j;
//	uint64_t write_address=0x00;
	for(i=1;i<=Total_meters_num;i++){
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Combined_active_total_EE,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Forward_active_total_EE,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Reverse_active_total_EE,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_A_phase_voltage,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_A_phase_current,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_active_power,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_apparent_power,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_active_power,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Total_power_factor,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_line_frequency,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Temperature_in_the_table,i);
		for( j = 0 ; j < 6 ; j++){
			Automatic_data_acquisition[j+36]=address[i][j];
		}
//		dlt645_show_flash_index(Automatic_data_acquisition);
	
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// �������4K����һ����������������С��λ������
//		if((Automatic_data_acquisition[33]==0)&&(Automatic_data_acquisition[32]%2==1)){
		
		if((Automatic_data_acquisition[32]%2==1)&&(Automatic_data_acquisition[31]==0)){
			SPI_FLASH_SectorErase(0x00000+((Automatic_data_acquisition[32]-1)/2)*0x1000+(i-1)*0x10000);	
			printf("\r FzuDq(config)# ");
			printf("���ڣ�%d  �������� %x \n",Automatic_data_acquisition[32],((0x00000+((Automatic_data_acquisition[32]-1)/2)*0x1000+(i-1)*0x10000)));
		}
		printf("\r FzuDq(config)# ");
		if(Automatic_data_acquisition[32]%2==1){
			SPI_FLASH_BufferWrite(Automatic_data_acquisition, 0x00000+((Automatic_data_acquisition[32]-1)/2)*0x1000+Automatic_data_acquisition[33]*0x55+(i-1)*0x10000, 42);	
			printf("д������  %d�� %dʱ ��ַ%x��\n",Automatic_data_acquisition[32],Automatic_data_acquisition[33],0x00000+((Automatic_data_acquisition[32]-1)/2)*0x1000+Automatic_data_acquisition[33]*0x55+(i-1)*0x10000);
		}else
		{
			SPI_FLASH_BufferWrite(Automatic_data_acquisition, 0x00000+((Automatic_data_acquisition[32]-1)/2)*0x1000+Automatic_data_acquisition[33]*0x55+0x800+(i-1)*0x10000, 42);	
			printf("д������  %d�� %dʱ ��ַ%x��\n",Automatic_data_acquisition[32],Automatic_data_acquisition[33],0x00000+((Automatic_data_acquisition[32]-1)/2)*0x1000+Automatic_data_acquisition[33]*0x55+0x800+(i-1)*0x10000);
		}
		/* �����ͻ�����������д��flash�� */
		// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
		printf("\r FzuDq(config)# ");
		printf("д�������Ϊ��\n");
		dlt645_show_flash_index(Automatic_data_acquisition);
		}
		memset(Automatic_data_acquisition,-1, 42); 
//		dlt645_read_flash_data (Automatic_data_acquisition[32] , Automatic_data_acquisition[33]);
		return 0;
}

int8_t dlt645_data_acquisition(uint8_t Automatic_data_acquisition[42],uint8_t address[][6],uint8_t i){
		int j;
//	uint64_t write_address=0x00;

		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Combined_active_total_EE,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Forward_active_total_EE,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Reverse_active_total_EE,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_A_phase_voltage,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_A_phase_current,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_active_power,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_apparent_power,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Instantaneous_total_active_power,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Total_power_factor,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_line_frequency,i);
		dlt645_MR_read_data(address,C_MRF_ReadData,L_MRF1_ReadData,Data_MRF1_Temperature_in_the_table,i);
		for( j = 0 ; j < 6 ; j++){
			Automatic_data_acquisition[j+36]=address[i][j];
		}
	}

void dlt645_show_flash_index(uint8_t Automatic_data_acquisition[42]){
	uint32_t Data,Integer,Decimals;
	printf("\r FzuDq(config)# ");
	printf("show_flash_index \n");
	
	Data=(Automatic_data_acquisition[0]<<24)+(Automatic_data_acquisition[1]<<16)+(Automatic_data_acquisition[2]<<8)+(Automatic_data_acquisition[3]);
	Integer=Data>>8;
	Decimals=Data&0xFF;
	printf("\r                ����й��ܵ��� %x.%x kWh\n" ,Integer,Decimals);

	Data=(Automatic_data_acquisition[4]<<24)+(Automatic_data_acquisition[5]<<16)+(Automatic_data_acquisition[6]<<8)+(Automatic_data_acquisition[7]);
	Integer=Data>>8;
	Decimals=Data&0xFF;
	printf("\r                �����й��ܵ��� %x.%x kWh\n" ,Integer,Decimals);
	
	Data=(Automatic_data_acquisition[8]<<24)+(Automatic_data_acquisition[9]<<16)+(Automatic_data_acquisition[10]<<8)+(Automatic_data_acquisition[11]);
	Integer=Data>>8;
	Decimals=Data&0xFF;
	printf("\r                �����й��ܵ��� %x.%x kWh\n" ,Integer,Decimals);
	
	Data=(Automatic_data_acquisition[12]<<8)+(Automatic_data_acquisition[13]);
	Integer=Data>>4;
	Decimals=Data&0x0F;
	printf("\r                A���ѹ %x.%x V\n" ,Integer,Decimals);

	Data=(Automatic_data_acquisition[14]<<16)+(Automatic_data_acquisition[15]<<8)+(Automatic_data_acquisition[16]);
	Integer=Data>>12;
	Decimals=Data&0xFFF;
	printf("\r                A����� %x.%x A\n" ,Integer,Decimals);

	Data=(Automatic_data_acquisition[17]<<16)+(Automatic_data_acquisition[18]<<8)+(Automatic_data_acquisition[19]);
	Integer=Data>>16;
	Decimals=Data&0xFFFF;
	printf("\r                ˲ʱ���й����� %x.%x kW\n" ,Integer,Decimals);
												
	Data=(Automatic_data_acquisition[20]<<16)+(Automatic_data_acquisition[21]<<8)+(Automatic_data_acquisition[22]);
	Integer=Data>>16;
	Decimals=Data&0xFFFF;
	printf("\r                ˲ʱ�����ڹ��� %x.%x kVA\n" ,Integer,Decimals);

	Data=((Automatic_data_acquisition[23]<<8)+(Automatic_data_acquisition[24]));
	Integer=Data>>12;
	Decimals=Data&0xFFF;
	printf("\r                �ܹ������� %x.%x \n" ,Integer,Decimals);

	Data=((Automatic_data_acquisition[25]<<8)+(Automatic_data_acquisition[26]));	
	Integer=Data>>8;
	Decimals=Data&0xFF;
	printf("\r                ��λ %x.%x Hz\n" ,Integer,Decimals);

	Data=((Automatic_data_acquisition[27]<<8)+(Automatic_data_acquisition[28]));	
	Integer=Data>>4;
	Decimals=Data&0xF;
	printf("\r                �¶� %x.%x ��\n" ,Integer,Decimals);

	printf(" \r                %d%d��%d��%d�� %0.2d:%0.2d:%0.2d\n",Automatic_data_acquisition[29],Automatic_data_acquisition[30],Automatic_data_acquisition[31],Automatic_data_acquisition[32],Automatic_data_acquisition[33],Automatic_data_acquisition[34],Automatic_data_acquisition[35]);

  printf(" \r                ����ַ: %0.2x %0.2x %0.2x %0.2x %0.2x %0.2x\n",Automatic_data_acquisition[36],Automatic_data_acquisition[37],Automatic_data_acquisition[38],Automatic_data_acquisition[39],Automatic_data_acquisition[40],Automatic_data_acquisition[41]);

	return;
}

void dlt645_read_flash_data (uint8_t day , uint8_t hour,uint8_t j){
	uint8_t Rx_Buffer[42],i;
	/* ���ո�д������ݶ������ŵ����ջ������� */
	printf("\r FzuDq(config)# ");
	if(day%2==1){
		SPI_FLASH_BufferRead(Rx_Buffer, 0x00000+((day-1)/2)*0x1000+hour*0x55+(j-1)*0x10000, 42);	
		printf("��������  %d�� %dʱ ��ַ%x��\n",day,hour,0x00000+((day-1)/2)*0x1000+hour*0x55+(j-1)*0x10000);
	}else
	{
		SPI_FLASH_BufferRead(Rx_Buffer, 0x00000+((day-1)/2)*0x1000+hour*0x55+0x800+(j-1)*0x10000, 42);	
		printf("��������  %d�� %dʱ ��ַ%x��\n",day,hour,0x00000+((day-1)/2)*0x1000+hour*0x55+0x800+(j-1)*0x10000);
	}
	dlt645_show_flash_index(Rx_Buffer);
	return;
}

