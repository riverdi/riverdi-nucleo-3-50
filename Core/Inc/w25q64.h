#ifndef __W25Q64_H_
#define __W25Q64_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*
 * FUNCTIONS
 */

HAL_StatusTypeDef
W25Q64_Init                    (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_ResetChip               (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_Configuration           (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_WriteEnable             (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_WriteDisable            (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_AutoPollingMemReady     (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_Erase_Chip              (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_EraseSector             (XSPI_HandleTypeDef  *xspi,
                                uint32_t             start_addr,
                                uint32_t             end_addr);

HAL_StatusTypeDef
W25Q64_Erase_Block_32KB        (XSPI_HandleTypeDef  *xspi,
                                uint32_t             block_addr);

HAL_StatusTypeDef
W25Q64_Erase_Block_64KB        (XSPI_HandleTypeDef  *xspi,
                                uint32_t             block_addr);

HAL_StatusTypeDef
W25Q64_Write                   (XSPI_HandleTypeDef  *xspi,
                                uint8_t             *data,
                                uint32_t             addr,
                                uint32_t             size);

HAL_StatusTypeDef
W25Q64_Read                    (XSPI_HandleTypeDef  *xspi,
                                uint8_t             *data,
                                uint32_t             addr,
                                uint32_t             size);

HAL_StatusTypeDef
W25Q64_EnableMemoryMappedMode  (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_IsBusy                  (XSPI_HandleTypeDef  *xspi);

HAL_StatusTypeDef
W25Q64_Read_Status_Registers   (XSPI_HandleTypeDef  *xspi,
                                uint8_t             *reg_data,
                                uint8_t              reg_num);

HAL_StatusTypeDef
W25Q64_Write_Status_Registers  (XSPI_HandleTypeDef  *xspi,
                                uint8_t              reg_data,
                                uint8_t              reg_num);

/*
 *  MEMORY COFIGURATION
 */

#define W25Q_FLASH_SIZE                       0x800000
#define W25Q_SECTOR_SIZE                      0x1000
#define W25Q_PAGE_SIZE                        0x100
#define W25Q_DUMMY_CYCLES_READ_QUAD           6
#define W25Q_AUTOPOLLING_INTERVAL_TIME        0x10U

/*
 *  MEMORY COMMANDS
 */

#define W25Q_WRITE_ENABLE_CMD             0x06U
#define W25Q_ENABLE_VOLATILE_SR_CMD       0x50U
#define W25Q_WRITE_DISABLE_CMD            0x04U
#define W25Q_SECTOR_ERASE_CMD             0x20U
#define W25Q_CHIP_ERASE_CMD               0xC7U
#define W25Q_READ_SR1_CMD                 0x05U
#define W25Q_WRITE_SR1_CMD                0x01U
#define W25Q_READ_SR2_CMD                 0x35U
#define W25Q_WRITE_SR2_CMD                0x31U
#define W25Q_READ_SR3_CMD                 0x15U
#define W25Q_WRITE_SR3_CMD                0x11U
#define W25Q_ENABLE_RST_CMD               0x66U
#define W25Q_RESET_CMD                    0x99U
#define W25Q_PAGE_PROGRAM_QUAD_INP_CMD    0x32U
#define W25Q_FAST_READ_QUAD_IO_CMD        0xEBU

/*
 *  MEMORY REGISTERS
 */

#define W25Q_SR_Quad_Enable               ((uint8_t)0x02)
#define W25Q_SR_DRV1                      ((uint8_t)0x9f)

#ifdef __cplusplus
}
#endif

#endif /* __W25Q64_H_ */
