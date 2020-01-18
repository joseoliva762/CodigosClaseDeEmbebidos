#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "string.h"
#include "DHT.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "nvs_flash.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"

#include "tcpip_adapter.h"
#include "esp_event.h"

#define WIFI_SSID "Ashanty"
#define WIFI_PASS "ForR3al"
#define WIFI_CHNL 10
#define true 1
#define false 0
#define SensorUpLed 2
#define coneccionLed 25
#define  direction "/vsd"
#define final "/"

static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

xSemaphoreHandle client_connected;
xSemaphoreHandle come_on;
xSemaphoreHandle come_back;
xQueueHandle command;

//*************************************************************
//Globales para pruebas.
int sock_client;
//*************************************************************

void delay_ms(int ms){
	vTaskDelay(ms/portTICK_PERIOD_MS);
}

void Sendfun(char *buff){
	int buffer[100] = {0};
	strcpy((char*)buffer, buff);
	gpio_set_level(SensorUpLed,true);
	send(sock_client, buffer, sizeof(buffer), 0);
	gpio_set_level(SensorUpLed,false);
	memset(buffer,0,sizeof(buffer));
}

char comparate(char* buffer){
	char cat[] = "cat";
	char nano[] = "nano";
	char more[] = "more";
	char ls[] = "ls";
	char remove[] = "remove";
	char rename[] = "rename";
	char fflush[] = "fflush";
	char mkdir[] = "mkdir";
	char cd[] = "cd";
	char home[] = "$HOME";
	char pwd[] = "pwd";
	char rmdir[] = "rmdir";
	char cdpunto[] = "bcd";

	if(strncmp(buffer,cat,3) == 0) return 0;
	else if(strncmp(buffer,more,4) == 0) return 1;
	else if(strncmp(buffer,nano,4) == 0) return 2;
	else if(strncmp(buffer,ls,2) == 0) return 3;
	else if(strncmp(buffer,remove,6) == 0) return 4;
	else if(strncmp(buffer,rename,6) == 0) return 5;
	else if(strncmp(buffer,fflush,6) == 0) return 6;
	else if(strncmp(buffer,mkdir,5) == 0) return 7;
	else if(strncmp(buffer,cd,2) == 0) return 8;
	else if(strncmp(buffer,home,5) == 0) return 9;
	else if(strncmp(buffer,pwd,3) == 0) return 10;
	else if(strncmp(buffer,rmdir,5) == 0) return 11;
	else if(strncmp(buffer,cdpunto,3) == 0) return 12;
	else return 13;
	//<cat> <more> <nano> <ls> <remove> <rename> <mkdir> <cd> <$HOME> <pwd> <rmdir>

}

//cat*********************************************************************************************************
void cat_command(char *dir){
	DIR *dr = NULL;
	FILE *archivo = NULL;
	char buffer[50] = {0};
	char directorio[50] = {0};

	//File System
	printf("LLEGOOOOOOOOOOOOOOOOOO\r\n");

	dr = opendir(dir);
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		strcpy(directorio,dir);
		memset(dir, 0, sizeof(&dir));
		printf("Directorio abierto. \r\n");
		printf("\r\n cat\r\n");
		strcpy(buffer,"\tNombre: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);

		printf("Directorio abierto.\r\n");
		strcat(directorio,final);
		strcat(directorio,buffer);
		printf("%s\r\n", directorio);
		archivo = fopen(directorio,"w");
		fclose(archivo);
		memset(&buffer,0,sizeof(buffer));
		if(archivo != NULL){
			memcpy(buffer, "\tArchivo creado [200].\r\n",sizeof("\tArchivo creado [200].\r\n"));
			printf("\r\n archivo creado [OK]\r\n");
		}else{
			memcpy(buffer, "\tArchivo No creado [404].\r\n",sizeof("\tArchivo No creado [404].\r\n"));
			printf("\r\narchivo No creado [404]\r\n");
		}
		strcpy(dir,directorio);
		Sendfun(buffer);
		closedir(dr);
	}
}

//ls **********************************************************************************************************
void ls_command(char *dir){
	DIR *dr = NULL;
	struct dirent *entrada_directory;
	char buffer[50] = {0};
	//File System

	dr = opendir(dir);
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("dir encontrado.\r\n");
		if((entrada_directory  = readdir(dr)) == NULL){
			memset(&buffer,0,sizeof(buffer));
			strcpy(buffer, "\tNo se encontraron Archivos, ni directorios.\r\n");
			Sendfun(buffer);
			printf("%s\r\n", buffer);
		}else{
			memset(&buffer,0,sizeof(buffer));
			strcpy(buffer, "\t");
			strcat(buffer,entrada_directory->d_name);
			strcat(buffer,"\r\n");
			Sendfun(buffer);
			printf("%s\r\n", buffer);
			while((entrada_directory  = readdir(dr)) != NULL){
				//printf("%s\r\n", entrada_directory->d_name);
				memset(&buffer,0,sizeof(buffer));
				strcpy(buffer, "\t");
				strcat(buffer,entrada_directory->d_name);
				strcat(buffer,"\r\n");
				Sendfun(buffer);
				printf("%s\r\n", buffer);
			}
		}
		closedir(dr);
	}
}

//nano*********************************************************************************************************
void nano_command(char *dir){
	DIR *dr = NULL;
	FILE *archivo = NULL;
	char buffer[50] = {0};
	//File System

	dr = opendir(dir);
	//dr = opendir("/vsd");
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("Directorio abierto. \r\n");
		printf("\r\n nano\r\n");
		strcpy(buffer,"\tNombre: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);

		printf("Directorio abierto.\r\n");
		strcat(dir,final);
		strcat(dir,buffer);
		printf("%s\r\n", dir);
		archivo = fopen(dir,"a");

		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer,"\tTexto: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);

		if(archivo != NULL){
			fprintf(archivo, buffer);
			memset(buffer,0,sizeof(buffer));
			memcpy(buffer, "\tArchivo Actualizado.[200].\r\n",sizeof("\tArchivo Actualizado.[200].\r\n"));
			Sendfun(buffer);
			printf("\r\n archivo ACTUALIZADO [OK]\r\n");
		}else{
			memset(&buffer,0,sizeof(buffer));
			memcpy(buffer, "\tArchivo No Actualizado.[404].\r\n",sizeof("\tArchivo No Actualizado.[404].\r\n"));
			Sendfun(buffer);
			printf("\r\narchivo No ACTUALIZADO [404]\r\n");
		}
		fclose(archivo);
		memset(&buffer,0,sizeof(buffer));
		closedir(dr);
	}

}

//remove*******************************************************************************************************
void remove_command(char *dir){
	DIR *dr = NULL;
	char buffer[50] = {0};

	dr = opendir(dir);
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("Directorio abierto. \r\n");
		printf("\r\n remove\r\n");
		strcpy(buffer,"\tNombre: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);
		strcat(dir,final);
		strcat(dir,buffer);
		printf("%s\r\n", dir);
		if(remove(dir) != 0){
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,"\tEliminacion Fallida [400]\r\n");
			Sendfun(buffer);
		}else{
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,"\tEliminacion Exitosa [200]\r\n");
			Sendfun(buffer);
		}

	}
	memset(&buffer,0,sizeof(buffer));
	closedir(dr);

}
/*
//fflush*******************************************************************************************************
void fflush_command(){
	DIR *dr = NULL;

	char buffer[50] = {0};
	char dir[50]={0};

	memset(dir,0,sizeof(dir));
	strcpy(dir,direction);

	dr = opendir("/vsd");
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("Directorio abierto. \r\n");
		printf("\r\n fflush\r\n");
		strcpy(buffer,"\tNombre: ");
		gpio_set_level(SensorUpLed,true);
		send(sock_client, buffer, sizeof(buffer), 0);
		gpio_set_level(SensorUpLed,false);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);
		strcat(dir,buffer);
		printf("%s\r\n", dir);



		archivo = fopen(dir, "r");
	    //fwrite(&datos, sizeof(datos), 1, save);
		if(fflush(dir) != 0){
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,"\tEl archivo no ha sido vaceado [400]\r\n");
			gpio_set_level(SensorUpLed,true);
			send(sock_client, buffer, sizeof(buffer), 0);
			gpio_set_level(SensorUpLed,false);
		}else{
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,"\tArchivo vacio [200]\r\n");
			gpio_set_level(SensorUpLed,true);
			send(sock_client, buffer, sizeof(buffer), 0);
			gpio_set_level(SensorUpLed,false);
		}

	}
	memset(&buffer,0,sizeof(buffer));
	closedir(dr);

}
*/
//rename*******************************************************************************************************
void rename_command(char *dir){
	DIR *dr = NULL;
	char buffer[200] = {0};
	char newName[200] = {0};
	char newdir[50]={0};

	memset(newdir,0,sizeof(newdir));
	strcpy(newdir,dir);

	dr = opendir(dir);
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("Directorio abierto. \r\n");
		printf("\r\n rename\r\n");
		strcpy(buffer,"\tNombre Actual: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);
		strcat(dir,final);
		strcat(dir,buffer);

		strcpy(newName,"\tNuevo Nombre: ");
		Sendfun(newName);
		memset(newName, 0, sizeof(newName));
		recv(sock_client , newName , sizeof(newName) , 0);
		strcat(newdir,final);
		strcat(newdir,newName);

		if(rename(dir, newdir) != 0){
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,"\tCambio Fallido [400]\r\n");
		}else{
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,"\tCambio Exitoso [200]\r\n");
		}
		Sendfun(buffer);
	}
	memset(buffer,0,sizeof(buffer));
	memset(newName,0,sizeof(newName));
	closedir(dr);
}

//more*********************************************************************************************************
void more_command(char *dir){
	DIR *dr = NULL;
	FILE *archivo = NULL;
	char buffer[200] = {0};

	dr = opendir(dir);
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("Directorio abierto. \r\n");
		printf("\r\n more\r\n");
		strcpy(buffer,"\tNombre: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);

		printf("Directorio abierto.\r\n");
		strcat(dir,final);
		strcat(dir,buffer);
		printf("%s\r\n", dir);
		archivo = fopen(dir,"r");
		memset(buffer, 0, sizeof(buffer));
		if(archivo != NULL){
			printf("archivo encontrado [OK]\r\n");
			send(sock_client, "\tArchivo encontrado [OK]\r\n", sizeof("\tArchivo encontrado [OK]\r\n"), 0);
			while(!feof(archivo)){
				memset(buffer, 0, sizeof(buffer));
				fgets(buffer, sizeof(buffer), archivo);
				//fgets((char*)buffer, sizeof((char*)buffer), archivo);
				printf("%s\r\n", buffer);
				Sendfun(buffer);
			}
		}else send(sock_client, "\tArchivo No Encontrado\r\n", sizeof("\tArchivo No Encontrado\r\n"), 0);

		fclose(archivo);
		memset(buffer,0,sizeof(buffer));
		closedir(dr);
	}
}

//mkdir command************************************************************************************************************************
void mkdir_command(char *dir){
	DIR *dr = NULL;
	char buffer[50] = {0};

	//File System
	dr = opendir(dir);
	if(dr == NULL){
		printf("no se encontró el directorio\r\n");
	}else{
		printf("Directorio abierto. \r\n");
		printf("\r\n mkdir\r\n");
		strcpy(buffer,"\tNombre: ");
		Sendfun(buffer);
		memset(buffer, 0, sizeof(buffer));
		recv(sock_client , buffer , sizeof(buffer) , 0);
		strcat(dir,final);
		strcat(dir,buffer);
		printf("%s\r\n", dir);

		if(mkdir(dir, S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO) == 0) memcpy(buffer, "\tDirectorio creado [200].\r\n",sizeof("\tDirectorio creado [200].\r\n"));
		else memcpy(buffer, "\tDirectorio No creado [404].\r\n",sizeof("\tDirectorio No creado [404].\r\n"));

		printf("%s ",buffer);
		Sendfun(buffer);
		closedir(dr);
	}
}
//rmdir command************************************************************************************************************************
void rmdir_command(char *dir){
	DIR *dr = NULL;
	char buffer[50] = {0};

	printf("\r\n rmdir\r\n");
	strcpy(buffer,"\tNombre: ");
	Sendfun(buffer);
	memset(buffer, 0, sizeof(buffer));
	recv(sock_client , buffer , sizeof(buffer) , 0);
	strcat(dir,final);
	strcat(dir,buffer);
	printf("%s\r\n", dir);
	rmdir(dir);
	memset(&buffer,0,sizeof(buffer));
	dr = opendir(dir);
	if(dr == NULL){
		printf("Directorio ELIIMINADO\r\n");
		memcpy(buffer, "\tDirectorio ELIIMINADO\r\n",sizeof("\tDirectorio ELIIMINADO\r\n"));
	}else{
		printf("Directorio NO ELIIMINADO\r\n");
		memcpy(buffer, "\tDirectorio NO ELIIMINADO\r\n",sizeof("\tDirectorio NO ELIIMINADO\r\n"));
		closedir(dr);	}
	Sendfun(buffer);
	printf("LEGOOOOO...\r\n");
}
//CD_command**********************************************************************************************************************
void cd_command(char *dir){
	DIR *dr = NULL;
	//char direccion[50] = {0};
	char buffer[50] = {0};
	char passdir[50] = {0};
	char finalbuffer[50] = {0};
	strcpy(passdir,dir);
	strcpy(finalbuffer,final);

	printf("Directorio abierto. \r\n");
	printf("\r\n cd\r\n");
	strcpy(buffer,"\tNombre: ");
	Sendfun(buffer);
	memset(buffer, 0, sizeof(buffer));
	recv(sock_client , buffer, sizeof(buffer), 0);
	strcat(finalbuffer,buffer);
	strcat(dir,finalbuffer);
	printf("%s\r\n", dir);
	memset(&buffer,0,sizeof(buffer));
	dr = opendir(dir);
	if(dr == NULL){
		strcpy(dir,passdir);
		printf("no se encontró el directorio\r\n");
		memcpy(buffer, "\tNo se encontró el directorio\r\n",sizeof("\tNo se encontró el directorio\r\n"));
	}else{
		printf("Directorio abierto. \r\n");
		memcpy(buffer, "\tDirectorio abierto. \r\n",sizeof("\tDirectorio abierto. \r\n"));
		closedir(dr);
	}
	Sendfun(buffer);
}

//Command fail******************************************************************************************************************
void command_fail(){
	char buffer[200] = {0};
	char n = 0;
	printf("\r\n command not found\r\n");
	while(n != 3){
		memset(buffer,0,sizeof(buffer));
		if(n == 0)memcpy(buffer, "\tcommand not found.\r\n",sizeof("\tcommand not found.\r\n"));
		if(n == 1)memcpy(buffer, "\tTal vez quizo decir: <cat> <more> <nano> <ls> <remove> ",sizeof("\tTal vez quizo decir: <cat> <nano> <more> <ls> <remove> <rename>\r\n"));
		if(n == 2)memcpy(buffer, "<rename> <mkdir> <cd> <$HOME> <pwd> <rmdir>\r\n", sizeof("<rename> <mkdir> <cd> <$HOME> <pwd> <rmdir>\r\n"));
		Sendfun(buffer);
		n++;
	}
}

// Wifi event handler
static esp_err_t event_handler(void *ctx, system_event_t *event){
	printf("Eventos de Wifi %d\r\n", event->event_id);
    switch(event->event_id){

       case SYSTEM_EVENT_AP_START:
           printf("AP inicializado OK\r\n");
           break;

	   case SYSTEM_EVENT_AP_STACONNECTED:
          xSemaphoreGive(client_connected);
          break;

	   default:
          break;
    }
	return ESP_OK;
}


// Main task
void main_task(){
	int sock, c;
	int cont = 0, tamano = 0,cont2= 0;
	char caracter = 0;
	char dir[50]={0};
	char prevdir[50] = {0};
	char pwdagg[50] = {0};

	gpio_set_level(SensorUpLed,true);
	gpio_set_level(coneccionLed,false);
	esp_vfs_fat_mount_config_t mount_config = {
		.max_files = 4,
		.format_if_mount_failed = false
	};
	struct sockaddr_in server, client;
	while(1){
		//MEMDIR
		char username[] = "@Root:~$ ";
		char SendBuff[50] ={0};
		char buffer[50] = {0};

		//usr
		strcpy(SendBuff,WIFI_SSID);
		strcat(SendBuff,username);
		printf("%s\r\n",SendBuff);

		//START ALL.
		gpio_set_level(coneccionLed,false);

		xSemaphoreTake(client_connected, portMAX_DELAY);
		printf("Cliente Conectado\r\n");
		gpio_set_level(coneccionLed,true);
		while(1){
			//Sockets
			sock = socket(AF_INET, SOCK_STREAM, 0);

			server.sin_family = AF_INET;
			server.sin_addr.s_addr = INADDR_ANY;
			server.sin_port = htons(3001);

			if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
				printf("Error de binding\r\n");
			}

			printf("Enlace Ok\r\n");
			memset(buffer, 0, sizeof(buffer));


			//Escuchando
			listen(sock, 3);

			while(1){
				printf("Esperando Conexiones\r\n");
				c = sizeof(struct sockaddr_in);
				sock_client = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c);
				if (sock_client < 0){
					printf("conexión rechazada");
					return;
				}
				printf("Cliente Conectado\r\n");
				Sendfun(SendBuff);
				if( esp_vfs_fat_spiflash_mount("/vsd", "storage", &mount_config, &s_wl_handle) == ESP_OK){
					printf("virtual file system [OK]\r\n");
					memset(dir,0,sizeof(dir));
					strcpy(dir,direction);
					//strcat(dir,"/");
					while(1){
						caracter = 13;
						//CREAMOS EL DIR
						memset(buffer, 0, sizeof(buffer));
						//Recibir mensajes
						recv(sock_client , buffer , sizeof(buffer) , 0);
						//recive el comando
						caracter = comparate((char*)buffer);
						printf("%s: %s ==> %d\r\n ", SendBuff, buffer, caracter);
						switch (caracter){
							case 0:
								printf("%s\r\n",dir);
								strcpy(prevdir,dir);
								cat_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 1:
								strcpy(prevdir,dir);
								more_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 2:
								strcpy(prevdir,dir);
								nano_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 3:
								strcpy(prevdir,dir);
								ls_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 4:
								strcpy(prevdir,dir);
								remove_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 5:
								strcpy(prevdir,dir);
								rename_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 6:
								//fflush_command();
								printf("FFLUSH NO DISPONIBLE\r\n");
							 break;
							case 7:
								strcpy(prevdir,dir);
								mkdir_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 8:
								cd_command(dir);
								strcpy(prevdir,dir);
								memset(buffer,0,sizeof(buffer));
								tamano = strlen(prevdir);
								for(cont2 = 0; cont2 <= tamano; cont2++){
									if(prevdir[cont2] == 13) cont++;
								}
								memset(dir,0,sizeof(dir));
								printf("cont = %d\r\n", cont);
								cont2 = 0;
								tamano = 0;
								while(cont2 != cont){
									if(prevdir[tamano] == 13) cont2++;
									if(cont2 != cont){
										printf("cont2 = %d\r\n", cont2);
										dir[tamano] = prevdir[tamano];
										tamano++;
									}
								}
								memset(prevdir,0,sizeof(prevdir));
								cont = cont2 = tamano = 0;
								printf("%s\r\n",dir);
								printf("cortado\r\n");
							 break;
							case 9:
								memset(dir,0,sizeof(dir));
								strcpy(dir,direction);
							 break;
							case 10:
								strcpy(pwdagg,"\t");
								strcat(pwdagg,dir);
								strcat(pwdagg,"\r\n");
								Sendfun(pwdagg);
								memset(pwdagg,0,sizeof(pwdagg));
							 break;
							case 11:
								strcpy(prevdir,dir);
								rmdir_command(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							case 12:
								strcpy(prevdir,dir);
								memset(buffer,0,sizeof(buffer));

								tamano = strlen(prevdir);
								for(cont2 = 0; cont2 <= tamano; cont2++){
									if(prevdir[cont2] == 47) cont++;
								}
								if(cont == 1){
									memset(prevdir,0,sizeof(prevdir));
									strcpy(prevdir,"\tEsta en el directorio principal\r\n");
								}else{
									memset(dir,0,sizeof(dir));
									printf("cont = %d\r\n", cont);
									cont2 = 0;
									tamano = 0;
									while(cont2 != cont){
										if(prevdir[tamano] == 47) cont2++;
										if(cont2 != cont){
											printf("cont2 = %d\r\n", cont2);
											dir[tamano] = prevdir[tamano];
											tamano++;
										}
									}
									memset(prevdir,0,sizeof(prevdir));
									strcpy(prevdir,"\t");
									strcat(prevdir,dir);
									strcat(prevdir,"\r\n");
								}
								cont = cont2 = tamano = 0;
								printf("%s\r\n",dir);
								Sendfun(prevdir);
								memset(prevdir,0,sizeof(prevdir));
							 break;
							default:
								command_fail();
							 break;
						}
						Sendfun(SendBuff);
					}
				} else printf("Error\r\n");
			}
			close(sock_client);
		}
		delay_ms(1000);
	}
}


// Main application
void app_main(){
	gpio_set_direction(SensorUpLed,GPIO_MODE_OUTPUT);
	gpio_set_direction(coneccionLed,GPIO_MODE_OUTPUT);

	//Deshabilitamos log eventos wifi
	esp_log_level_set("wifi", ESP_LOG_NONE);

	//Inicialización NVS (Non Volatile Storage)
	ESP_ERROR_CHECK(nvs_flash_init());

	tcpip_adapter_init();

	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	tcpip_adapter_ip_info_t info;

	memset(&info, 0, sizeof(info));
	IP4_ADDR(&info.ip, 192, 168, 10, 1);
	IP4_ADDR(&info.gw, 192, 168, 10, 1);
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));


	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

	wifi_auth_mode_t auth = WIFI_AUTH_OPEN;

	wifi_config_t ap_config = {
		.ap = {
			.ssid = WIFI_SSID,
			.password = WIFI_PASS,
			.ssid_len = 0,
			.channel = WIFI_CHNL,
			.authmode = auth,
			.ssid_hidden = 0,
			.max_connection = 4,
			.beacon_interval = 100,
		},
	};

	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	printf("Inicializando AP\r\n");

	client_connected = xSemaphoreCreateBinary();
	come_on = xSemaphoreCreateBinary();
	come_back = xSemaphoreCreateBinary();
	command = xQueueCreate(2, sizeof(char));

	//Tarea principal
    xTaskCreate(main_task, "main_task", 20000, NULL, 5, NULL);
}
