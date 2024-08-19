#include "main.h"
#include <string.h>

/*********************************************************/

#define FMC_BANK1_REG ((uint16_t *) 0x60000000)
#define FMC_BANK1_MEM ((uint16_t *) 0x60200000)

/*********************************************************/

static inline void
display_reset_low (void)
{
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
}

static inline void
display_reset_high (void)
{
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
}

/*********************************************************/

static inline void
display_send_command (uint8_t cmd)
{
  *FMC_BANK1_REG = cmd;
  __DSB();
}

static inline void
display_send_data (uint8_t data)
{
  *FMC_BANK1_MEM = data;
  __DSB();
}

/*********************************************************/

static void
display_column_address_set (uint16_t sc,
                            uint16_t ec)
{
  uint8_t data;

  display_send_command(0x2A);
  data = (sc>>8) & 0xff;
  display_send_data(data);
  data = sc & 0xff;
  display_send_data(data);
  data = (ec>>8) & 0xff;
  display_send_data(data);
  data = ec & 0xff;
  display_send_data(data);
}

static void
display_page_address_set (uint16_t sp,
                          uint16_t ep)
{
  uint8_t data;

  display_send_command(0x2B);
  data = (sp>>8) & 0xff;
  display_send_data(data);
  data = sp & 0xff;
  display_send_data(data);
  data = (ep>>8) & 0xff;
  display_send_data(data);
  data = ep & 0xff;
  display_send_data(data);
}

/*********************************************************/

void
display_update (uint16_t  *frameBuffer,
                uint16_t   dispWidth,
                uint16_t   dispHeight,
                int16_t    rectX,
                int16_t    rectY,
                int16_t    rectWidth,
                int16_t    rectHeight)
{
  uint32_t height;
  __IO uint16_t* ptr;

  display_column_address_set (rectX, rectX + rectWidth - 1);
  display_page_address_set (rectY, rectY + rectHeight - 1);

  /* this part should be accelerated using DMA hardware */
  display_send_command(0x2C);
  for (height = 0; height < rectHeight ; height++)
    {
      ptr = frameBuffer + rectX + (height + rectY) * dispWidth;
      for (uint32_t cnt = 0; cnt < rectWidth; cnt++)
        {
          display_send_data((*ptr)>>8);
          display_send_data(*ptr);
          ptr++;
        }
    }
}

/*********************************************************/

void
display_init (void)
{
  /* display reset */
  display_reset_high();
  HAL_Delay(100);
  display_reset_low();
  HAL_Delay(100);
  display_reset_high();
  HAL_Delay(120);

  /* set p-gamma */
  display_send_command(0xE0);
  display_send_data(0x00);
  display_send_data(0x10);
  display_send_data(0x14);
  display_send_data(0x01);
  display_send_data(0x0E);
  display_send_data(0x04);
  display_send_data(0x33);
  display_send_data(0x56);
  display_send_data(0x48);
  display_send_data(0x03);
  display_send_data(0x0C);
  display_send_data(0x0B);
  display_send_data(0x2B);
  display_send_data(0x34);
  display_send_data(0x0F);

  /* set n-gamma */
  display_send_command(0XE1);
  display_send_data(0x00);
  display_send_data(0x12);
  display_send_data(0x18);
  display_send_data(0x05);
  display_send_data(0x12);
  display_send_data(0x06);
  display_send_data(0x40);
  display_send_data(0x34);
  display_send_data(0x57);
  display_send_data(0x06);
  display_send_data(0x10);
  display_send_data(0x0C);
  display_send_data(0x3B);
  display_send_data(0x3F);
  display_send_data(0x0F);

  /* set power control 1 */
  display_send_command(0XC0);
  display_send_data(0x0F);
  display_send_data(0x0C);

  /* set power control 2 */
  display_send_command(0xC1);
  display_send_data(0x41);

  /* set VCOM control */
  display_send_command(0xC5);
  display_send_data(0x00);
  display_send_data(0x25);
  display_send_data(0x80);

  /* memory access */
  display_send_command(0x36);
  display_send_data(0x7A);

  /* pixel format */
  display_send_command(0x3A);
  display_send_data(0x55);

  /* interface mode control */
  display_send_command(0xB0);
  display_send_data(0x00);

  /* frame rate control */
  display_send_command(0xB1);
  display_send_data(0xA0);

  /* display inversion color */
  display_send_command(0xB4);
  display_send_data(0x02);

  /* display function control */
  display_send_command(0xB6);
  display_send_data(0x02);
  display_send_data(0x22);

  /* display inversion on */
  display_send_command(0x21);

  /* sleep out */
  display_send_command(0x11);
  HAL_Delay(120);

  /* tearing effect line on */
  display_send_command(0x35);
  display_send_data(0x00);

  /* display on */
  display_send_command(0x29);
}

/*********************************************************/
