#ifndef __DYNAMIXEL_H
#define __DYNAMIXEL_H

#include "main.h"

// Dynamixel Protocol 2.0 Instructions
#define DXL_INST_PING          0x01
#define DXL_INST_READ          0x02
#define DXL_INST_WRITE         0x03
#define DXL_INST_REG_WRITE     0x04
#define DXL_INST_ACTION        0x05
#define DXL_INST_FACTORY_RESET 0x06
#define DXL_INST_REBOOT        0x08
#define DXL_INST_CLEAR         0x10
#define DXL_INST_STATUS        0x55
#define DXL_INST_SYNC_READ     0x82
#define DXL_INST_SYNC_WRITE    0x83
#define DXL_INST_BULK_READ     0x92
#define DXL_INST_BULK_WRITE    0x93

// XL430-W250 Control Table (Simplified)
#define DXL_ADDR_OPERATING_MODE 11
#define DXL_ADDR_TORQUE_ENABLE 64
#define DXL_ADDR_GOAL_POSITION 116
#define DXL_ADDR_GOAL_VELOCITY 104
#define DXL_ADDR_PRESENT_POSITION 132

void DXL_Init(void);
void DXL_SetPower(uint8_t enable);
void DXL_SetDir(uint8_t is_tx);

uint16_t DXL_UpdateCRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

HAL_StatusTypeDef DXL_Ping(uint8_t id);
HAL_StatusTypeDef DXL_Reboot(uint8_t id);
HAL_StatusTypeDef DXL_Read(uint8_t id, uint16_t addr, uint8_t *p_data, uint16_t len);
HAL_StatusTypeDef DXL_ReadWord(uint8_t id, uint16_t addr, uint32_t *p_data);
HAL_StatusTypeDef DXL_Write(uint8_t id, uint16_t addr, uint8_t *p_data, uint16_t len);
HAL_StatusTypeDef DXL_WriteByte(uint8_t id, uint16_t addr, uint8_t data);
HAL_StatusTypeDef DXL_WriteWord(uint8_t id, uint16_t addr, uint32_t data);

#endif /* __DYNAMIXEL_H */
