#include "main.h"
#include "portmacro.h"
#include "stm32f4xx_hal_rtc.h"
#include <stdio.h>


void show_time_date(void)
{
    static char time_bufor[40];
    static char date_bufor[40];

    char * pvtime_bufor = time_bufor;
    char * pvdate_bufor = date_bufor;

    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

    // DISPLAY TIME IN hh:mm:ss format
    sprintf(time_bufor, "%s: \t%0.2d:%0.2d:%0.2d", "\nCurrent Time&Date", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    xQueueSend(handle_queue_print, (void*) &pvtime_bufor, portMAX_DELAY);

    // DISPLAY DATE IN day-month-year format
    sprintf(date_bufor, "\t%02d-%02d-%2d\n\n", rtc_date.Date, rtc_date.Month, 2000 + rtc_date.Year);
    xQueueSend(handle_queue_print, (void*) &pvdate_bufor, portMAX_DELAY);

}


void rtc_configure_time(RTC_TimeTypeDef *time)
{
    time->TimeFormat = RTC_HOURFORMAT12_AM;
    HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);
}

void rtc_configure_date(RTC_DateTypeDef *date)
{
    HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);
}