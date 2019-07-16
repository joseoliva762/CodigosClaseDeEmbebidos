#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_now.h"

#define CHANNEL_ESPNOW 1

xSemaphoreHandle wifi_on;

static esp_err_t event_handler(void *ctx, system_event_t *event){

	printf("Evento de wifi %d\r\n", event->event_id);
	switch(event->event_id){
	case SYSTEM_EVENT_STA_START:
		printf("Wifi OK\r\n");
		xSemaphoreGive(wifi_on);
		break;
	default:
		break;
	}
	return(ESP_OK);
}

static void receive_callback (const uint8_t *mac_addr, const uint8_t *data, int data_len){

	char buff[20] = {0};
	sprintf(buff, MACSTR, MAC2STR(mac_addr));
	printf("mensaje recibido desde %s mesaje %s\r\n", buff, data);

}

static void send_callback (const uint8_t *mac_addr, esp_now_send_status_t status){
	printf("Test Send Callback\r\n");
}

void main_task(){

	unsigned char mac_broadcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned char msg[] = "Hola Embebidos - Carlos\r\n";

	xSemaphoreTake(wifi_on, portMAX_DELAY);
	printf("Listo para configurar ESP NOW\r\n");
	esp_now_init();
	esp_now_register_recv_cb(receive_callback);
	esp_now_register_send_cb(send_callback);

	esp_now_peer_info_t *peer = NULL;
	peer = malloc(sizeof(esp_now_peer_info_t));
	memset(peer, 0, sizeof(esp_now_peer_info_t));

	peer->channel = CHANNEL_ESPNOW;
	peer->ifidx = ESP_IF_WIFI_STA;
	peer->encrypt = 0;
	memcpy(peer->peer_addr, mac_broadcast, sizeof(mac_broadcast));

	if(esp_now_add_peer(peer) != ESP_OK) {
		printf("Error al agregar peer\r\n");
		return(0);
	}
	free(peer);
	printf("peer agregado OK\r\n");

	while(1){
		if(esp_now_send(mac_broadcast, msg, sizeof(msg)) == ESP_OK){
			printf("Mensaje Enviado OK\r\n");
		}
		else{
			printf("Error al enviar Mensaje\r\n");
		}
		vTaskDelay(1000);
	}

}


void app_main()
{
	wifi_on = xSemaphoreCreateBinary();
	nvs_flash_init();
	tcpip_adapter_init();
	esp_event_loop_init(event_handler, NULL);

	//Configuracion Interfaz WiFi
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&wifi_init_config);
	esp_wifi_set_storage(WIFI_STORAGE_RAM);
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_start();
	esp_wifi_set_channel(CHANNEL_ESPNOW, 0);

	xTaskCreate(&main_task, "main_task", 2048, NULL, 5, NULL);

}

