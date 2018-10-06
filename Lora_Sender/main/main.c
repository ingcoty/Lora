#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lora.h"
#include <string.h>

void task_tx(void *p)
{  char buff[30] = {0};
   unsigned int pkt = 0;
   gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
   for(;;) {
	  sprintf(buff, "Hello packet %d", pkt++);
	  vTaskDelay(pdMS_TO_TICKS(1000));
	  gpio_set_level(GPIO_NUM_2, 1);
      lora_send_packet((uint8_t*)buff, strlen(buff));
      printf("packet sent...\n");
      gpio_set_level(GPIO_NUM_2, 0);
   }
}

void app_main()
{
   lora_init();
   lora_set_frequency(433e6);
   lora_enable_crc();
   xTaskCreate(&task_tx, "task_tx", 2048, NULL, 5, NULL);
}
