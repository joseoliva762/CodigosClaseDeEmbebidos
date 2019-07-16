#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "string.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
/*
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
*/
#include "nvs_flash.h"

#define WIFI_SSID "linux"
#define WIFI_PASS "s5F1iqZM"


xSemaphoreHandle wifi_on;

// Wifi event handler
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	printf("Eventos de Wifi %d\r\n", event->event_id);
    switch(event->event_id){

       case SYSTEM_EVENT_STA_START:
           esp_wifi_connect();
           break;

	   case SYSTEM_EVENT_STA_GOT_IP:
          xSemaphoreGive(wifi_on);
          break;

	   default:
          break;
    }

	return ESP_OK;
}


// Main task
void main_task()
{
	//int sock = 0;
	//int res = 0;
    //char request[] = "Hola desde ESP32\r\n";

	printf("Esperando Conexion\r\n");
	xSemaphoreTake(wifi_on, portMAX_DELAY);
	printf("ESP32 Conectado  [OK] \r\n");
	while(1){
		vTaskDelay(500);
	}
}


// Main application
void app_main()
{
	//Deshabilitamos log eventos wifi
	esp_log_level_set("wifi", ESP_LOG_NONE);

	//Inicialización NVS (Non Volatile Storage)
	ESP_ERROR_CHECK(nvs_flash_init());

	//Inicialización de Stack TCP
	//tcpip_adapter_init();

	//Inicialización de Driver WiFi
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	//Inicialización WiFi
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); //Selección de modo Station

	//Configurar SSID y PASS
	wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

	//Creacion Semaphoro Binario
	wifi_on = xSemaphoreCreateBinary();

	//Tarea principal
    xTaskCreate(main_task, "main_task", 20000, NULL, 5, NULL);
}
