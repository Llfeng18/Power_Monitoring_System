/**
  ******************************************************************************
  * @file    rx_data_queue.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   环形缓冲区，适用于接收外部数据时用作缓冲
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
//实例化节点数据类型
QUEUE_DATA_TYPE  node_data[QUEUE_NODE_NUM]; 
QUEUE_DATA_TYPE  node_debug_data[QUEUE_NODE_NUM]; 
QUEUE_DATA_TYPE  node_pi_data[QUEUE_NODE_NUM]; 

//实例化队列类型
QueueBuffer rx_queue_pi;
QueueBuffer rx_queue;
QueueBuffer rx_queue_debug;


//队列缓冲区的内存池
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
/*环形缓冲队列*/

/**
  * @brief  初始化缓冲队列
  * @param  cb:缓冲队列结构体
  * @param  size: 缓冲队列的元素个数
  * @note 	初始化时还需要给cb->elems指针赋值
  */
void cbInit(QueueBuffer *cb, int size) 
{
    cb->size  = size;	/* maximum number of elements           */
    cb->read = 0; 		/* index of oldest element              */
    cb->write   = 0; 	 	/* index at which to write new element  */
//    cb->elems = (uint8_t *)calloc(cb->size, sizeof(uint8_t));  //elems 要额外初始化
}
 
//(此函数改成了宏，在头文件)
/**
  * @brief  输出缓冲队列当前的状态信息
  * @param  cb:缓冲队列结构体
  */
//void cbPrint(QueueBuffer *cb) 
//{
//    DATA_QUEUE_LOG("size=0x%x, read=%d, write=%d\n", cb->size, cb->read, cb->write);
//	  DATA_QUEUE_LOG("size=0x%x, read_using=%d, write_using=%d\n", cb->size, cb->read_using, cb->write_using);
//}
 
/**
  * @brief  判断缓冲队列是(1)否(0)已满
  * @param  cb:缓冲队列结构体
  */
int cbIsFull(QueueBuffer *cb) 
{
    return cb->write == (cb->read ^ cb->size); /* This inverts the most significant bit of read before comparison */ 
}
 
/**
  * @brief  判断缓冲队列是(1)否(0)全空
  * @param  cb:缓冲队列结构体
  */		
int cbIsEmpty(QueueBuffer *cb) 
{
    return cb->write == cb->read; 
}

/**
  * @brief  对缓冲队列的指针加1
  * @param  cb:缓冲队列结构体
  * @param  p：要加1的指针
  * @return  返回加1的结果
  */	
int cbIncr(QueueBuffer *cb, int p) 
{
    return (p + 1)&(2*cb->size-1); /* read and write pointers incrementation is done modulo 2*size */
}
 
/**
  * @brief  获取可写入的缓冲区指针
  * @param  cb:缓冲队列结构体
  * @return  可进行写入的缓冲区指针
  * @note  得到指针后可进入写入操作，但写指针不会立即加1，
           写完数据时，应调用cbWriteFinish对写指针加1
  */
QUEUE_DATA_TYPE* cbWrite(QueueBuffer *cb) 
{
    if (cbIsFull(cb)) /* full, overwrite moves read pointer */
    {
			return NULL;
		}		
		else
		{
			//当wriet和write_using相等时，表示上一个缓冲区已写入完毕，需要对写指针加1
			if(cb->write == cb->write_using)
			{
				cb->write_using = cbIncr(cb, cb->write); //未满，则增加1
			}
		}
		
	return  cb->elems[cb->write_using&(cb->size-1)];
}



/**
  * @brief 数据写入完毕，更新写指针到缓冲结构体
  * @param  cb:缓冲队列结构体
  */
void cbWriteFinish(QueueBuffer *cb)
{
    cb->write = cb->write_using;
}
 
/**
  * @brief  获取可读取的缓冲区指针
  * @param  cb:缓冲队列结构体
  * @return  可进行读取的缓冲区指针
  * @note  得到指针后可进入读取操作，但读指针不会立即加1，
					 读取完数据时，应调用cbReadFinish对读指针加1
  */
QUEUE_DATA_TYPE* cbRead(QueueBuffer *cb) 
{
		if(cbIsEmpty(cb))
			return NULL;
		
	//当read和read_using相等时，表示上一个缓冲区已读取完毕(即已调用cbReadFinish)，
	//需要对写指针加1
	if(cb->read == cb->read_using)	
		cb->read_using = cbIncr(cb, cb->read);
	
	return cb->elems[cb->read_using&(cb->size-1)];
}


/**
  * @brief 数据读取完毕，更新读指针到缓冲结构体
  * @param  cb:缓冲队列结构体
  */
void cbReadFinish(QueueBuffer *cb) 
{	
		//重置当前读完的数据节点的长度
		cb->elems[cb->read_using&(cb->size-1)]->len = 0;
	
    cb->read = cb->read_using;
}



//队列的指针指向的缓冲区全部销毁
void camera_queue_free(void)
{
    uint32_t i = 0;

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        if(node_data[i].head != NULL)
        {
					//若是动态申请的空间才要free
//            free(node_data[i].head);
            node_data[i].head = NULL;
        }
    }

    return;
}


/**
  * @brief  缓冲队列初始化，分配内存,使用缓冲队列时，
  * @param  无
  * @retval 无
  */
void rx_queue_init(void)
{
  uint32_t i = 0;

  memset(node_data, 0, sizeof(node_data));
		 
	/*初始化缓冲队列*/
	cbInit(&rx_queue,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_data[i].head = node_buff[i];
        
        /*初始化队列缓冲指针，指向实际的内存*/
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
		 
	/*初始化缓冲队列*/
	cbInit(&rx_queue_debug,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_debug_data[i].head = node_debug_buff[i];
        
        /*初始化队列缓冲指针，指向实际的内存*/
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
		 
	/*初始化缓冲队列*/
	cbInit(&rx_queue_pi,QUEUE_NODE_NUM);

    for(i = 0; i < QUEUE_NODE_NUM; i ++)
    {
        node_pi_data[i].head = node_pi_buff[i];
        
        /*初始化队列缓冲指针，指向实际的内存*/
        rx_queue_pi.elems[i] = &node_pi_data[i];
        
        DATA_QUEUE_LOG("node_pi_data[i].head=0x%x,\r\nrx_queue_pi.elems[i] =0x%x", (uint32_t)node_pi_data[i].head,(uint32_t)rx_queue_pi.elems[i]->head);

        memset(node_pi_data[i].head, 0, QUEUE_NODE_DATA_LEN);
    }
		
	cbPrint(rx_queue_pi);	
			printf("\r                Pi_Queue_Config\n");
}


/**
  * @brief  往队列中写入数据的样例
  */
void push_data_to_queue(char *src_dat,uint16_t src_len)
{
	QUEUE_DATA_TYPE *data_p;
	uint8_t i;
	
	for(i=0;i<src_len;i++)
	{
		/*获取写缓冲区指针，准备写入新数据*/
		data_p = cbWrite(&rx_queue);
		
		if (data_p != NULL)	//若缓冲队列未满，开始传输
		{		
			//往缓冲区写入数据，如使用串口接收、dma写入等方式
			*(data_p->head + i) = src_dat[i];
				data_p->len++;
			printf("\r\ndata_p->len =%d",data_p->len);
		}else return;	
		
		cbPrint(rx_queue);	
	}	
	
	/*写入缓冲区完毕*/
	cbWriteFinish(&rx_queue);
	
	cbPrint(rx_queue);	

}


/**
  * @brief  从队列中取数据的样例
  */
int8_t pull_data_from_queue(void)
{
	QUEUE_DATA_TYPE *rx_data;	
		
	/*从缓冲区读取数据，进行处理，*/
	rx_data = cbRead(&rx_queue); 

	if(rx_data != NULL)//缓冲队列非空
	{		
		//加上字符串结束符，方便直接输出字符串
		*(rx_data->head+rx_data->len) = '\0';
//		printf("\r FzuDq(config)# ");
//		printf("sc1667接收到长度为%d的数据\r\n",rx_data->len);	
//		QUEUE_DEBUG("接收到的数据：%s",rx_data->head);
		QUEUE_DEBUG_ARRAY((uint8_t*)rx_data->head,rx_data->len);
		if(rx_data->len<3){
			printf("\r FzuDq(config)# ");
			printf("sc1667接收到非法帧丢弃 \n");	
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
		//使用完数据必须调用cbReadFinish更新读指针
		cbReadFinish(&rx_queue);
//		printf("\r FzuDq(config)# ");
	}
}


int8_t pull_data_from_queue_debug(void)
{
//	if(Receive_Flag==1){
		QUEUE_DATA_TYPE *rx_debug_data;	
			
		/*从缓冲区读取数据，进行处理，*/
		rx_debug_data = cbRead(&rx_queue_debug); 

		if(rx_debug_data != NULL)//缓冲队列非空
		{		
			//加上字符串结束符，方便直接输出字符串
			*(rx_debug_data->head+rx_debug_data->len) = '\0';
			
			QUEUE_DEBUG("接收到的数据：%s",rx_debug_data->head);
			QUEUE_DEBUG_ARRAY((uint8_t*)rx_debug_data->head,rx_debug_data->len);
			
			debug_processing_data(rx_debug_data->head);
			//使用完数据必须调用cbReadFinish更新读指针
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
		
		/*用接收到的时间设置RTC*/
		Time_Adjust(&set_time);
		
		//向备份寄存器写入标志
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
//		printf("here\n");
		ShowTime=1;
		newTime=1;
	}else if(strcmp(temp,"debug") == 0){
			if(debug_index==0){
				debug_index=1;
				printf("\r FzuDq(config)# debug模式开启\n");
			}else{
				debug_index=0;
				printf("\r FzuDq(config)# debug模式关闭\n");
			}
	}else if(strcmp(temp,"STA/CCO") == 0){
			if(CCO==1){
				CCO=0;
				printf("\r FzuDq(config)# STA模式开启\n");
			}else{
				CCO=1;
				printf("\r FzuDq(config)# CCO模式开启\n");
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
			printf("\r FzuDq(config)# 所有命令提示\n");
			printf("\r                show time                               查看当前设备时间\n");
			printf("\r                show mailaddress                        查看当前设备HPLC通讯地址\n");
			printf("\r                show version XX XX XX XX XX XX          查看通讯地址为XX XX XX XX XX XX的设备HPLC版本信息\n");
			printf("\r                show MAC XX XX XX XX XX XX              查看通讯地址为XX XX XX XX XX XX的设备HPLC MAC地址\n");
			printf("\r                show password                           查看当前设备HPLC网络口令\n");
			printf("\r                show parameter XX XX XX XX XX XX        查看通讯地址为XX XX XX XX XX XX的设备HPLC网络参数\n");
			printf("\r                show CCO                                查看当前设备CCO通讯地址(仅仅针对STA设备有效)\n");
			printf("\r                show STA                                查看当前设备STA通讯地址(仅仅针对CCO设备有效)\n");
			printf("\r                show meter                              查看所有设备所连接电表地址\n");
			printf("\r                show data XX XX XX XX XX XX             查看地址为XX XX XX XX XX XX的电表的运行数据\n");
			printf("\r                show flash XX XX XX XX XX XX XX XX XX   查看通讯地址为XX XX XX XX XX XX的设备XX日XX时第XX个电表的运行数据\n");
			printf("\r                time XXXX/XX/XX XX:XX:XX                修改当前设备时间设备时间\n");
			printf("\r                debug                                   打开/关闭调试模式\n");
			printf("\r                STA/CCO                                 STA或CCO模式切换\n");
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
					printf("\r                无匹配的通讯地址,请检查\n");
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
					printf("\r                无匹配的通讯地址,请检查\n");
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
					printf("\r                无匹配的通讯地址,请检查\n");
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
					printf("\r                无匹配的电表,请检查\n");
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
					printf("\r                无匹配的通讯地址,请检查\n");
				}
			}
			
			
		}else if(strcmp(temp,"STA") == 0){	
			printf("\r FzuDq(config)# 总STA数量有%d台\n",Total_sc1667_num);
				for(j=1;j<=Total_sc1667_num;j++){
					printf("\r                第%d台STA地址 : ",j);
					for(i=0;i<6;i++){
						printf(" %02x",sc1667_search_address[j][i]);
					}
					printf("\n");
				}
		}else if(strcmp(temp,"CCO") == 0){	
			printf("\r FzuDq(config)# CCO地址 %02x %02x %02x %02x %02x %02x \n",sc1667_cco_address[0],sc1667_cco_address[1],sc1667_cco_address[2],sc1667_cco_address[3],sc1667_cco_address[4],sc1667_cco_address[5]);
		}else if(strcmp(temp,"meter") == 0){	
			printf("\r FzuDq(config)# ");
			printf("本地总电表数 %d \n",Total_meters_num);	
			for(i=0;i<Total_meters_num;i++){
				printf("\r                第%d台电表地址 :  ",i+1);
				for(j=0;j<6;j++){
					printf("%02x ",address[i][j]);
				}
				printf("  波特率%d \n",address_Baud_rate[i]);
			}
			
			for(k=1;k<=Total_sc1667_num;k++){
				printf("\r FzuDq(config)# STA %d 通信地址为: %02x %02x %02x %02x %02x %02x 共%d个电表:\n",k,STA_meter_address[k][0],STA_meter_address[k][1],STA_meter_address[k][2],STA_meter_address[k][3],STA_meter_address[k][4],STA_meter_address[k][5],STA_meter_address[k][6]);
							for(j=0;j<STA_meter_address[k][6];j++){
												printf("\r                第%d台电表地址 : ",j+1);
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
			
		/*从缓冲区读取数据，进行处理，*/
		rx_pi_data = cbRead(&rx_queue_pi); 

		if(rx_pi_data != NULL)//缓冲队列非空
		{		
			//加上字符串结束符，方便直接输出字符串
			*(rx_pi_data->head+rx_pi_data->len) = '\0';
			
			QUEUE_DEBUG("Pi接收到的数据：%s",rx_pi_data->head);
			QUEUE_DEBUG_ARRAY((uint8_t*)rx_pi_data->head,rx_pi_data->len);
			
			pi_processing_data(rx_pi_data->head);
			//使用完数据必须调用cbReadFinish更新读指针
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
						printf("\r                无匹配的电表,请检查\n");
					}
			
			}
				
			
			
			default :
			{
					printf("无匹配\n" );
			}

		}
	return 1;
}
}