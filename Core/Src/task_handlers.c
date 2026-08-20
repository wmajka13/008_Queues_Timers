#include "main.h"
#include "portmacro.h"

void menu_task( void *pvParameters ) 
{ 
	for( ;; )
	{
	//  -- Task application code here. --
	}
	// vTaskDelete( NULL ); 
}

void led_task( void *pvParameters ) 
{ 
	for( ;; )
	{
	//  -- Task application code here. --
	}
	// vTaskDelete( NULL ); 
}

void rtc_task( void *pvParameters ) 
{ 
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
	// vTaskDelete( NULL ); 
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
