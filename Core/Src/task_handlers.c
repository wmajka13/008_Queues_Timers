#include "main.h"
#include "portmacro.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_uart.h"
#include <stdint.h>
#include <string.h>

const char *msg_inv = "////Invalid option////\n";


void menu_task( void *pvParameters ) 
{ 
	uint32_t cmd_addr;
	command_t *cmd;
	int option;

	const char* msg_menu = "\n========================\n"
							"|         Menu         |\n"
							"========================\n"
							"LED effect    ----> 0\n"
							"Date and time ----> 1\n"
							"Exit          ----> 2\n"
							"Enter your choice here : ";

	for( ;; )
	{
		xQueueSend(handle_queue_print, &msg_menu, portMAX_DELAY);

		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

		cmd = (command_t*) cmd_addr;

		if( cmd->len == 1 )
		{
			option = cmd->payload[0] - '0';

			switch (option)
			{
			case 0:
				curr_state = S_LED_EFFECT;
				xTaskNotify(handle_task_led, 0, eNoAction);
				break;
			
			case 1:
				curr_state = S_RTC_MENU;
				xTaskNotify(handle_task_rtc, 0, eNoAction);
				break;
				
			case 2:
				// TODO: Implement exit
				break;

			default:
				xQueueSend(handle_queue_print, &msg_inv, portMAX_DELAY);
				continue;
			}

		} else
		{
			//INVALID OPTION
			xQueueSend(handle_queue_print, &msg_inv, portMAX_DELAY);
			continue;
		}

		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
	}
	vTaskDelete( NULL ); 
}

void led_task( void *pvParameters ) 
{ 
	uint32_t cmd_addr;
	command_t *cmd;

	const char* msg_led = "========================\n"
						  "|      LED Effect     |\n"
						  "========================\n"
						  "(none,e1,e2,e3,e4)\n"
						  "Enter your choice here : ";

	for( ;; )
	{
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		xQueueSend(handle_queue_print, &msg_led, portMAX_DELAY);

		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

		cmd = (command_t *) cmd_addr;

		if( ! strcmp( (char*) cmd->payload, "none"))	led_effect_stop();
		else if ( ! strcmp( (char*) cmd->payload, "e1"))	led_effect(1);
		else if ( ! strcmp( (char*) cmd->payload, "e2"))	led_effect(2);
		else if ( ! strcmp( (char*) cmd->payload, "e3"))	led_effect(3);
		else if ( ! strcmp( (char*) cmd->payload, "e4"))	led_effect(4);
		else xQueueSend(handle_queue_print, &msg_inv, portMAX_DELAY);
	
		curr_state = S_MAIN_MENU;

		xTaskNotify(handle_task_menu, 0, eNoAction);
	}
	// vTaskDelete( NULL ); 
}

void rtc_task( void *pvParameters ) 
{
	const char* msg_rtc1 = "========================\n"
							"|         RTC          |\n"
							"========================\n";

	const char* msg_rtc2 = "Configure Time            ----> 0\n"
							"Configure Date            ----> 1\n"
							"Enable reporting          ----> 2\n"
							"Exit                      ----> 3\n"
							"Enter your choice here : ";


	const char *msg_rtc_hh = "Enter hour(1-12):";
	const char *msg_rtc_mm = "Enter minutes(0-59):";
	const char *msg_rtc_ss = "Enter seconds(0-59):";

	const char *msg_rtc_dd  = "Enter date(1-31):";
	const char *msg_rtc_mo  ="Enter month(1-12):";
	const char *msg_rtc_dow  = "Enter day(1-7 sun:1):";
	const char *msg_rtc_yr  = "Enter year(0-99):";

	const char *msg_conf = "Configuration successful\n";
	const char *msg_rtc_report = "Enable time&date reporting(y/n)?: ";


	uint32_t cmd_addr;
	command_t *cmd;

	RTC_TimeTypeDef time = {0};
	RTC_DateTypeDef date = {0};

	(void)time;
	(void)date;

	for(;;){
		/*Notify wait (wait till someone notifies) */
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		/*Print the menu and show current date and time information */
		xQueueSend(handle_queue_print, (void*) &msg_rtc1, portMAX_DELAY);
		xQueueSend(handle_queue_print, (void*) &msg_rtc2, portMAX_DELAY);

		show_time_date();
		

		while(curr_state != S_MAIN_MENU){

			/*Wait for command notification (Notify wait) */
			xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
			cmd = (command_t*) cmd_addr;

			switch(curr_state)
			{
				case S_RTC_MENU:

					/*process RTC menu commands */
					if ( ! strcmp( (char*) cmd->payload, "0")) curr_state = S_RTC_TIME_CONFIG;
					else if ( ! strcmp( (char*) cmd->payload, "1"))	curr_state = S_RTC_DATE_CONFIG;
					else if ( ! strcmp( (char*) cmd->payload, "2"))	curr_state = S_RTC_REPORT;
					else if ( ! strcmp( (char*) cmd->payload, "3"))	curr_state = S_MAIN_MENU;
					else xQueueSend(handle_queue_print, &msg_inv, portMAX_DELAY);

					break;

				case S_RTC_TIME_CONFIG:

					//TODO: TEST THIS!
					/*get hh, mm, ss infor and configure RTC */
					xQueueSend(handle_queue_print, (void*) &msg_rtc_hh, portMAX_DELAY);
					xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
					cmd = (command_t*) cmd_addr;

					int hh_int = atoi((char*) cmd->payload);

					/*take care of invalid entries */
					if(hh_int >= 1 && hh_int <= 12)
						time.Hours = hh_int;
					else 
						xQueueSend(handle_queue_print, &msg_inv, portMAX_DELAY);


					break;

				case S_RTC_DATE_CONFIG:

					/*TODO : get date, month, day , year info and configure RTC */

					/*TODO: take care of invalid entries */

					break;

				case S_RTC_REPORT:
					/*TODO: enable or disable RTC current time reporting over ITM printf */
					break;

				default:
					break;

			}// switch end

		} //while end

		   /*TODO : Notify menu task */


		}//while super loop end
}

void print_task( void *pvParameters ) 
{ 
	uint32_t *msg;
	for( ;; )
	{
		xQueueReceive(handle_queue_print, &msg, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen((char*)msg), HAL_MAX_DELAY);
	}
	vTaskDelete( NULL ); 
}

void command_handling_task( void *pvParameters ) 
{ 
	command_t cmd;
	for( ;; )
	{
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		process_command(&cmd);
	}
	vTaskDelete( NULL ); 
}

void process_command(command_t *cmd)
{
	extract_command(cmd);

	switch( curr_state )
	{
	case S_MAIN_MENU:
		xTaskNotify(handle_task_menu, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	
	case S_LED_EFFECT:
		xTaskNotify(handle_task_led, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
		
	case S_RTC_MENU:
	case S_RTC_TIME_CONFIG:
	case S_RTC_DATE_CONFIG:
	case S_RTC_REPORT:
		xTaskNotify(handle_task_rtc, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	}
}

int extract_command(command_t *cmd)
{
	uint8_t item;
	uint8_t i = 0;
	BaseType_t status;

	status = uxQueueMessagesWaiting(handle_queue_input_data);

	if( status == 0 ) return -1;

	do {
		status = xQueueReceive(handle_queue_input_data, (void*) &item, 0);
		if( status == pdTRUE ) cmd->payload[i++] = item;
	} while( item != '\n' );

	cmd->payload[i-1] = '\0';
	cmd->len = i-1;

	return 0;
}

