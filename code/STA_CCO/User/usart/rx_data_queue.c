/**
  ******************************************************************************
  * @file    rx_data_queue.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   ���λ������������ڽ����ⲿ����ʱ��������
  ******************************************************************************
  */ 

#include "./usart/rx_data_queue.h"
#include "time.h"
#include "sc1667.h"
#include "./rtc/bsp_rtc.h"
#include "bsp_date.h"
#include "bsp_usart.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
//ʵ�����ڵ���������
QUEUE_DATA_TYPE  node_data[QUEUE_NODE_NUM]; 
QUEUE_DATA_TYPE  node_debug_data[QUEUE_NODE_NUM]; 
QUEUE_DATA_TYPE  node_pi_data[QUEUE_NODE_NUM]; 

//ʵ������������
QueueBuffer rx_queue_pi;
QueueBuffer rx_queue;
QueueBuffer rx_queue_debug;


//���л��������ڴ��
__align(4) char node_buff[QUEUE_NODE_NUM][QUEUE_NODE_DATA_LEN] ;
__align(4) char node_debug_buff[QUEUE_NODE_NUM][QUEUE_NODE_DATA_LEN] ;
__align(4) char node_pi_buff[QUEUE_NODE_NUM][QUEUE_NODE_DATA_LEN] ;

extern uint8_t Receive_Flag;
extern struct rtc_time set_time;
extern uint8_t ShowTime,newTime;
extern __IO uint8_t sc1667_initindex;
extern uint8_t sc1667_cco_address[6];
extern uint8_t sc1667_search_address[6][6];
extern uint8_t Total_sc1667_num,Selected_sc1667;
extern __IO uint8_t address[6][6];
extern __IO uint8_t Address_Fully_Matched[2][6];
extern __IO uint8_t Total_meters_num ,Selected_meter;
extern __IO uint32_t address_Baud_rate[6];
extern __IO uint8_t Automatic_data_acquisition[42];
extern __IO uint8_t CCO;
extern __IO uint8_t debug_index;
extern __IO uint8_t STA_meter_address[3][37];
extern __IO uint8_t Total_meters_num;
extern uint8_t sc1667_mailing_address[6];
int pirequest=0;
/*���λ������*/

/**
  * @brief  ��ʼ���������
  * @param  cb:������нṹ��
  * @param  size: ������е�Ԫ�ظ���
  * @note 	��ʼ��ʱ����Ҫ��cb->elemsָ�븳ֵ
  */
void cbInit(QueueBuffer *cb, int size) 
{
    cb->size  = size;	/* maximum number of elements           */
    cb->read = 0; 		/* index of oldest element              */
    cb->write   = 0; 	 	/* index at which to write new element  */
//    cb->elems = (uint8_t *)calloc(cb->size, sizeof(uint8_t));  //elems Ҫ�����ʼ��
}
 
//(�˺����ĳ��˺꣬��ͷ�ļ�)
/**
  * @brief  ���������е�ǰ��״̬��Ϣ
  * @param  cb:������нṹ��
  */
//void cbPrint(QueueBuffer *cb) 
//{
//    DATA_QUEUE_LOG("size=0x%x, read=%d, write=%d\n", cb->size, cb->read, cb->write);
//	  DATA_QUEUE_LOG("size=0x%x, read_using=%d, write_using=%d\n", cb->size, cb->read_using, cb->write_using);
//}
 
/**
  * @brief  �жϻ��������(1)��(0)����
  * @param  cb:������нṹ��
  */
int cbIsFull(QueueBuffer *cb) 
{
    return cb->write == (cb->read ^ cb->size); /* This inverts the most significant bit of read before comparison */ 
}
 
/**
  * @brief  �жϻ��������(1)��(0)ȫ��
  * @param  cb:������нṹ��
  */		
int cbIsEmpty(QueueBuffer *cb) 
{
    return cb->write == cb->read; 
}

/**
  * @brief  �Ի�����е�ָ���1
  * @param  cb:������нṹ��
  * @param  p��Ҫ��1��ָ��
  * @return  ���ؼ�1�Ľ��
  */	
int cbIncr(QueueBuffer *cb, int p) 
{
    return (p + 1)&(2*cb->size-1); /* read and write pointers incrementation is done modulo 2*size */
}
 
/**
  * @brief  ��ȡ��д��Ļ�����ָ��
  * @param  cb:������нṹ��
  * @return  �ɽ���д��Ļ�����ָ��
  * @note  �õ�ָ���ɽ���д���������дָ�벻��������1��
           д������ʱ��Ӧ����cbWriteFinish��дָ���1
  */
QUEUE_DATA_TYPE* cbWrite(QueueBuffer *cb) 
{
    if (cbIsFull(cb)) /* full, overwrite moves read pointer */
    {
			return NULL;
		}		
		else
		{
			//��wriet��write_using���ʱ����ʾ��һ����������д����ϣ���Ҫ��дָ���1
			if(cb->write == cb->write_using)
			{
				cb->write_using = cbIncr(cb, cb->write); //δ����������1
			}
		}
		
	return  cb->elems[cb->write_using&(cb->size-1)];
}



/**
  * @brief ����д����ϣ�����дָ�뵽����ṹ��
  * @param  cb:������нṹ��
  */
void cbWriteFinish(QueueBuffer *cb)
{
    cb->write = cb->write_using;
}
 
/**
  * @brief  ��ȡ�ɶ�ȡ�Ļ�����ָ��
  * @param  cb:������нṹ��
  * @return  �ɽ��ж�ȡ�Ļ�����ָ��
  * @note  �õ�ָ���ɽ����ȡ����������ָ�벻��������1��
					 ��ȡ������ʱ��Ӧ����cbReadFinish�Զ�ָ���1
  */
QUEUE_DATA_TYPE* cbRead(QueueBuffer *cb) 
{
		if(cbIsEmpty(cb))
			return NULL;
		
	//��read��read_using���ʱ����ʾ��һ���������Ѷ�ȡ���(���ѵ���cbReadFinish)��
	//��Ҫ��дָ���1
	if(cb->read == cb->read_using)	
		cb->read_using = cbIncr(cb, cb->read);
	
	return cb->elems[cb->read_using&(cb->size-1)];
}


/**
  * @brief ���ݶ�ȡ��ϣ����¶�ָ�뵽����ṹ��
  * @param  cb:������нṹ��
  */
void cbReadFinish(QueueBuffer *cb) 
{	
		//���õ�ǰ��������ݽڵ�ĳ���
		cb->elems[cb->read_using&(cb->size-1)]->len = 0;
	
    cb->read = cb->read_using;
}



//���е�ָ��ָ��Ļ�����ȫ������
void camera_queue_free(void)
{
    uint32_t i = 0;

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        if(node_data[i].head != NULL)
        {
					//���Ƕ�̬����Ŀռ��Ҫfree
//            free(node_data[i].head);
            node_data[i].head = NULL;
        }
    }

    return;
}


/**
  * @brief  ������г�ʼ���������ڴ�,ʹ�û������ʱ��
  * @param  ��
  * @retval ��
  */
void rx_queue_init(void)
{
  uint32_t i = 0;

  memset(node_data, 0, sizeof(node_data));
		 
	/*��ʼ���������*/
	cbInit(&rx_queue,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_data[i].head = node_buff[i];
        
        /*��ʼ�����л���ָ�룬ָ��ʵ�ʵ��ڴ�*/
        rx_queue.elems[i] = &node_data[i];
        
        DATA_QUEUE_LOG("node_data[i].head=0x%x,\r\nrx_queue.elems[i] =0x%x", (uint32_t)node_data[i].head,(uint32_t)rx_queue.elems[i]->head);

        memset(node_data[i].head, 0, QUEUE_NODE_DATA_LEN);
    }
		
	cbPrint(rx_queue);	
	printf("\r                HPLC_Queue_Config\n");
}


void rx_queue_debug_init(void)
{
  uint32_t i = 0;

  memset(node_debug_data, 0, sizeof(node_debug_data));
		 
	/*��ʼ���������*/
	cbInit(&rx_queue_debug,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_debug_data[i].head = node_debug_buff[i];
        
        /*��ʼ�����л���ָ�룬ָ��ʵ�ʵ��ڴ�*/
        rx_queue_debug.elems[i] = &node_debug_data[i];
        
        DATA_QUEUE_LOG("node_debug_data[i].head=0x%x,\r\nrx_queue_debug.elems[i] =0x%x", (uint32_t)node_debug_data[i].head,(uint32_t)rx_queue_debug.elems[i]->head);

        memset(node_debug_data[i].head, 0, QUEUE_NODE_DATA_LEN);
    }
		
	cbPrint(rx_queue_debug);	
			printf("\r                Debug_Queue_Config\n");
}


void rx_queue_pi_init(void)
{
  uint32_t i = 0;

  memset(node_pi_data, 0, sizeof(node_pi_data));
		 
	/*��ʼ���������*/
	cbInit(&rx_queue_pi,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_pi_data[i].head = node_pi_buff[i];
        
        /*��ʼ�����л���ָ�룬ָ��ʵ�ʵ��ڴ�*/
        rx_queue_pi.elems[i] = &node_pi_data[i];
        
        DATA_QUEUE_LOG("node_pi_data[i].head=0x%x,\r\nrx_queue_pi.elems[i] =0x%x", (uint32_t)node_pi_data[i].head,(uint32_t)rx_queue_pi.elems[i]->head);

        memset(node_pi_data[i].head, 0, QUEUE_NODE_DATA_LEN);
    }
		
	cbPrint(rx_queue_pi);	
			printf("\r                Pi_Queue_Config\n");
}


/**
  * @brief  ��������д�����ݵ�����
  */
void push_data_to_queue(char *src_dat,uint16_t src_len)
{
	QUEUE_DATA_TYPE *data_p;
	uint8_t i;
	
	for(i=0;i<src_len;i++)
	{
		/*��ȡд������ָ�룬׼��д��������*/
		data_p = cbWrite(&rx_queue);
		
		if (data_p != NULL)	//���������δ������ʼ����
		{		
			//��������д�����ݣ���ʹ�ô��ڽ��ա�dmaд��ȷ�ʽ
			*(data_p->head + i) = src_dat[i];
				data_p->len++;
			printf("\r\ndata_p->len =%d",data_p->len);
		}else return;	
		
		cbPrint(rx_queue);	
	}	
	
	/*д�뻺�������*/
	cbWriteFinish(&rx_queue);
	
	cbPrint(rx_queue);	

}


/**
  * @brief  �Ӷ�����ȡ���ݵ�����
  */
int8_t pull_data_from_queue(void)
{
	QUEUE_DATA_TYPE *rx_data;	
		
	/*�ӻ�������ȡ���ݣ����д���*/
	rx_data = cbRead(&rx_queue); 

	if(rx_data != NULL)//������зǿ�
	{		
		//�����ַ���������������ֱ������ַ���
		*(rx_data->head+rx_data->len) = '\0';
//		printf("\r FzuDq(config)# ");
//		printf("sc1667���յ�����Ϊ%d������\r\n",rx_data->len);	
//		QUEUE_DEBUG("���յ������ݣ�%s",rx_data->head);
		QUEUE_DEBUG_ARRAY((uint8_t*)rx_data->head,rx_data->len);
		if(rx_data->len<3){
			printf("\r FzuDq(config)# ");
			printf("sc1667���յ��Ƿ�֡���� \n");	
			PRINTF_ARRAY(rx_data->head, rx_data->len);
			cbReadFinish(&rx_queue);
			return -1;
		}
		else if(sc1667_checkout(rx_data->head)==-1){
			if(rx_data->head[0]==0xFE&&rx_data->head[1]==0xFE&&rx_data->head[2]==0xFE&&rx_data->head[3]==0xFE&&rx_data->head[4]==0x68){
				sc1667_initindex=1;
			}else{
				PRINTF_ARRAY(rx_data->head, rx_data->len);
			}
			cbReadFinish(&rx_queue);
			return -2;
		}	
		sc1667_processing_data(rx_data->head);
		//ʹ�������ݱ������cbReadFinish���¶�ָ��
		cbReadFinish(&rx_queue);
//		printf("\r FzuDq(config)# ");
	}
}


int8_t pull_data_from_queue_debug(void)
{
//	if(Receive_Flag==1){
		QUEUE_DATA_TYPE *rx_debug_data;	
			
		/*�ӻ�������ȡ���ݣ����д���*/
		rx_debug_data = cbRead(&rx_queue_debug); 

		if(rx_debug_data != NULL)//������зǿ�
		{		
			//�����ַ���������������ֱ������ַ���
			*(rx_debug_data->head+rx_debug_data->len) = '\0';
			
			QUEUE_DEBUG("���յ������ݣ�%s",rx_debug_data->head);
			QUEUE_DEBUG_ARRAY((uint8_t*)rx_debug_data->head,rx_debug_data->len);
			
			debug_processing_data(rx_debug_data->head);
			//ʹ�������ݱ������cbReadFinish���¶�ָ��
			cbReadFinish(&rx_queue_debug);
			printf("\r FzuDq(config)# ");
		}


//	}
}

int8_t debug_processing_data(uint8_t *Recbuf){
	uint8_t temp[20],i,j=0,k,l,user_Data[20];
	uint32_t temp_num ;
	for(i=0;i<20;i++){
		temp[i]=Recbuf[i];
		if(temp[i]==' '||temp[i]=='\r'||temp[i]=='\n'){
			break;
		}
	}
	temp[i]='\0';
//	printf("i=%d\n",i);
	/*time 2021/12/25 21:38:00*/
	if(strcmp(temp,"time") == 0){
//		printf("temp=%d\n",Recbuf[5]);
		
		set_time.tm_year = (Recbuf[5]-48)*1000+(Recbuf[6]-48)*100+(Recbuf[7]-48)*10+Recbuf[8]-48;
		set_time.tm_mon = (Recbuf[10]-48)*10+Recbuf[11]-48;
		set_time.tm_mday = (Recbuf[13]-48)*10+Recbuf[14]-48;
		set_time.tm_hour = (Recbuf[16]-48)*10+Recbuf[17]-48;
		set_time.tm_min = (Recbuf[19]-48)*10+Recbuf[20]-48;
		set_time.tm_sec = (Recbuf[22]-48)*10+Recbuf[23]-48;
		
		/*�ý��յ���ʱ������RTC*/
		Time_Adjust(&set_time);
		
		//�򱸷ݼĴ���д���־
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
//		printf("here\n");
		ShowTime=1;
		newTime=1;
	}else if(strcmp(temp,"debug") == 0){
			if(debug_index==0){
				debug_index=1;
				printf("\r FzuDq(config)# debugģʽ����\n");
			}else{
				debug_index=0;
				printf("\r FzuDq(config)# debugģʽ�ر�\n");
			}
	}else if(strcmp(temp,"STA/CCO") == 0){
			if(CCO==1){
				CCO=0;
				printf("\r FzuDq(config)# STAģʽ����\n");
			}else{
				CCO=1;
				printf("\r FzuDq(config)# CCOģʽ����\n");
			}
	}else if(strcmp(temp,"show") == 0){
//		printf("here\n");
		memset(temp,-1, 20); 
		j=i+1;
		for(i=0;i<20;i++){
			temp[i]=Recbuf[i+j];
			if(temp[i]==' '||temp[i]=='\r'||temp[i]=='\n'){
				break;
				}
		}
		temp[i]='\0';
//		printf("temp=%s i=%d\n",temp,i);
		if(strcmp(temp,"command") == 0){
			printf("\r FzuDq(config)# ����������ʾ\n");
			printf("\r                show time                               �鿴��ǰ�豸ʱ��\n");
			printf("\r                show mailaddress                        �鿴��ǰ�豸HPLCͨѶ��ַ\n");
			printf("\r                show version XX XX XX XX XX XX          �鿴ͨѶ��ַΪXX XX XX XX XX XX���豸HPLC�汾��Ϣ\n");
			printf("\r                show MAC XX XX XX XX XX XX              �鿴ͨѶ��ַΪXX XX XX XX XX XX���豸HPLC MAC��ַ\n");
			printf("\r                show password                           �鿴��ǰ�豸HPLC�������\n");
			printf("\r                show parameter XX XX XX XX XX XX        �鿴ͨѶ��ַΪXX XX XX XX XX XX���豸HPLC�������\n");
			printf("\r                show CCO                                �鿴��ǰ�豸CCOͨѶ��ַ(�������STA�豸��Ч)\n");
			printf("\r                show STA                                �鿴��ǰ�豸STAͨѶ��ַ(�������CCO�豸��Ч)\n");
			printf("\r                show meter                              �鿴�����豸�����ӵ���ַ\n");
			printf("\r                show data XX XX XX XX XX XX             �鿴��ַΪXX XX XX XX XX XX�ĵ�����������\n");
			printf("\r                show flash XX XX XX XX XX XX XX XX XX   �鿴ͨѶ��ַΪXX XX XX XX XX XX���豸XX��XXʱ��XX��������������\n");
			printf("\r                time XXXX/XX/XX XX:XX:XX                �޸ĵ�ǰ�豸ʱ���豸ʱ��\n");
			printf("\r                debug                                   ��/�رյ���ģʽ\n");
			printf("\r                STA/CCO                                 STA��CCOģʽ�л�\n");
		}else if(strcmp(temp,"time") == 0){
			ShowTime=1;
		}else if(strcmp(temp,"mailaddress") == 0){
			sc1667_read_data( Ctrl_initiate , Cmd_local_Readmailingaddress, L_Empty, Data_Empty );
		}else if(strcmp(temp,"version") == 0){
			j=j+i+1;
			for(k=0;k<6;k++){
				if(Recbuf[j+k]>96){
					Recbuf[j+k]=Recbuf[j+k]-87;
				}else{
					Recbuf[j+k]=Recbuf[j+k]-48;
				}
				if(Recbuf[j+k+1]>96){
					Recbuf[j+k+1]=Recbuf[j+k+1]-87;
				}else{
					Recbuf[j+k+1]=Recbuf[j+k+1]-48;
				}
				temp[k]=Recbuf[j+k]*16+Recbuf[j+k+1];
				j=j+2;
			}
			k=0;
			for(k=0;k<6;k++){
				if(sc1667_mailing_address[k]!=temp[k]){
					break;
				}
			}
			if(k==6){
				sc1667_read_data( Ctrl_initiate , Cmd_local_Readversion  , L_Empty, Data_Empty );
			}else{
				i=0;
				for(j=0;(i!=6)&&(j<Total_sc1667_num);j++){
						for(i=0;i<6;i++){
								if(sc1667_search_address[j][i]!=temp[i]){
									break;
								}
						}
					}
				if(i==6){
					sc1667_remote_read_data( Cmd_remote_Readversion , temp );
				}else{
					printf("\r                ��ƥ���ͨѶ��ַ,����\n");
				}
			}
		}else if(strcmp(temp,"MAC") == 0){
			j=j+i+1;
			for(k=0;k<6;k++){
				if(Recbuf[j+k]>96){
					Recbuf[j+k]=Recbuf[j+k]-87;
				}else{
					Recbuf[j+k]=Recbuf[j+k]-48;
				}
				if(Recbuf[j+k+1]>96){
					Recbuf[j+k+1]=Recbuf[j+k+1]-87;
				}else{
					Recbuf[j+k+1]=Recbuf[j+k+1]-48;
				}
				temp[k]=Recbuf[j+k]*16+Recbuf[j+k+1];
				j=j+2;
			}
			k=0;
			for(k=0;k<6;k++){
				if(sc1667_mailing_address[k]!=temp[k]){
					break;
				}
			}
			if(k==6){
				sc1667_read_data( Ctrl_initiate , Cmd_local_ReadMAC , L_Empty, Data_Empty );
			}else{
				i=0;
				for(j=0;(i!=6)&&(j<Total_sc1667_num);j++){
						for(i=0;i<6;i++){
								if(sc1667_search_address[j][i]!=temp[i]){
									break;
								}
						}
					}
				if(i==6){
					sc1667_remote_read_data( Cmd_remote_ReadMAC , temp );
				}else{
					printf("\r                ��ƥ���ͨѶ��ַ,����\n");
				}
			}
		}else if(strcmp(temp,"password") == 0){
			sc1667_read_data( Ctrl_initiate , Cmd_local_Readpassword, L_Empty, Data_Empty );
		}else if(strcmp(temp,"parameter") == 0){		
			j=j+i+1;
			for(k=0;k<6;k++){
				if(Recbuf[j+k]>96){
					Recbuf[j+k]=Recbuf[j+k]-87;
				}else{
					Recbuf[j+k]=Recbuf[j+k]-48;
				}
				if(Recbuf[j+k+1]>96){
					Recbuf[j+k+1]=Recbuf[j+k+1]-87;
				}else{
					Recbuf[j+k+1]=Recbuf[j+k+1]-48;
				}
				temp[k]=Recbuf[j+k]*16+Recbuf[j+k+1];
				j=j+2;
			}
			k=0;
			for(k=0;k<6;k++){
				if(sc1667_mailing_address[k]!=temp[k]){
					break;
				}
			}
			if(k==6){
				sc1667_read_data( Ctrl_initiate , Cmd_local_Readparameter, L_Empty, Data_Empty );
			}else{
				i=0;
				for(j=0;(i!=6)&&(j<Total_sc1667_num);j++){
						for(i=0;i<6;i++){
								if(sc1667_search_address[j][i]!=temp[i]){
									break;
								}
						}
					}
				if(i==6){
					sc1667_remote_read_data( Cmd_remote_Readparameter , temp );
				}else{
					printf("\r                ��ƥ���ͨѶ��ַ,����\n");
				}
			}
		}else if(strcmp(temp,"data") == 0){
			j=j+i+1;
//			memset(Automatic_data_acquisition,-1, 42); 
//			printf("j=%d\n",j);
			for(k=0;k<6;k++){
				temp[k]=(Recbuf[j+k]-48)*16+(Recbuf[j+k+1]-48);
//				printf("j=%d Recbuf[j+k]-48=%d Recbuf[j+k+1]-48=%d\n",j,Recbuf[j+k]-48,Recbuf[j+k+1]-48);	
				j=j+2;
			}
			k=0;
			for(l=0;(l<Total_meters_num)&&(k!=6);l++){
				for(k=0;k<6;k++){
					if(address[l][k]!=temp[k]){
						break;
					}
				}
			}
			if(k==6){
				dlt645_data_acquisition(Automatic_data_acquisition,address,l);
				dlt645_show_flash_index(Automatic_data_acquisition);
			}else{
				i=0;
				for(k=1;(i!=6)&&(k<=Total_sc1667_num);k++){
					for(j=0;(i!=6)&&(j<STA_meter_address[k][6]);j++){
							for(i=0;i<6;i++){
//									printf("k=%d j=%d i=%d STA_meter_address[k][7+i+6*j]=%d temp[i]=%d\n",k,j,i,STA_meter_address[k][7+i+6*j],temp[i]);
									if(STA_meter_address[k][7+i+6*j]!=temp[i]){
										break;
									}
							}
						}
				}

				if(i==6){
					k=k-1;
					for(l=0;l<6;l++){
						temp[l]=STA_meter_address[k][l];
					}
					user_Data[0]=0x04;
					user_Data[1]=0x01;
					user_Data[2]=j-1;
					sc1667_send_data(temp ,3, user_Data );
				}else{
					printf("\r                ��ƥ��ĵ��,����\n");
				}
			}
		}else if(strcmp(temp,"flash") == 0){
			j=j+i+1;
			memset(Automatic_data_acquisition,-1, 42); 
			for(k=0;k<9;k++){
				if(Recbuf[j+k]>96){
					Recbuf[j+k]=Recbuf[j+k]-87;
				}else{
					Recbuf[j+k]=Recbuf[j+k]-48;
				}
				if(Recbuf[j+k+1]>96){
					Recbuf[j+k+1]=Recbuf[j+k+1]-87;
				}else{
					Recbuf[j+k+1]=Recbuf[j+k+1]-48;
				}
				if(k>=6){
					temp[k]=Recbuf[j+k]*10+Recbuf[j+k+1];
				}else{
					temp[k]=Recbuf[j+k]*16+Recbuf[j+k+1];
				}
				j=j+2;
			}
//			printf("\r\n");
//			for(k=0;k<6;k++){
//				printf(" %0.2x",temp[k]);
//			}
//			printf("\r\n");
//			for(k=6;k<9;k++){
//				printf(" %d",temp[k]);
//			}
//			printf("\r\n");
			k=0;
			for(k=0;k<6;k++){
				if(sc1667_mailing_address[k]!=temp[k]){
					break;
				}
			}
			if(k==6){
				dlt645_read_flash_data (temp[6],temp[7],temp[8]);
			}else{
				i=0;
				for(j=0;(i!=6)&&(j<Total_sc1667_num);j++){
						for(i=0;i<6;i++){
								if(sc1667_search_address[j][i]!=temp[i]){
									break;
								}
						}
					}

				if(i==6){
					for(l=0;l<6;l++){
						sc1667_search_address[j-1][i];
					}
					user_Data[0]=0x06;
					user_Data[1]=0x03;
					user_Data[2]=temp[6];
					user_Data[3]=temp[7];
					user_Data[4]=temp[8];
					sc1667_send_data(temp ,5, user_Data );
				}else{
					printf("\r                ��ƥ���ͨѶ��ַ,����\n");
				}
			}
			
			
		}else if(strcmp(temp,"STA") == 0){	
			printf("\r FzuDq(config)# ��STA������%d̨\n",Total_sc1667_num);
				for(j=1;j<=Total_sc1667_num;j++){
					printf("\r                ��%d̨STA��ַ : ",j);
					for(i=0;i<6;i++){
						printf(" %02x",sc1667_search_address[j][i]);
					}
					printf("\n");
				}
		}else if(strcmp(temp,"CCO") == 0){	
			printf("\r FzuDq(config)# CCO��ַ %02x %02x %02x %02x %02x %02x \n",sc1667_cco_address[0],sc1667_cco_address[1],sc1667_cco_address[2],sc1667_cco_address[3],sc1667_cco_address[4],sc1667_cco_address[5]);
		}else if(strcmp(temp,"meter") == 0){	
			printf("\r FzuDq(config)# ");
			printf("�����ܵ���� %d \n",Total_meters_num);	
			for(i=0;i<Total_meters_num;i++){
				printf("\r                ��%d̨����ַ :  ",i+1);
				for(j=0;j<6;j++){
					printf("%02x ",address[i][j]);
				}
				printf("  ������%d \n",address_Baud_rate[i]);
			}
			
			for(k=1;k<=Total_sc1667_num;k++){
				printf("\r FzuDq(config)# STA %d ͨ�ŵ�ַΪ: %02x %02x %02x %02x %02x %02x ��%d�����:\n",k,STA_meter_address[k][0],STA_meter_address[k][1],STA_meter_address[k][2],STA_meter_address[k][3],STA_meter_address[k][4],STA_meter_address[k][5],STA_meter_address[k][6]);
							for(j=0;j<STA_meter_address[k][6];j++){
												printf("\r                ��%d̨����ַ : ",j+1);
												for(i=0;i<6;i++){
													printf(" %02x",STA_meter_address[k][7+i+6*j]);
												}
												printf("\n");
											}
			}

	}
}
}







int8_t pull_data_from_queue_pi(void)
{
//	if(Receive_Flag==1){
		QUEUE_DATA_TYPE *rx_pi_data;	
			
		/*�ӻ�������ȡ���ݣ����д���*/
		rx_pi_data = cbRead(&rx_queue_pi); 

		if(rx_pi_data != NULL)//������зǿ�
		{		
			//�����ַ���������������ֱ������ַ���
			*(rx_pi_data->head+rx_pi_data->len) = '\0';
			
			QUEUE_DEBUG("Pi���յ������ݣ�%s",rx_pi_data->head);
			QUEUE_DEBUG_ARRAY((uint8_t*)rx_pi_data->head,rx_pi_data->len);
			
			pi_processing_data(rx_pi_data->head);
			//ʹ�������ݱ������cbReadFinish���¶�ָ��
			cbReadFinish(&rx_queue_pi);
			
//			Usart_SendString( Pi_USART4,"\r Here");
//			printf("\r FzuDq(config)# ");
		}


//	}
}

int8_t pi_processing_data(uint8_t *Recbuf){
uint32_t i=0,j,k,L,l;
	uint8_t Cmd;
	uint16_t len;
	uint8_t Data[200],temp[6];
	uint8_t Rx_Buffer[42];
	uint8_t temp_address[6];
  len=(Recbuf[1]>>4)*16+(Recbuf[1]&0x0F);
//	printf("len=%d\n",len);
	Cmd=Recbuf[0];
//	printf("Cmd=%d\n",Cmd);
//	printf("\r FzuDq(config)# ");
	switch(Cmd)
			{
			case Cmd_Pi_ReadCCOmailaddress:
			{
//				Usart_SendHalfWord( Pi_USART4,Cmd_Pi_ReadCCOmailaddress<<2+Cmd_L_Empty);
				Data[0]=Cmd_Pi_ReadCCOmailaddress;
				Data[1]=Cmd_L_Empty;
				for(i=0;i<6;i++){
					Data[i+2]=sc1667_mailing_address[i];
				}
				Usart_SendArray(Pi_USART4,Data,8);

				break;

			}
			case Cmd_Pi_ReadSTAmailaddress :
			{
				Data[0]=Cmd_Pi_ReadSTAmailaddress;
				Data[1]=Total_sc1667_num*6+1;
				
				Data[2]=Total_sc1667_num;
				for(j=1;j<=Total_sc1667_num;j++){
					for(i=0;i<6;i++){
						Data[3+6*(j-1)+i]=sc1667_search_address[j][i];
					}
				}
				
				Usart_SendArray(Pi_USART4,Data,Total_sc1667_num*6+3);
				break;
			}
			case Cmd_Pi_Readmeteraddress :
			{
				Data[0]=Cmd_Pi_Readmeteraddress;
				Data[1]=0;
				
				for(i=0;i<6;i++){
					if(Recbuf[2+i]!=sc1667_mailing_address[i]){
						break;
					}
				}
				if(i==6){
					Data[1]=6*Total_meters_num+1;
					Data[2]=Total_meters_num;
					for(j=1;j<=Total_meters_num;j++){
						for(i=0;i<6;i++){
							Data[3+6*(j-1)+i]=address[j][i];
						}
					}
				}else{
					for(k=1;k<=Total_sc1667_num;k++){
						for(j=0;j<6;j++){
							if(Recbuf[2+j]!=STA_meter_address[k][j]){
								break;
							}
						}
						if(j==6){
							Data[1]=6*STA_meter_address[k][6]+1;
							Data[2]=STA_meter_address[k][6];
							for(i=0;i<Data[1]-1;i++){
								Data[3+i]=STA_meter_address[k][7+i];
							}
							k=Total_sc1667_num+1;
							break;
						}
					}
				}
				Usart_SendArray(Pi_USART4,Data,Data[1]+2);
				break;

			}
			
			
			case Cmd_Pi_Readmeterdata:{
//				memset(Automatic_data_acquisition,-1, 42); 
				for(k=0;k<6;k++){
					temp[k]=Recbuf[2+k];
	//				printf("j=%d Recbuf[j+k]-48=%d Recbuf[j+k+1]-48=%d\n",j,Recbuf[j+k]-48,Recbuf[j+k+1]-48);	
				}
				k=0;
				for(l=0;(l<Total_meters_num)&&(k!=6);l++){
					for(k=0;k<6;k++){
						if(address[l][k]!=temp[k]){
							break;
						}
					}
				}
				if(k==6){
					dlt645_data_acquisition(Automatic_data_acquisition,address,l);
					Data[0]=Cmd_Pi_Readmeterdata;
					Data[1]=42;
					for(i=0;i<42;i++){
						Data[2+i]=Automatic_data_acquisition[i];
					}
					Usart_SendArray(Pi_USART4,Data,44);
//					dlt645_show_flash_index(Automatic_data_acquisition);
				}else{
					i=0;
					for(k=1;(i!=6)&&(k<=Total_sc1667_num);k++){
						for(j=0;(i!=6)&&(j<STA_meter_address[k][6]);j++){
								for(i=0;i<6;i++){
	//									printf("k=%d j=%d i=%d STA_meter_address[k][7+i+6*j]=%d temp[i]=%d\n",k,j,i,STA_meter_address[k][7+i+6*j],temp[i]);
										if(STA_meter_address[k][7+i+6*j]!=temp[i]){
											break;
										}
								}
							}
					}

					if(i==6){
						k=k-1;
						for(l=0;l<6;l++){
							temp[l]=STA_meter_address[k][l];
						}
						Data[0]=0x04;
						Data[1]=0x01;
						Data[2]=j-1;
						sc1667_send_data(temp ,3, Data );
						pirequest=1;
					}else{
						printf("\r                ��ƥ��ĵ��,����\n");
					}
			
			}
				
			
			
			default :
			{
					printf("��ƥ��\n" );
			}

		}
	return 1;
}
}