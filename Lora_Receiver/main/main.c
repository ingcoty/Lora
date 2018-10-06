#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lora.h"
#include "driver/gpio.h"

uint8_t buf[32];

void task_rx(void *p)
{
   int x;
   gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
   printf("receiver mode\r\n");
   for(;;) {
      lora_receive();    // put into receive mode
      while(lora_received()) {
    	 gpio_set_level(GPIO_NUM_2, 1);
         x = lora_receive_packet(buf, sizeof(buf));
         buf[x] = 0;
         printf("Received: %s rssi = %d and rs=%f\n", buf, lora_packet_rssi(),lora_packet_snr());
         lora_receive();
      }
      vTaskDelay(1);
      gpio_set_level(GPIO_NUM_2, 0);
   }
}

void app_main()
{
   lora_init();
   lora_set_frequency(433e6);
   lora_enable_crc();
   xTaskCreate(&task_rx, "task_rx", 2048, NULL, 5, NULL);
}
