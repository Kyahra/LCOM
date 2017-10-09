#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "RTC.h"
#include "defs.h"

static int rtc_hook_id = 8;

extern unsigned long data;
extern unsigned long output;
unsigned long asm_inb();



Time * newTime() {
	Time* time = (Time*) malloc(sizeof(Time));

	time->seconds = 0;
	time->minutes = 0;
	time->hour = 0;

	return time;

}

void deleteTime(Time* time) {
	free(time);
}

void updateTime(Time* time) {
	unsigned long seconds, minutes, hour;

	sys_enable_iop(SELF);

	sys_outb(RTC_ADDR_REG, REG_C);
	asm_inb();

	sys_outb(RTC_ADDR_REG, SECONDS);
	asm_inb();
	seconds = data;

	sys_outb(RTC_ADDR_REG, MINUTES);
	asm_inb();
	minutes = data;

	sys_outb(RTC_ADDR_REG, HOURS);
	asm_inb();
	hour = data;

	time->seconds = seconds;
	time->minutes = minutes;
	time->hour = hour;

}

int timeElapsed(Time* s_time, Time* c_time) {
	unsigned start;
	unsigned end;
	unsigned duration;

	start = 3600 * s_time->hour + 60 * s_time->minutes + s_time->seconds;
	end = 3600 * c_time->hour + 60 * c_time->minutes + c_time->seconds;

	duration = end - start;

	if (duration < 0)
		duration += 86400;

	return duration;

}

void drawScore(int score, int x, int y) {

	int digit;
	int i;

	for (i = 0; i < 7; i++) {
		if (score > 0) {
			digit = score % 10;
		} else
			digit = 0;

		drawBitmap(digits[digit], x, y);
		score /= 10;
		x -= 20;

	}
}

unsigned rtc_subscribe_int() {

	unsigned tmp_hook_id = rtc_hook_id;


	if(sys_outb(RTC_ADDR_REG, REG_B) !=OK) return -1;
	if(sys_outb(RTC_DATA_REG, RTC_CONF) !=OK) return -1;

	if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id)!=OK) return -1;
	if(sys_irqenable(&rtc_hook_id)!=OK) return -1;

	return BIT(tmp_hook_id);
}

int rtc_unsubscribe_int() {
	if(sys_irqrmpolicy(&rtc_hook_id)!=OK) return -1;

	return 0;
}
