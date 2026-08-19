/*
 * blink.c - LED berkedip.
 *
 * Salin tiap blok ke penanda USER CODE yang sama di Core/Src/main.c.
 * Ganti LED_GREEN sesuai nama pin di Core/Inc/main.h board masing-masing.
 */

/* USER CODE BEGIN PD */
#define JEDA_MS  500u
/* USER CODE END PD */

/* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_Delay(JEDA_MS);

    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    HAL_Delay(JEDA_MS);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
