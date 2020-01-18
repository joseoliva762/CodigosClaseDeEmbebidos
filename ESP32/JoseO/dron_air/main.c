#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "string.h"
#include "math.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "nvs_flash.h"

//libreria para ADC
#include <driver/gpio.h>
#include <driver/adc.h>

#define WIFI_SSID "Root"
#define WIFI_PASS "3S7nWLNm"
#define True 1
#define False 0

// Definimos los puertos ADC.
#define ADC_WIDTH ADC_WIDTH_BIT_10
#define ADC_ATTEN ADC_ATTEN_DB_11
// PAra el MQ7 Analogo.
#define ADC_GPIO_MQ7 35
#define ADC_CHANNEL_MQ7 ADC1_CHANNEL_7
// PAra el MQ4 Analogo.
#define ADC_GPIO_MQ4 32
#define ADC_CHANNEL_MQ4 ADC1_CHANNEL_4
// PAra el MQ131 Analogo.
#define ADC_GPIO_MQ131 33
#define ADC_CHANNEL_MQ131 ADC1_CHANNEL_5
// PAra el MG811 Analogo.
#define ADC_GPIO_MG811 34
#define ADC_CHANNEL_MG811 ADC1_CHANNEL_6
#define ANALOG_SAMPLING_TIMES  50   //sampling time in one reading process
#define ANALOG_SAMPLING_INTERVEL 5  //sampling interval of the two samples

xSemaphoreHandle wifi_on;
xSemaphoreHandle MQ7_Run;
xSemaphoreHandle MQ4_Run;
xSemaphoreHandle MQ131_Run;
xSemaphoreHandle MG811_Run;
xQueueHandle sendinfo;
#define queueTam 10

int sock = 0, tiempo_de_respuesta = 3000;
//struck send
typedef struct{
    unsigned char id[50];
    unsigned char type[50];
    float data;
}packet;

void delay_ms(int ms){
	vTaskDelay(ms/portTICK_PERIOD_MS);
}

void Sendfun(packet *req){
	send(sock, (const void*)&req, sizeof(req), 0);
}

float constrain(float porcentaje, float lim_inf, float lim_sup){
	if(porcentaje < lim_inf) return lim_inf;
	else if(porcentaje > lim_sup) return porcentaje; //return lim_sup;
	else return porcentaje;
}

void send_queue(packet *req){
	if(xQueueSend(sendinfo, (const void*)req, 0))printf(" "); else printf("Err send Queue. \r\n");
}

// Wifi event handler
static esp_err_t event_handler(void *ctx, system_event_t *event){
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
////////////////////////////////////////////////////////////////////////////////////////////////////77
void MQ7_task(){
	packet request;
	float voltaje = 0, ADC = 0, Rs = 0;
	memcpy(request.id, "Dron Station No1.", sizeof("Dron Station No1."));
	xSemaphoreTake(MQ7_Run, portMAX_DELAY);
	// Activacion de tareas..
	xSemaphoreGive(MQ4_Run);
	printf("MQ7 Activo    [OK]... \r\n");
	while(True){
		memcpy(request.type, "MQ-7", sizeof("MQ-7"));
		ADC = adc1_get_raw(ADC_CHANNEL_MQ7);
		voltaje = ADC * (5.0 / 1023.0);
		Rs = 1000*((5-voltaje)/voltaje);
		request.data = (0.4091) * (pow(Rs/5463, -1.497));
		send_queue(&request);
		delay_ms(tiempo_de_respuesta);
	}
}

void MQ4_task(){
	int promedio_de_lectura = 300, contador = 0;
	packet request;
	float voltaje = 0, ADC = 0, Rs = 0, Ro = 0, ratio = 0, ppm  = 0;
	memcpy(request.id, "Dron Station No1.", sizeof("Dron Station No1."));
	xSemaphoreTake(MQ4_Run, portMAX_DELAY);
	// Activacion de tareas..
	xSemaphoreGive(MQ131_Run);
	printf("MQ4 Activo    [OK]... \r\n");
	while(True){
		memcpy(request.type, "MQ-4", sizeof("MQ-4"));
		for(contador = 0; contador <= promedio_de_lectura; contador++){
			ADC += adc1_get_raw(ADC_CHANNEL_MQ4);
		}
		ADC /= promedio_de_lectura;
		voltaje = ADC * (5.0 / 1023.0);
		Rs = ((5-voltaje)/voltaje);//1000*((5-voltaje)/voltaje);
		Ro = Rs/26+(1/3);
		ratio = Rs/Ro;
		ppm = 100* pow(log10(40)/log10(0.09), ratio);
		request.data = voltaje;//ppm;//(0.4091) * (pow(Rs/5463, -1.497));
		send_queue(&request);
		delay_ms(tiempo_de_respuesta);
	}
}

void MQ131_task(){
	packet request;
	float voltaje = 0, ADC = 0, Rs = 0, Rl = 4.5;
	int promedio_de_lectura = 300, contador = 0;
	memcpy(request.id, "Dron Station No1.", sizeof("Dron Station No1."));
	xSemaphoreTake(MQ131_Run, portMAX_DELAY);
	// Activacion de tareas..
	xSemaphoreGive(MG811_Run);
	printf("MQ131 Activo    [OK]... \r\n");
	while(True){
		memcpy(request.type, "MQ-131", sizeof("MQ-131"));
		for(contador = 0; contador <= promedio_de_lectura; contador++){
			ADC += adc1_get_raw(ADC_CHANNEL_MQ131);
		}
		ADC /= promedio_de_lectura;
		voltaje = ADC * (5.0 / 1023.0);
		Rs = (5/voltaje-1)*Rl;
		request.data = pow(5, 2)*(Rs/pow((Rs+ Rl),2));
		send_queue(&request);
		delay_ms(tiempo_de_respuesta);
	}
}

void MG811_task(){
	packet request;
	float voltaje = 0, ADC = 0, percentageCO2Value;
	const float VoltageGain = 8.5;       //Voltage Gain of the circus
	int promedio_de_lectura = 300, contador = 0;
	memcpy(request.id, "Dron Station No1.", sizeof("Dron Station No1."));
	xSemaphoreTake(MG811_Run, portMAX_DELAY);
	printf("MG811 Activo    [OK]... \r\n");
	while(True){
		memcpy(request.type, "MG-811", sizeof("MG-811"));
		for(contador = 0; contador < ANALOG_SAMPLING_TIMES; contador++){
			ADC += adc1_get_raw(ADC_CHANNEL_MG811);
			delay_ms(ANALOG_SAMPLING_INTERVEL);
		}
		ADC /= promedio_de_lectura;
		voltaje = ADC*5.0/(1024.0*ANALOG_SAMPLING_TIMES);
		voltaje *= (1000.0/VoltageGain);
		percentageCO2Value = 25784989641.0468*pow(2.718281828459045, -0.0558861525*voltaje);
		percentageCO2Value = constrain(percentageCO2Value* pow(10, -6), 350, 10000);
		request.data = percentageCO2Value;
		send_queue(&request);
		delay_ms(tiempo_de_respuesta);
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////77
// -- configuracion de los Puertos analogos.
static void adc_gpioInit(){
	//MQ7
	gpio_set_direction(ADC_GPIO_MQ7, GPIO_MODE_INPUT);
	//MQ4
	gpio_set_direction(ADC_GPIO_MQ4, GPIO_MODE_INPUT);
	//MQ131
	gpio_set_direction(ADC_GPIO_MQ131, GPIO_MODE_INPUT);
	//MG811
	gpio_set_direction(ADC_GPIO_MG811, GPIO_MODE_INPUT);
}
static void adc_initConfig(){
	//MQ7 está en el channel 7 pin 35
	adc1_config_width(ADC_WIDTH);
	adc1_config_channel_atten(ADC_CHANNEL_MQ7, ADC_ATTEN);
	//MQ4 está en el channel 4 pin 32
	adc1_config_channel_atten(ADC_CHANNEL_MQ4, ADC_ATTEN);
	//MQ131 está en el channel 5 pin 33
	adc1_config_channel_atten(ADC_CHANNEL_MQ131, ADC_ATTEN);
	//Mg811 está en el channel 5 pin 34
	adc1_config_channel_atten(ADC_CHANNEL_MG811, ADC_ATTEN);
}

static void adcConfig(){
	//1.. Inicializamos el GPIO del ADC.
	adc_gpioInit();
	//2.. Configuramos el ADC.
	adc_initConfig();
}

// Main task
void main_task(){
	int res;
	adcConfig();   // Configuramos el puerto ADC.

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
	memcpy(request.id, "Dron Station No1.", sizeof("Dron Station No1."));
	memcpy(request.type , "Dron", sizeof("Dron"));
	request.data = 0;
	//Sendfun((void)request);
	send(sock, (const void*)&request, sizeof(request), 0);
	// Activacion de tareas..
	xSemaphoreGive(MQ7_Run);
	printf("Id: %s, Type: %s, data: %.2f.\r\n", request.id,request.type, request.data);
	//send(sock, (const void*)&request, sizeof(request), 0);
	while(1){
		xQueueReceive(sendinfo, &request , portMAX_DELAY);
		//Sendfun((void)request);
		printf("Id: %s, Type: %s, data: %.2f.\r\n", request.id,request.type, request.data);
		send(sock, (const void*)&request, sizeof(request), 0);
		delay_ms(tiempo_de_respuesta/4);
	}
}


// Main application
void app_main(){
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
	sendinfo = xQueueCreate(queueTam, sizeof(packet));
	MQ7_Run = xSemaphoreCreateBinary();
	MQ4_Run = xSemaphoreCreateBinary();
	MQ131_Run = xSemaphoreCreateBinary();
	MG811_Run = xSemaphoreCreateBinary();
	//Tarea principal
	xTaskCreate(MQ7_task, "MQ7_task", 4096, NULL, 6, NULL);
	xTaskCreate(MQ4_task, "MQ4_task", 4096, NULL, 6, NULL);
	xTaskCreate(MQ131_task, "MQ131_task", 4096, NULL, 6, NULL);
	xTaskCreate(MG811_task, "MG811_task", 4096, NULL, 6, NULL);
    xTaskCreate(main_task, "main_task", 20000, NULL, 6, NULL);
}
