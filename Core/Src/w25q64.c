#include "main.h"
#include "w25q64.h"

/*
 * W25Q64_Init()
 */
#if 0
HAL_StatusTypeDef
W25Q64_Init (XSPI_HandleTypeDef *xspi)
{
  if (HAL_XSPI_DeInit(xspi) != HAL_OK)
    return HAL_ERROR;

  MX_OCTOSPI1_Init();

  if (W25Q64_ResetChip(xspi) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_Configuration(xspi) != HAL_OK)
    return HAL_ERROR;

  HAL_Delay(1);

  if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_WriteEnable(xspi) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}
#endif


/*
 * W25Q64_ResetChip()
 */
HAL_StatusTypeDef
W25Q64_ResetChip (XSPI_HandleTypeDef *xspi)
{
  XSPI_RegularCmdTypeDef cmd = {0};

  /* Enable Reset */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_ENABLE_RST_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_NONE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 0;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  /* Reset Device */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_RESET_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_NONE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 0;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_Configuration()
 * Enable Quad Mode & Set Dummy Cycles Count
 *
 */
HAL_StatusTypeDef
W25Q64_Configuration (XSPI_HandleTypeDef *xspi)
{
  uint8_t reg1, reg2, reg3;
  uint8_t w_reg1, w_reg2, w_reg3;

  reg1 = 0;
  reg2 = 0;
  reg3 = 0;

  w_reg1 = 0;
  w_reg2 = 0;
  w_reg3 = 0;

  if (W25Q64_Read_Status_Registers(xspi, &reg1, 1) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_Read_Status_Registers(xspi, &reg2, 2) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_Read_Status_Registers(xspi, &reg3, 3) != HAL_OK)
    return HAL_ERROR;

  w_reg1 = reg1;
  w_reg2 = reg2 | W25Q_SR_Quad_Enable;
  w_reg3 = (reg3 & W25Q_SR_DRV1);

  if (W25Q64_Write_Status_Registers(xspi, w_reg1, 1) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_Write_Status_Registers(xspi, w_reg2, 2) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_Write_Status_Registers(xspi, w_reg3, 3) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_WriteEnable()
 */
HAL_StatusTypeDef
W25Q64_WriteEnable (XSPI_HandleTypeDef *xspi)
{
  XSPI_RegularCmdTypeDef cmd;
  XSPI_AutoPollingTypeDef cfg;

  /* Enable write operations */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_WRITE_ENABLE_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_NONE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 0;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  /* Common Commands*/
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_READ_SR1_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_1_LINE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 1;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  /* Configure automatic polling mode to wait for write enabling */
  cfg.MatchValue    = 0x02U;
  cfg.MatchMask     = 0x02U;
  cfg.MatchMode     = HAL_XSPI_MATCH_MODE_AND;
  cfg.IntervalTime  = W25Q_AUTOPOLLING_INTERVAL_TIME;
  cfg.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_AutoPolling (xspi,&cfg,HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_AutoPollingMemReady()
 */
HAL_StatusTypeDef
W25Q64_AutoPollingMemReady (XSPI_HandleTypeDef *xspi)
{
  XSPI_RegularCmdTypeDef cmd;
  XSPI_AutoPollingTypeDef cfg;

  /* Configure automatic polling mode to wait for memory ready */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_READ_SR1_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_1_LINE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 1;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  cfg.MatchValue    = 0x00U;
  cfg.MatchMask     = 0x01U;
  cfg.MatchMode     = HAL_XSPI_MATCH_MODE_AND;
  cfg.IntervalTime  = W25Q_AUTOPOLLING_INTERVAL_TIME;
  cfg.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_XSPI_AutoPolling (xspi,&cfg,HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_Erase_Chip()
 */
HAL_StatusTypeDef
W25Q64_Erase_Chip (XSPI_HandleTypeDef *xspi)
{
  XSPI_RegularCmdTypeDef cmd = {0};

  /* Erasing Sequence */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_CHIP_ERASE_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_NONE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 1;

  if (W25Q64_WriteEnable(xspi) != HAL_OK)
    return HAL_ERROR;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  while (W25Q64_IsBusy(xspi) == HAL_ERROR)
    HAL_Delay(1);

  if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_EraseSector()
 */
HAL_StatusTypeDef
W25Q64_EraseSector (XSPI_HandleTypeDef  *xspi,
                    uint32_t             start_addr,
                    uint32_t             end_addr)
{
  XSPI_RegularCmdTypeDef cmd = {0};

  start_addr = start_addr - start_addr % W25Q_SECTOR_SIZE;

  /* Erasing Sequence */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_SECTOR_ERASE_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_1_LINE;
  cmd.DataMode              = HAL_XSPI_DATA_NONE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 0;

  while (end_addr >= start_addr)
    {
      cmd.Address = (start_addr & 0x0FFFFFFF);

      if (W25Q64_WriteEnable(xspi) != HAL_OK)
        return HAL_ERROR;

      if (HAL_XSPI_Command (xspi,&cmd,HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        return HAL_ERROR;

      start_addr += W25Q_SECTOR_SIZE;

      if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
        return HAL_ERROR;
    }

  return HAL_OK;
}


/*
 * W25Q64_Write()
 */
HAL_StatusTypeDef
W25Q64_Write (XSPI_HandleTypeDef  *xspi,
              uint8_t             *data,
              uint32_t             addr,
              uint32_t             size)
{
  XSPI_RegularCmdTypeDef cmd = {0};
  uint32_t end_addr, current_size, current_addr, data_addr;

  end_addr = 0;
  current_size = 0;
  current_addr = 0;
  data_addr = 0;

  while (current_addr <= addr)
    current_addr += W25Q_PAGE_SIZE;

  current_size = current_addr - addr;

  /* Check if the size of the data is less than the place in the page */
  if (current_size > size)
    current_size = size;

  /* Initialize the address variables */
  current_addr = addr;
  end_addr = addr + size;
  data_addr = (uint32_t)data;

  /* Perform the write page by page */
  do
    {
      /* Initialize the program command */
      cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
      cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
      cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
      cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
      cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
      cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
      cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
      cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
      cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
      cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
      cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
      cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
      cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
      cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
      cmd.Instruction           = W25Q_PAGE_PROGRAM_QUAD_INP_CMD;
      cmd.AddressMode           = HAL_XSPI_ADDRESS_1_LINE;
      cmd.Address               = current_addr;
      cmd.DataMode              = HAL_XSPI_DATA_4_LINES;
      cmd.DummyCycles           = 0;
      cmd.DataLength            = current_size;

      if (current_size == 0)
        return HAL_OK;

      /* Enable write operations */
      if (W25Q64_WriteEnable(xspi) != HAL_OK)
        return HAL_ERROR;

      /* Configure the command */
      if (HAL_XSPI_Command (xspi,&cmd,HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        return HAL_ERROR;

      /* Transmission of the data */
      if (HAL_XSPI_Transmit (xspi, (uint8_t*)data_addr,
                             HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          return HAL_ERROR;
        }

      /* Configure automatic polling mode to wait for end of program */
      if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
        return HAL_ERROR;

      /* Update the address and size variables for next page programming */
      current_addr += current_size;
      data_addr += current_size;
      current_size = ((current_addr + W25Q_PAGE_SIZE) > end_addr) ? \
                      (end_addr - current_addr) : W25Q_PAGE_SIZE;

    } while (current_addr <= end_addr);

  return HAL_OK;
}


/*
 * W25Q64_Read()
 */
HAL_StatusTypeDef
W25Q64_Read (XSPI_HandleTypeDef  *xspi,
             uint8_t             *data,
             uint32_t             addr,
             uint32_t             size)
{
  XSPI_RegularCmdTypeDef cmd = {0};

  /* Initialize the read command */
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_FAST_READ_QUAD_IO_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_4_LINES;
  cmd.Address               = addr;
  cmd.DataMode              = HAL_XSPI_DATA_4_LINES;
  cmd.DummyCycles           = W25Q_DUMMY_CYCLES_READ_QUAD;
  cmd.DataLength            = size;

  /* Configure the command */
  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  /* Reception of the data */
  if (HAL_XSPI_Receive (xspi, data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_EnableMemoryMappedMode()
 */
HAL_StatusTypeDef
W25Q64_EnableMemoryMappedMode (XSPI_HandleTypeDef *xspi)
{
  XSPI_RegularCmdTypeDef cmd = {0};
  XSPI_MemoryMappedTypeDef mapped_cfg = {0};

  /* Enable Memory-Mapped mode */
  cmd.OperationType         = HAL_XSPI_OPTYPE_READ_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_FAST_READ_QUAD_IO_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_4_LINES;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_4_LINES;
  cmd.DummyCycles           = W25Q_DUMMY_CYCLES_READ_QUAD;
  cmd.DataLength            = 0;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  /* Initialize the program command */
  cmd.OperationType         = HAL_XSPI_OPTYPE_WRITE_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_PAGE_PROGRAM_QUAD_INP_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_1_LINE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_4_LINES;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 0;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  /* Initialize Memory Mapped Command */
  mapped_cfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_XSPI_MemoryMapped (xspi, &mapped_cfg) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_IsBusy()
 */
HAL_StatusTypeDef
W25Q64_IsBusy (XSPI_HandleTypeDef *xspi)
{
  HAL_StatusTypeDef state;
  uint8_t status_rgister = {0};

  state = W25Q64_Read_Status_Registers (xspi, &status_rgister, 1);
  if (state != HAL_OK)
    return state;

  status_rgister = status_rgister & 0b1;

  return status_rgister ? HAL_ERROR : HAL_OK;
}


/*
 * W25Q64_Read_Status_Registers()
 */
HAL_StatusTypeDef
W25Q64_Read_Status_Registers (XSPI_HandleTypeDef  *xspi,
                              uint8_t             *reg_data,
                              uint8_t              reg_num)
{
  XSPI_RegularCmdTypeDef cmd = {0};

  /* Common Commands*/
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = 0;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_1_LINE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 1;

  if (reg_num == 1)
    cmd.Instruction = W25Q_READ_SR1_CMD;
  else if (reg_num == 2)
    cmd.Instruction = W25Q_READ_SR2_CMD;
  else if (reg_num == 3)
    cmd.Instruction = W25Q_READ_SR3_CMD;
  else
    return HAL_ERROR;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  if(HAL_XSPI_Receive(xspi, reg_data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}


/*
 * W25Q64_Write_Status_Registers()
 */
HAL_StatusTypeDef
W25Q64_Write_Status_Registers (XSPI_HandleTypeDef  *xspi,
                               uint8_t              reg_data,
                               uint8_t              reg_num)
{
  XSPI_RegularCmdTypeDef cmd = {0};

  /* Common Commands*/
  cmd.OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG;
  cmd.IOSelect              = HAL_XSPI_SELECT_IO_3_0;
  cmd.InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  cmd.AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;
  cmd.DataDTRMode           = HAL_XSPI_DATA_DTR_DISABLE;
  cmd.DQSMode               = HAL_XSPI_DQS_DISABLE;
  cmd.SIOOMode              = HAL_XSPI_SIOO_INST_EVERY_CMD;
  cmd.AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytes        = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_NONE;
  cmd.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
  cmd.InstructionMode       = HAL_XSPI_INSTRUCTION_1_LINE;
  cmd.InstructionWidth      = HAL_XSPI_INSTRUCTION_8_BITS;
  cmd.AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;
  cmd.Instruction           = W25Q_WRITE_DISABLE_CMD;
  cmd.AddressMode           = HAL_XSPI_ADDRESS_NONE;
  cmd.Address               = 0;
  cmd.DataMode              = HAL_XSPI_DATA_NONE;
  cmd.DummyCycles           = 0;
  cmd.DataLength            = 0;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
    return HAL_ERROR;

  cmd.Instruction = W25Q_ENABLE_VOLATILE_SR_CMD;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
    return HAL_ERROR;

  if (reg_num == 1)
    cmd.Instruction = W25Q_WRITE_SR1_CMD;
  else if (reg_num == 2)
    cmd.Instruction = W25Q_WRITE_SR2_CMD;
  else if (reg_num == 3)
    cmd.Instruction = W25Q_WRITE_SR3_CMD;
  else
    return HAL_ERROR;

  cmd.DataMode   = HAL_XSPI_DATA_1_LINE;
  cmd.DataLength = 1;

  if (W25Q64_WriteEnable(xspi) != HAL_OK)
    return HAL_ERROR;

  if (HAL_XSPI_Command (xspi, &cmd, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  if(HAL_XSPI_Transmit(xspi,&reg_data,HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    return HAL_ERROR;

  if (W25Q64_AutoPollingMemReady(xspi) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}
