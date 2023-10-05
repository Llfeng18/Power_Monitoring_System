#ifndef _DLT645_H
#define _DLT645_H

#include <stdint.h>
/* MRF Master request frame */
#include "./rtc/bsp_rtc.h"

/*������*/
#define C_MRF_ReadData 0x11
#define C_MRF_ReadAddress 0x13
#define C_MRF_traffic_Rate 0x17

/*�����򳤶�*/
#define L_MRF1_ReadData 4
#define L_Empty  0
#define L_MRF_traffic_Rate 1

/*��ȡ���ݸ�ʽXXXXXX.XX ���ݳ��ȣ��ֽڣ�4 ��λkWh*/
#define Data_MRF1_Combined_active_total_EE 0x00000000
#define Data_MRF1_Forward_active_total_EE  0x00010000
#define Data_MRF1_Reverse_active_total_EE 0x00020000

/*��ȡ���ݸ�ʽXXX.X ���ݳ��ȣ��ֽڣ�2 ��λV*/
#define Data_MRF1_A_phase_voltage 0x02010100

/*��ȡ���ݸ�ʽXXX.XXX ���ݳ��ȣ��ֽڣ�3 ��λA*/
#define Data_MRF1_A_phase_current 0x02020100

/*��ȡ���ݸ�ʽXX.XXXX ���ݳ��ȣ��ֽڣ�3 ��λkW*/
#define Data_MRF1_Instantaneous_total_active_power 0x02030000

///*��ȡ���ݸ�ʽXX.XXXX ���ݳ��ȣ��ֽڣ�3 ��λkvar*/
//#define Data_MRF1_Instantaneous_total_reactive_power 0x02040000

/*��ȡ���ݸ�ʽXX.XXXX ���ݳ��ȣ��ֽڣ�3 ��λkVA*/
#define Data_MRF1_Instantaneous_total_apparent_power 0x02050000

/*��ȡ���ݸ�ʽX.XXX ���ݳ��ȣ��ֽڣ�2 ��λ */
#define Data_MRF1_Total_power_factor 0x02060000

/*��ȡ���ݸ�ʽXXX.X ���ݳ��ȣ��ֽڣ�2 ��λ�� */
//#define Data_MRF1_A_phase_Angle  0x02070100

/*��ȡ���ݸ�ʽXX.XX ���ݳ��ȣ��ֽڣ�2  ��λHz */
#define Data_MRF1_line_frequency  0x02800002

/*��ȡ���ݸ�ʽXXX.X ���ݳ��ȣ��ֽڣ�2  ��λ��*/
#define Data_MRF1_Temperature_in_the_table  0x02800007

/*��ȡ/д�����ݸ�ʽNNNNNNNNNNNN ���ݳ��ȣ��ֽڣ�6  ��λ*/
#define Data_RAWF1_Mailing_Address  0x04000401
#define Data_RAWF1_table_number     0x04000402

/*������*/
#define Data_Empty  0x00

#define  Data_MRF_traffic_Rate_19200  0x40
#define  Data_MRF_traffic_Rate_9600   0x20
#define  Data_MRF_traffic_Rate_4800   0x10
#define  Data_MRF_traffic_Rate_2400   0x08
#define  Data_MRF_traffic_Rate_1200   0x04
#define  Data_MRF_traffic_Rate_600    0x02


uint32_t dlt645_send_farme(uint8_t address[][6] , uint8_t C, uint8_t L , uint8_t Data[] , uint8_t Selected_meter);
int8_t dlt645_MR_read_data(uint8_t address[][6] , uint8_t C, uint8_t L , uint32_t Data_32 , uint8_t Selected_meter);
int8_t dlt645_checkout(uint8_t *Recbuf);
int8_t dlt645_processing_data(uint8_t *Recbuf,uint8_t len, uint8_t Selected_meter);
int8_t dlt645_Get_meter_data(uint8_t address[][6], uint8_t Selected_meter);
int8_t dlt645_Get_meter_address_data_Init(uint8_t address[][6],uint8_t Address_Fully_Matched[][6]);
int8_t dlt645_Automatic_data_acquisition(uint8_t Automatic_data_acquisition[42],uint8_t address[][6]);
//uint8_t address[6]={ 0x19 , 0x12 , 0x00 , 0x10 , 0x30 , 0x76 };
//uint8_t C = 0x11 , L= 4 , Data[4] ={ 0x00 , 0x01 , 0x00 , 0x00};
void dlt645_show_flash_index(uint8_t Automatic_data_acquisition[42]);
void dlt645_read_flash_data (uint8_t day , uint8_t hour,uint8_t j);
#endif
