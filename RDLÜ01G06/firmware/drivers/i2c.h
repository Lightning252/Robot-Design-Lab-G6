#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/stm32f10x_i2c.h"

u8 I2C1OperationFinished();
u8 I2C1SendBytes(u8 *data, u8 size, u8 addr);
u8 I2C1ReadBytes(u8 size, u8 addr);
u8 I2C1WriteReadBytes(u8 *txdata, u8 txsize, u8 rxsize, u8 addr);

/**
 * This function checks weather the current i2c operation 
 * e.g. I2C2SendBytes, I2C2ReadBytes or I2C2WriteReadBytes
 * was finished.
 *
 * returns 0 if unfinished and !0 if finished
 */
u8 I2C2OperationFinished();

/**
 * This function will try to start an asynchronious I2C
 * data transfer on the I2C2 interface. It will write
 * the given data on the bus, addressed to the given
 * address.
 *
 * Note this function may fail.
 *
 * Returns 0 on success and 1 on failure
 */
u8 I2C2SendBytes(u8 *data, u8 size, u8 addr);

/**
 * This function will try to start an asynchronious I2C
 * data transfer on the I2C2 interface. It will reads size 
 * bytes from the bus from the device with the address addr.
 * 
 * Note this function fails if the initialization
 * of the reading fails.
 * 
 * Note also one must call I2C2OperationFinished() to 
 * find out if the operation finished and handleI2C2Errors
 * to check if errors occured.
 * 
 * Returns 0 on success and 1 on failure
 * */
u8 I2C2ReadBytes(u8 size, u8 addr);
u8 I2C2WriteReadBytes(u8 *txdata, u8 txsize, u8 rxsize, u8 addr);

/**
 * This function tries to write the given data on 
 * the I2C2 bus. This function will only return
 * if the write was sucessfull and completed. 
 *
 * Note if this function fails it will block the
 * programm infinite.
 */
void I2C2SendBytesBlocking(u8 *data, u8 size, u8 addr);

/**
 * This function sets up the I2C2 bus with the given
 * Adress as its own address (for receving data) and
 * the given speed. It will also configure the IO Pins,
 * activate the interrupts and the RCC.
 */
void setupI2C2(u16 address, int speed);

/**
 * This function sets up the I2C1 bus with the given
 * Adress as its own address (for receving data) and
 * the given speed. It will also configure the IO Pins,
 * activate the interrupts and the RCC.
 */
void setupI2C1(u16 address, int speed, FunctionalState remapped);

/**
 * This function checks weather an error occured during the
 * current I2C operation and tries to handle it. If an error
 * occured the the current operation is canceled and needs
 * to be restarted.
 *
 * Known Bugs: In case of Bus error the recovery does not allways work
 *
 * Return 1 if an error occured 0 otherwise
 */
u8 handleI2C2Errors();

/**
 * This function checks weather an error occured during the
 * current I2C operation and tries to handle it. If an error
 * occured the the current operation is canceled and needs
 * to be restarted.
 *
 * Known Bugs: In case of Bus error the recovery does not allways work
 *
 * Return 1 if an error occured 0 otherwise
 */
u8 handleI2C1Errors();

#ifdef __cplusplus
}
#endif

#endif
