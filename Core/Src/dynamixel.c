#include "dynamixel.h"
#include <string.h>

extern UART_HandleTypeDef huart4;

static const uint16_t crc_table[256] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

void DXL_Init(void) {
    DXL_SetPower(1);
    DXL_SetDir(0); // RX mode
}

void DXL_SetPower(uint8_t enable) {
    HAL_GPIO_WritePin(DXL_PWR_EN_GPIO_Port, DXL_PWR_EN_Pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void DXL_SetDir(uint8_t is_tx) {
    HAL_GPIO_WritePin(DXL_DIR_GPIO_Port, DXL_DIR_Pin, is_tx ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint16_t DXL_UpdateCRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size) {
    uint16_t i, j;
    for (j = 0; j < data_blk_size; j++) {
        i = ((crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }
    return crc_accum;
}

static HAL_StatusTypeDef DXL_Transmit(uint8_t *p_data, uint16_t len) {
    // Aggressive RX Flush before transmit to clear any stale data (like old Status Packets)
    uint8_t dummy;
    while (__HAL_UART_GET_FLAG(&huart4, UART_FLAG_RXNE)) {
        dummy = (uint8_t)(huart4.Instance->RDR & 0xFF);
        (void)dummy;
    }
    __HAL_UART_CLEAR_OREFLAG(&huart4);

    __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_TC);
    DXL_SetDir(1);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart4, p_data, len, 10);
    if (status == HAL_OK) {
        while(__HAL_UART_GET_FLAG(&huart4, UART_FLAG_TC) == RESET);
    }
    
    // Exact delay for 1Mbps (approx 10-20us) to ensure Stop bit is on the wire
    for(volatile int i=0; i<100; i++); 
    
    DXL_SetDir(0);
    return status;
}

static HAL_StatusTypeDef DXL_Receive(uint8_t *p_data, uint16_t len) {
    // Clear any hardware errors that might have occurred during/after transmit
    __HAL_UART_CLEAR_OREFLAG(&huart4);
    __HAL_UART_CLEAR_NEFLAG(&huart4);
    __HAL_UART_CLEAR_FEFLAG(&huart4);
    
    return HAL_UART_Receive(&huart4, p_data, len, 20); // 20ms is plenty for 1Mbps
}

HAL_StatusTypeDef DXL_Ping(uint8_t id) {
    uint8_t packet[10];
    packet[0] = 0xFF;
    packet[1] = 0xFF;
    packet[2] = 0xFD;
    packet[3] = 0x00;
    packet[4] = id;
    packet[5] = 0x03;
    packet[6] = 0x00;
    packet[7] = DXL_INST_PING;
    
    uint16_t crc = DXL_UpdateCRC(0, packet, 8);
    packet[8] = crc & 0xFF;
    packet[9] = (crc >> 8) & 0xFF;
    
    if (DXL_Transmit(packet, 10) != HAL_OK) return HAL_ERROR;
    
    // Status Packet for Ping is 14 bytes: FF FF FD 00 ID 07 00 55 ERR MODEL_L MODEL_H VER CRC_L CRC_H
    uint8_t rx_buf[14];
    HAL_StatusTypeDef rx_status = DXL_Receive(rx_buf, 14);
    
    if (rx_status == HAL_OK && rx_buf[4] == id) {
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

HAL_StatusTypeDef DXL_Reboot(uint8_t id) {
    uint8_t packet[10] = {0xFF, 0xFF, 0xFD, 0x00, id, 0x03, 0x00, DXL_INST_REBOOT};
    uint16_t crc = DXL_UpdateCRC(0, packet, 8);
    packet[8] = crc & 0xFF;
    packet[9] = (crc >> 8) & 0xFF;
    return DXL_Transmit(packet, 10);
}

HAL_StatusTypeDef DXL_Read(uint8_t id, uint16_t addr, uint8_t *p_data, uint16_t len) {
    uint8_t packet[14];
    packet[0] = 0xFF;
    packet[1] = 0xFF;
    packet[2] = 0xFD;
    packet[3] = 0x00;
    packet[4] = id;
    packet[5] = 0x07;
    packet[6] = 0x00;
    packet[7] = DXL_INST_READ;
    packet[8] = addr & 0xFF;
    packet[9] = (addr >> 8) & 0xFF;
    packet[10] = len & 0xFF;
    packet[11] = (len >> 8) & 0xFF;
    
    uint16_t crc = DXL_UpdateCRC(0, packet, 12);
    packet[12] = crc & 0xFF;
    packet[13] = (crc >> 8) & 0xFF;
    
    if (DXL_Transmit(packet, 14) != HAL_OK) return HAL_ERROR;
    
    // Status Packet Protocol 2.0: FF FF FD 00 ID LEN_L LEN_H 55 ERR PARAM... CRC_L CRC_H
    // Length = 11 + Parameters (len)
    uint8_t rx_buf[128];
    if (11 + len > 128) return HAL_ERROR;
    
    if (DXL_Receive(rx_buf, 11 + len) == HAL_OK) {
        // Headers are 0xFF 0xFF 0xFD 0x00
        if (rx_buf[0] == 0xFF && rx_buf[1] == 0xFF && rx_buf[2] == 0xFD && 
            rx_buf[4] == id && rx_buf[8] == 0) { // Check ID and Error byte
            memcpy(p_data, &rx_buf[9], len);
            return HAL_OK;
        }
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef DXL_ReadWord(uint8_t id, uint16_t addr, uint32_t *p_data) {
    uint8_t buf[4] = {0,};
    if (DXL_Read(id, addr, buf, 4) == HAL_OK) {
        *p_data = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
        return HAL_OK;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef DXL_Write(uint8_t id, uint16_t addr, uint8_t *p_data, uint16_t len) {
    uint8_t packet[128]; // Max packet size for this simple example
    if (len + 12 > 128) return HAL_ERROR;

    packet[0] = 0xFF;
    packet[1] = 0xFF;
    packet[2] = 0xFD;
    packet[3] = 0x00;
    packet[4] = id;
    
    uint16_t total_len = len + 5; // addr(2) + inst(1) + crc(2)
    packet[5] = total_len & 0xFF;
    packet[6] = (total_len >> 8) & 0xFF;
    packet[7] = DXL_INST_WRITE;
    packet[8] = addr & 0xFF;
    packet[9] = (addr >> 8) & 0xFF;
    
    memcpy(&packet[10], p_data, len);
    
    uint16_t crc = DXL_UpdateCRC(0, packet, 10 + len);
    packet[10 + len] = crc & 0xFF;
    packet[11 + len] = (crc >> 8) & 0xFF;
    
    return DXL_Transmit(packet, 12 + len);
}

HAL_StatusTypeDef DXL_WriteByte(uint8_t id, uint16_t addr, uint8_t data) {
    return DXL_Write(id, addr, &data, 1);
}

HAL_StatusTypeDef DXL_WriteWord(uint8_t id, uint16_t addr, uint32_t data) {
    uint8_t buf[4];
    buf[0] = data & 0xFF;
    buf[1] = (data >> 8) & 0xFF;
    buf[2] = (data >> 16) & 0xFF;
    buf[3] = (data >> 24) & 0xFF;
    return DXL_Write(id, addr, buf, 4);
}
