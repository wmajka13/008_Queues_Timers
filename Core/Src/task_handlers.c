#include "main.h"
#include "stm32f4xx_hal_def.h"
#include <stdint.h>

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
		}

		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
	}
	vTaskDelete( NULL ); 
}

void led_task( void *pvParameters ) 
{ 
	const char* msg_led = "========================\n"
						  "|      LED Effect     |\n"
						  "========================\n"
						  "(none,e1,e2,e3,e4)\n"
						  "Enter your choice here : ";

	for( ;; )
	{
	//  -- Task application code here. --
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

	for( ;; )
	{
	//  -- Task application code here. --
	}
	// vTaskDelete( NULL ); 
}

void print_task( void *pvParameters ) 
{ 
	for( ;; )
	{
	//  -- Task application code here. --
	}
	// vTaskDelete( NULL ); 
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
