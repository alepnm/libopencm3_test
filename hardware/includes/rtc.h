#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

extern volatile uint32_t timestamp;
extern volatile uint8_t UpdateDateTimeRequired;

/*   */
void    rtc_init(void);
int     rtc_backup_datetime(void);

#endif /* RTC_H_INCLUDED */
