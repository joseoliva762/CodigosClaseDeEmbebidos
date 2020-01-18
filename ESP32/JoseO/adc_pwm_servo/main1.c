#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "string.h"

//libreria para ADC
#include <driver/gpio.h>
#include <driver/adc.h>

//librerias para mcpwm.
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#define TRUE 1
#define FALSE 0

// Puerto PWM
#define GPIO_MCPWM_0A 15 //seteamos el puerto 15 como pwm0A
#define MCPWM_UNIT MCPWM_UNIT_0 //definimos la unidad.
#define MCPWM_IO_SIGNAL MCPWM0A //señal mcpwm_io_signal_t
#define DUTY_MODE MCPWM_DUTY_MODE_0 //modo ALTO
#define MCPWM_TIMER MCPWM_TIMER_0 //config del timer
#define MCPWM_OPERATOR MCPWM0A //el numero es igual al numero del timer.

// Puerto ADC
#define ADC_GPIO 35
#define ADC_WIDTH ADC_WIDTH_BIT_10
#define ADC_CHANNEL ADC1_CHANNEL_7
#define ADC_ATTEN ADC_ATTEN_DB_11

SemaphoreHandle_t runPotData;
xQueueHandle potDataQueue;
#define queueTam 10

void delay_ms(int ms){
	vTaskDelay(ms/portTICK_PERIOD_MS);
}

// MCPWM CONFIG *******************************************************************************************************************************
static void mcpwm_gpioInit(){
	printf("Inicializando puertos PWM...\r\n");
	if(mcpwm_gpio_init(MCPWM_UNIT, MCPWM_IO_SIGNAL, GPIO_MCPWM_0A ) != ESP_OK) printf("Error en la inicializacion del puerto\r\n");
	else printf("\tpuerto %d inicializado como pwm.\r\n",GPIO_MCPWM_0A);
}
static void mcpwm_initConfig(){
	printf("Configuracion inicial de parametros..\r\n");
	mcpwm_config_t pwmConfig;
	pwmConfig.frequency = 50;
	pwmConfig.cmpr_a = 0;
	pwmConfig.duty_mode = DUTY_MODE;
	pwmConfig.counter_mode = MCPWM_UP_COUNTER;


	if(mcpwm_init(MCPWM_UNIT,MCPWM_TIMER, &pwmConfig)!= ESP_OK) printf("Error configracion inicial\r\n");
	else printf("\tconfiguracion inicial EXITOSA.\r\n");
}
static void set_dutyCicle(float dutyCicle){
	mcpwm_set_duty(MCPWM_UNIT, MCPWM_TIMER, MCPWM_OPERATOR, dutyCicle);
	printf("Nivel de luminosodad %.2f porciento.\r\n",dutyCicle);
}
static void mcpwmConfig(){
	//1.. Inicializamos el GPIO del PWM.
	mcpwm_gpioInit();
	//2.. Configuracion inicial del mcpwm.
	mcpwm_initConfig();
}



// ADC CONFIG *******************************************************************************************************************************
static void adc_gpioInit(){
	gpio_set_direction(ADC_GPIO, GPIO_MODE_INPUT);
}
static void adc_initConfig(){
	adc1_config_width(ADC_WIDTH);
	adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);
}
static void adcConfig(){
	//1.. Inicializamos el GPIO del ADC.
	adc_gpioInit();
	//2.. Configuramos el ADC.
	adc_initConfig();
}

void mainTask_config(){
	float dutyCicle = 0;
	float potData = 0;
	mcpwmConfig(); // Configuracomo los puertos PWM.
	adcConfig();   // Configuramos el puerto ADC.
	xSemaphoreGive(runPotData);
	printf("ADC y PWM Configurado Exitosamente... \r\n");
	while(TRUE){
		xQueueReceive(potDataQueue, &potData , portMAX_DELAY);
		dutyCicle = (potData*100)/1023;
		set_dutyCicle(dutyCicle);
		delay_ms(50);
	}
}
void get_potData(){
	float potData = 0; //datos de 0-1023 en el puerto 35.
	xSemaphoreTake(runPotData,portMAX_DELAY);xSemaphoreTake(runPotData,portMAX_DELAY);

	printf("Potenciometro funcionando... \r\n");
	while(TRUE){
		potData = adc1_get_raw(ADC_CHANNEL);
		if(xQueueSend(potDataQueue, &potData, 0))printf(" "); else printf("Err send Queue. \r\n");
		delay_ms(50);
	}
}
void app_main(){
	runPotData = xSemaphoreCreateBinary();
	potDataQueue = xQueueCreate(queueTam, sizeof(float));
	printf("ADC y PWM... \n");
	xTaskCreate(get_potData, "get_potData", 4096, NULL, 6, NULL);
	xTaskCreate(mainTask_config, "mainTask_config", 4096, NULL, 5, NULL);

}
