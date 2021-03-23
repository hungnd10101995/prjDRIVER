#ifndef _24AA08_H_
#define _24AA08_H_



/*===========================================================================*/
#define T_wc        3

#define IHD_BILLING_DATA_START_ADDRESS  0x00
#define IHD_DATA_START_ADDRESS          0x180
#define IHD_ID_ADDRESS                  0x3C0
#define IHD_METER_ID_ADDRESS            0x3D0 
#define IHD_METER_TYPE_ADDRESS          (IHD_METER_ID_ADDRESS + 10)
#define IHD_DATA_INDEX_ADDRESS          (IHD_ID_ADDRESS + 10) //dia chi index cua manual read
#define IHD_NEW_ID_INDEX_ADDRESS        (IHD_METER_TYPE_ADDRESS + 4)//dia chi luu index khi thay doi ID
#define IHD_DATA_BI_INDEX_ADDRESS       0x3EA//dia chi index cua billing
#define IHD_BILLING_DAY_ADDRESS         0x3FA 
/////////////// FUNCTON ////////////////////////////
void ic24aa08_write_2byte(unsigned int address, int data);
extern void ic24aa08_write_4byte(unsigned int address, unsigned long data);
extern void ic24aa08_write_byte(unsigned int address, uint8_t data);//tuantm
extern void ic24aa08_write_5yte_rtc(unsigned int address);
//read
extern unsigned char ic24aa08_read_byte(unsigned int address); //tuantm - phuc vu rfmesh
extern unsigned long ic24aa08_read_4byte(unsigned int address);
extern int ic24aa08_read_2byte(unsigned int address);
extern void ic24aa08_read_5byte_rtc(unsigned int address, uint8_t index);
//other
extern void eeprom_data_save_task(void); //tuantm
extern void ic24aa08_load_parameters(void);
//extern unsigned char read_ext_eeprom(unsigned int address); //tuantm - phuc vu rfmesh

//them phan nay cho RFmesh version 2.0
extern void save_rfmesh_stt (unsigned char stt);
extern unsigned char load_rfmesh_stt();
extern unsigned long load_rfmesh_masterID (void);
extern void save_rfmesh_masterID (unsigned long id);
extern unsigned char save_path_eeprom();
extern unsigned char load_path_eeprom();


#endif //_24AA08_H_
