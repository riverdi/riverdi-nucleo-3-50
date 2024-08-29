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

#define W25Q_FLASH_SIZE                  			0x800000
#define W25Q_SECTOR_SIZE                 			0x1000
#define W25Q_PAGE_SIZE                   			0x100
#define W25Q_DUMMY_CYCLES_READ_QUAD      			6
#define W25Q_AUTOPOLLING_INTERVAL_TIME   			0x10U

/*
 *  MEMORY COMMANDS
 */

#define W25Q_WRITE_ENABLE_CMD 						0x06U	/* sets WEL bit, must be set before any write/program/erase */
#define W25Q_ENABLE_VOLATILE_SR_CMD 				0x50U	/* check 7.1 in data sheet */
#define W25Q_WRITE_DISABLE_CMD 						0x04U	/* resets WEL bit (state after power-up) */

#define W25Q_POWERUP_CMD 							0xABU	/* release power-down */
#define W25Q_FULLID_CMD 							0x90U	/* read Manufacturer ID & Device ID */
#define W25Q_READ_JEDEC_ID_CMD 						0x9FU	/* read JEDEC-standard ID */
#define W25Q_READ_UID_CMD 							0x4BU	/* read unique chip 64-bit ID */

#define W25Q_READ_DATA_CMD 							0x03U	/* read data by standard SPI */
#define W25Q_FAST_READ_CMD 							0x0BU	/* highest FR speed (8.2.12) */

#define W25Q_PAGE_PROGRAM_CMD 						0x02U	/* program page (256bytes) by single SPI line */

#define W25Q_SECTOR_ERASE_CMD 						0x20U	/* sets all 4Kbyte sector with 0xFF (erases it) */
#define W25Q_32KB_BLOCK_ERASE_CMD 					0x52U	/* sets all 32Kbyte block with 0xFF */
#define W25Q_64KB_BLOCK_ERASE_CMD 					0xD8U	/* sets all 64Kbyte block with 0xFF */
#define W25Q_CHIP_ERASE_CMD 						0xC7U	/* fill all the chip with 0xFF */

#define W25Q_READ_SR1_CMD 							0x05U	/* read status-register 1 */
#define W25Q_WRITE_SR1_CMD 							0x01U	/* write status-register 1 (8.2.5) */
#define W25Q_READ_SR2_CMD 							0x35U	/* read status-register 2 */
#define W25Q_WRITE_SR2_CMD 							0x31U	/* write status-register 2 (8.2.5) */
#define W25Q_READ_SR3_CMD 							0x15U	/* read ststus-register 3 */
#define W25Q_WRITE_SR3_CMD 							0x11U	/* write status-register 3 (8.2.5) */

#define W25Q_READ_SFDP_CMD 							0x5AU	/* read SFDP register parameters */
#define W25Q_ERASE_SECURITY_REG_CMD					0x44U	/* erase security registers */
#define W25Q_PROG_SECURITY_REG_CMD 					0x42U	/* program security registers */
#define W25Q_READ_SECURITY_REG_CMD 					0x48U	/* read security registers */

#define W25Q_GLOBAL_LOCK_CMD 						0x7EU	/* global read-only protection enable */
#define W25Q_GLOBAL_UNLOCK_CMD 						0x98U	/* global read-only protection disable */
#define W25Q_READ_BLOCK_LOCK_CMD 					0x3DU	/* check block/sector protection */
#define W25Q_IND_BLOCK_LOCK_CMD 					0x36U	/* make block/sector read-only */
#define W25Q_IND_BLOCK_UNLOCK_CMD 					0x39U	/* disable block/sector protection */

#define W25Q_ERASEPROG_SUSPEND_CMD					0x75U	/* suspend erase/program operation (can be applied only when SUS=0, BYSY=1) */
#define W25Q_ERASEPROG_RESUME_CMD 					0x7AU	/* resume erase/program operation (if SUS=1, BUSY=0) */
#define W25Q_POWERDOWN_CMD 							0xB9U	/* powers down the chip (power-up by reading ID) */

#define W25Q_ENABLE_RST_CMD 						0x66U	/* enable software-reset ability */
#define W25Q_RESET_CMD 								0x99U	/* make software reset */

#define W25Q_FAST_READ_DUAL_OUT_CMD 				0x3BU	/* fast read in dual-SPI OUTPUT (8.2.14) */
#define W25Q_FAST_READ_DUAL_IO_CMD 					0xBBU	/* fast read in dual-SPI I/O (address transmits by both lines) */
#define W25Q_FULLID_DUAL_IO_CMD 					0x92U	/* read Manufacturer ID & Device ID by dual I/O */
#define W25Q_PAGE_PROGRAM_QUAD_INP_CMD 				0x32U	/* program page (256bytes) by Quad SPI lines */
#define W25Q_FAST_READ_QUAD_OUT_CMD 				0x6BU	/* fast read in quad-SPI OUTPUT (8.2.16) */
#define W25Q_FULLID_QUAD_IO_CMD 					0x94U	/* read Manufacturer ID & Device ID by Quad I/O */
#define W25Q_FAST_READ_QUAD_IO_CMD 					0xEBU	/* fast read in quad-SPI I/O (address transmits by quad lines) */
#define W25Q_SET_BURST_WRAP_CMD 					0x77U	/* use with quad-I/O (8.2.22) */

/*
 *  MEMORY REGISTERS
 */

#define W25Q_SR_Quad_Enable							((uint8_t)0x02)    /*!< Quad Enable */
#define W25Q_SR_DRV1                     			((uint8_t)0x9f)    /*!< DRV1:2 bit */

#ifdef __cplusplus
}
#endif

#endif /* __W25Q64_H_ */
