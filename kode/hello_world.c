/*
 * hello_world.c - kirim teks ke komputer lewat UART.
 *
 * Salin tiap blok ke penanda USER CODE yang sama di Core/Src/main.c.
 * Ganti huart1 sesuai USART board (huart2 untuk Nucleo, dst).
 *
 * Baca di komputer, 115200 baud:
 *   Windows : PuTTY -> Serial, COM3, 115200
 *   Linux   : picocom -b 115200 /dev/ttyACM0
 */

/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* USER CODE BEGIN 0 */
/* printf memanggil fungsi ini untuk tiap huruf. Isinya kosong secara bawaan,
 * jadi kita definisikan sendiri supaya hurufnya keluar lewat UART. */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END 0 */

/* USER CODE BEGIN WHILE */
  printf("\r\nHello World!\r\n");
  printf("Clock: %lu Hz\r\n\r\n", HAL_RCC_GetSysClockFreq());

  uint32_t hitung = 0;

  while (1)
  {
    printf("Hello World ke-%lu\r\n", (unsigned long)(++hitung));
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
