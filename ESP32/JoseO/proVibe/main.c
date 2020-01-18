#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "string.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "nvs_flash.h"

#define WIFI_SSID "Root"
#define WIFI_PASS "3S7nWLNm"
#define Zone "Asbama"
#define Id "1103120463"
#define Type "vibracion"
#define Data "Se registran Vibraciones"
#define sensorData 21
#define SensorUpLed 2
#define ON 1
#define OFF 0

xSemaphoreHandle wifi_on;
xSemaphoreHandle vibe_on;
xSemaphoreHandle active;
xQueueHandle state;

//struck send
typedef struct{
    unsigned char zone[30];
    unsigned char id[12];
    unsigned char type[10];
    unsigned char data[100];
}packet;

void delay_ms(int ms){
	vTaskDelay(ms/portTICK_PERIOD_MS);
}

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
          xSemaphoreGive(active);
          break;

	   default:
          break;
    }

	return ESP_OK;
}
//vibe Task
void vibe_task(){
	char level = 1;
	packet datos;
	memcpy(datos.zone, Zone, sizeof(Zone));
	memcpy(datos.id, Id, sizeof(Id));
	memcpy(datos.type , Type, sizeof(Type));
	memcpy(datos.data, Data, sizeof(Data));
	xSemaphoreTake(active, portMAX_DELAY);
	while(1){
		level = gpio_get_level(sensorData);
		if(level == 0){
			//xQueueSend(state, &level, 0);
			xSemaphoreGive(vibe_on);
			printf("No se registro %s, en la finca %s, en el aspersor: %s \r\n", datos.type, datos.zone,  datos.id);
		}else{
			gpio_set_level(SensorUpLed,OFF);
			printf("N0\r\n");
		}
		delay_ms(12);
	}
}

// Main task
void main_task(){
	int sock = 0;
	int res = 0;
	//char level = 0;
    //char request[] = "Main Task [ON]";
	printf("Esperando Conexion\r\n");
	xSemaphoreTake(wifi_on, portMAX_DELAY);
	printf("ESP32 Conectado  [OK] \r\n");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("10.42.0.1");
	addr.sin_port = htons(3001);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));

	packet request;

	memcpy(request.zone, Zone, sizeof(Zone));
	memcpy(request.id, Id, sizeof(Id));
	memcpy(request.type , Type, sizeof(Type));
	memcpy(request.data, Data, sizeof(Data));
	send(sock, (const void*)&request, sizeof(request), 0);
	while(1){
		xSemaphoreTake(vibe_on, portMAX_DELAY);
		send(sock, (const void*)&request, sizeof(request), 0);
		gpio_set_level(SensorUpLed,ON);
		delay_ms(13);
	}
}


// Main application
void app_main(){
	//pin del sensor
	gpio_set_direction(sensorData,GPIO_MODE_INPUT);
	gpio_set_direction(SensorUpLed,GPIO_MODE_OUTPUT);

	//Deshabilitamos log eventos wifi
	esp_log_level_set("wifi", ESP_LOG_NONE);

	//Inicialización NVS (Non Volatile Storage)
	ESP_ERROR_CHECK(nvs_flash_init());

	//Inicialización de Stack TCP
	tcpip_adapter_init();

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
	vibe_on = xSemaphoreCreateBinary();
	active = xSemaphoreCreateBinary();
	state = xQueueCreate(4, sizeof(char));

	//Tarea principal
    xTaskCreate(main_task, "main_task", 20000, NULL, 6, NULL);
    xTaskCreate(vibe_task, "vibe_task", 20000, NULL, 7, NULL);
}
