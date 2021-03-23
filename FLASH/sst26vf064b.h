/* 
 * File:   sst26vf064b.h
 * Author: tuantm
 *
 * Created on November 29, 2016, 6:40 PM
 */

#ifndef SST26VF064B_H
#define	SST26VF064B_H

#ifdef	__cplusplus
extern "C" {
#endif
extern unsigned int data_256[256];
extern unsigned int block_protection_18[18];
void Portassign();
void SetSIO0_Output();
void SetSIO0_Input();
void SetSIO1_Output();
void SetSIO1_Input();
void SetSIO2_Output();
void SetSIO2_Input();
void SetSIO3_Output();
void SetSIO3_Input();
void SIO0_Low();
void SIO1_Low();
void SIO2_Low();
void SIO3_Low();
void SIO0_High();
void SIO1_High();
void SIO2_High();
void SIO3_High();
void SCK_High();
void SCK_Low();
void CE_High();
void CE_Low();
void Reset_Hold_Low();
void Reset_Hold_High();
void WP_Low();
void WP_High();

void DisplayLED(char Led_Stat);

  /* Function Prototypes */

void init();
void NoOp();
void Send_Byte(unsigned char out);
void Send_Double_Byte(unsigned char out);
void SendSQI_Byte(unsigned char out);
unsigned char Get_Byte();
unsigned char Get_Double_Byte();
unsigned char GetSQI_Nibble();
unsigned char GetSQI_Byte();
void ResetEn();
void Reset();
void En_QIO();
void Reset_QIO();
unsigned char Read(uint32_t Dst);
void Read_Cont(uint32_t Dst, uint32_t no_bytes);
unsigned char HighSpeed_Read(uint32_t Dst);
void HighSpeed_Read_Cont(uint32_t Dst, uint32_t no_bytes);
void Set_Burst(unsigned char byte);
void Read_Burst(uint32_t Dst, unsigned char burstlength);
void Read_PI(unsigned char Dst, unsigned char datalength);
void Read_Index(uint32_t Dst, unsigned char datalength);
void Read_BlockIndex(unsigned char Dst, unsigned char datalength);
void Jedec_ID_Read();
void QuadJ_ID_Read();
void SPI_Quad_IO_InMode_Read_Cont(uint32_t Dst, uint32_t no_bytes);
unsigned char SPI_Quad_IO_InMode_Read(uint32_t Dst);
unsigned char SPI_Quad_IO_NotInMode_Read(uint32_t Dst);
void SPI_Quad_IO_NotInMode_Read_Cont(uint32_t Dst, uint32_t no_bytes);
unsigned char SPI_Double_IO_InMode_Read(uint32_t Dst);
unsigned char SPI_Double_IO_NotInMode_Read(uint32_t Dst);
void SPI_Double_IO_NotInMode_Read_Cont(uint32_t Dst, uint32_t no_bytes);
void SPI_Double_IO_InMode_Read_Cont(uint32_t Dst, uint32_t no_bytes);
void HighSpeed_NotInMode_Read_Cont(uint32_t Dst, uint32_t no_bytes);
void HighSpeed_InMode_Read_Cont(uint32_t Dst, uint32_t no_bytes);
unsigned char HighSpeed_InMode_Read(uint32_t Dst);
unsigned char HighSpeed_NotInMode_Read(uint32_t Dst);
void Sector_Erase(uint32_t Dst);
void SPI_Block_Erase(uint32_t Dst, uint8_t size);
void Chip_Erase();
void Page_Program(uint32_t Dst);
void Write_Suspend();
void Write_Resume();
void ReadSID(unsigned char *security_ID, uint32_t Dst, uint32_t security_length);
void ProgSID(unsigned char *security_ID, uint32_t Dst, uint32_t security_length);
void LockSID();
unsigned char Read_Status_Register();
void WREN();
void WRDI();
void ReadBlockProtection();
void WriteBlockProtection();
void LockBlockProtection();
void SPI_Global_Block_Protection_Unlock();
void Global_Block_Protection_Unlock();

void Wait_Busy();
void Wait_Busy_special();
void SPI_Wait_Busy_special();
unsigned char SPI_SFDP_Read(uint32_t Dst);
extern int test_flash_sst26vf064b();
extern void flash_test(void);
extern uint8_t init_flash_status(void);
extern BOOL sst26vf064b_check_ID(void);
#ifdef	__cplusplus
}
#endif

#endif	/* SST26VF064B_H */

