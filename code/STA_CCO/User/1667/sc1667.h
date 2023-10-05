#ifndef _SC1667_H
#define _SC1667_H

#include "stm32f10x.h"
#include <stdio.h>

/*控制域Ctrl*/
#define Ctrl_14H_initiate                     0x40    
#define Ctrl_14H_normal_response              0x00
#define Ctrl_14H_abnormal_response            0x20

#define Ctrl_initiate                         0x00
#define Ctrl_normal_response                  0x00
#define Ctrl_abnormal_response                0x20

/*指令字列表Cmd*/
#define Cmd_local_restart                     0x01
#define Cmd_local_Readversion                 0x02
#define Cmd_local_ReadMAC                     0x03
#define Cmd_local_Readpassword                0x06
#define Cmd_local_Setpassword                 0x07
#define Cmd_local_Readmailingaddress          0x0B
#define Cmd_local_Setmailingaddress           0x0C
#define Cmd_local_Readparameter               0x0D
#define Cmd_local_Setparameter                0x0E
#define Cmd_local_enciphereddata              0x40
#define Cmd_local_Decryptdata                 0x41
#define Cmd_local_MD5_SUM                     0x42

#define Cmd_channel_senddata                  0x14
#define Cmd_channel_receivedata               0x15
#define Cmd_channel_InitiatingSearch          0x17
#define Cmd_channel_StopSearch                0x18
#define Cmd_channel_Reportresults             0x19
#define Cmd_channel_NotificationSearch        0x1A
#define Cmd_channel_ResponseSearch            0x1B
#define Cmd_channel_Initiatepasswordsetting   0x1C
#define Cmd_channel_Notifiespasswordsetting   0x1D
#define Cmd_channel_Reportsettingresults      0x1F

#define Cmd_remote_Readversion                0x52
#define Cmd_remote_ReadMAC                    0x53
#define Cmd_remote_Readparameter              0x5D


/*用户控制码*/

#define User_Cmd_unidied_time                 0x01
#define User_Cmd_CCORequest_meteraddress      0x02
#define User_Cmd_STAResponse_meteraddress     0x03
#define User_Cmd_CCORequest_meterdata         0x04
#define User_Cmd_STAResponse_meterdata        0x05
#define User_Cmd_CCORequest_meterflash        0x06
#define User_Cmd_STAResponse_meterflash       0x07
#define User_Cmd_STAResponse_ccoaddress       0x08
#define User_Cmd_CCORequest_timeaddress       0x09
#define User_Cmd_CCORequest_linedistance      0x10
#define User_Cmd_STAResponse_linedistance     0x11

/*数据域长度*/
//#define L_MRF1_ReadData 4
#define L_Empty  0
//#define L_MRF_traffic_Rate 1



/*空数据*/
#define Data_Empty  0x00

//#define  Data_MRF_traffic_Rate_19200  0x40
//#define  Data_MRF_traffic_Rate_9600   0x20
//#define  Data_MRF_traffic_Rate_4800   0x10
//#define  Data_MRF_traffic_Rate_2400   0x08
//#define  Data_MRF_traffic_Rate_1200   0x04
//#define  Data_MRF_traffic_Rate_600    0x02

/*debug*/

#define HPLC_DEBUG_ON          0
#define HPLC_DEBUG_ARRAY_ON    0
#define HPLC_DEBUG_FUNC_ON     0

#define HPLC_DEBUG_SEND(array, num)      do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(HPLC_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-HPLC-DEBUG-SEND->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)

 #define HPLC_DEBUG_ARRAY(array, num)    do{\
																				 int32_t i;\
																				 uint8_t* a = array;\
																				 if(HPLC_DEBUG_ARRAY_ON)\
																				 {\
																						printf("<<-HPLC-DEBUG-ARRAY->>\n");\
																						for (i = 0; i < (num); i++)\
																						{\
																								printf("%02x   ", (a)[i]);\
																								if ((i + 1 ) %10 == 0)\
																								{\
																										printf("\n");\
																								}\
																						}\
																						printf("\n");\
																				}\
																			 }while(0)




																			 
int8_t sc1667_read_data( uint8_t Ctrl,uint8_t Cmd , uint16_t L , uint64_t Data_64 );
uint32_t sc1667_send_farme(  uint8_t Ctrl,uint8_t Cmd , uint16_t L , uint8_t Data[]);			
int8_t sc1667_checkout(uint8_t *Recbuf);
int8_t sc1667_processing_data(uint8_t *Recbuf);
uint32_t sc1667_send_data(uint8_t address[] ,uint8_t User_Data_len, uint8_t user_Data[] );
int8_t sc1667_remote_read_data( uint8_t Cmd , uint8_t Data[] );									 
																			 
																			 
																			 
#endif																			 

																			 