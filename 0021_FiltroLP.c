#include <stdio.h>
#include <string.h>
#include <driver/adc.h>
#include <driver/dac.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_system.h"

#define M 17

const float h[]=
{
	-0.0171035387965417, //h(0)
	-0.0419431579233366, //h(1)
	-0.0501329294124475, //h(2)
	-0.0309497847516785, //h(3)
	 0.0175345019583181, //h(4)
	 0.0864308262744764, //h(5)
	 0.158173992108178,  //h(6)
	 0.212237065988464,  //h(7)
	 0.232320416318186,  //h(8)
	 0.212237065988464,  //h(9)
	 0.158173992108178,  //h(10)
	 0.0864308262744764, //h(11)
	 0.0175345019583181, //h(12)
	-0.0309497847516785, //h(13)
	-0.0501329294124475, //h(14)
	-0.0419431579233366, //h(15)
	-0.0171035387965417  //h(16)
};


void filter_lowpass(){

	float y0 = 0;
	float x[M] = {0};
	unsigned char i = 0;

	adc1_config_width(ADC_WIDTH_9Bit);
	adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_11db);
	dac_output_enable(DAC_CHANNEL_1);

	while(1){
		for( i=M-1; i!=0; i-- )x[i]=x[i-1];
		x[0] = (float) adc1_get_raw(ADC1_CHANNEL_0);
		y0 = 0.0;
		for( i=0; i<M; i++ ) y0 += h[i]*x[i];
		y0 = y0*0.498;
		if(y0 > 255) y0 = 255;
		if(y0 < 0) y0 = 0;
		dac_output_voltage(DAC_CHANNEL_1, (char)y0 );
		ets_delay_us(720);
	}

}

void app_main()
{
	xTaskCreate(&filter_lowpass, "filter_lowpass", 2000, NULL, 5, NULL);
}

