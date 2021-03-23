#ifndef _I2C_H_
#define _I2C_H_

#define I2C_THD_STA		5
#define I2C_TSU_STA		5
#define I2C_TSU_DAT		1
#define I2C_TTR			3

#define	I2C_THIGH		5
#define I2C_TLOW		4

#define	I2C_TSU_STO		4
#define	I2C_TBUF		5

void i2c_init (void);
void i2c_start (void);
void i2c_stop (void);
void i2c_write (unsigned char output_data);
unsigned char i2c_read (char ack);
extern void i2c_reset(void); //tuantm

#endif