#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "string.h"

//librerias para mcpwm.
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#define TRUE 1
#define FALSE 0

//puerto pwm
#define GPIO_MCPWM_0A 15 //seteamos el puerto 15 como pwm0A
#define MCPWM_UNIT MCPWM_UNIT_0 //definimos la unidad.
#define MCPWM_IO_SIGNAL MCPWM0A //señal mcpwm_io_signal_t
#define DUTY_MODE MCPWM_DUTY_MODE_0 //modo ALTO
#define MCPWM_TIMER MCPWM_TIMER_0 //config del timer
#define MCPWM_OPERATOR MCPWM0A //el numero es igual al numero del timer.

void delay_ms(int ms){
	vTaskDelay(ms/portTICK_PERIOD_MS);
}

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
	printf("Nivel de luminosodad %.2f porciento\r\n",dutyCicle);
}
static void mcpwmConfig(){
	//1.. Inicializamos el GPIO del PWM.
	mcpwm_gpioInit();
	//2.. configuracion inicial del mcpwm.
	mcpwm_initConfig();
}

static void mcpwmTest_mainTask(){
	float pwmCont = 0;
	mcpwmConfig(); //configuracomo los puertos PWM.
	while(TRUE){
		for(pwmCont = 0; pwmCont <=100; pwmCont++){
			set_dutyCicle(pwmCont);
			delay_ms(200);
		}
	}
}

void app_main(){
	printf("Testeando pwm...\n");
	xTaskCreate(mcpwmTest_mainTask, "mcpwmTest_mainTask", 4096, NULL, 5, NULL);
}
