#include "test3.h"
#include "i8254.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int proc_args(int argc, char **argv);
static unsigned short parse_ushort(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {
	sef_startup();
	/* DO NOT FORGET TO initialize service */

	if (argc == 1) {
		print_usage(argv);
		return 0;
	}

	else
		return proc_args(argc, argv);
}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"scan < 0 ->C, !=0 ->Assembly>\" \n"
					"\t service run %s -args \"leds <array of leds to toogle>\" \n"
					"\t service run %s -args \"timed_scan <unsigned short time(s)>\" \n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	unsigned short ass;
	unsigned short time;

	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		ass = parse_ushort(argv[2], 10);

		if (ass == USHRT_MAX)
			return 1;

		printf("kbd::kbd_test_scan(%d)\n", ass);
		return kbd_test_scan(ass);
	}

	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {

		if (argc < 3) {
			printf(
					"keyboard: wrong no. of arguments for test of kbd_test_leds() \n");
			return 1;
		}
		unsigned int size = argc - 2;
		unsigned short *toggle = malloc(sizeof(unsigned short) * size);

		unsigned int i = 0;

		while (i < size) {
			unsigned short x = parse_ushort(argv[i + 2], 10);
			if (x < 0 || x > 2) {
				printf("keyboard: invalid value (0-2)\n");
				return 1;
			}
			toggle[i] = x;
			i++;
		}

		printf("keyboard :: kbd_test_leds()\n");

		kbd_test_leds(size, toggle);

		return 0;
	} else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
		if (argc != 3) {
			printf(
					"keyboard: wrong no of arguments for keyboard_timed_scan()\n");
			return 1;
		}

		time = parse_ushort(argv[2], 10);

		if (time == USHRT_MAX)
			return 1;
		if (time < 0) {
			printf(
					"keyboard: wrong argument for keyboard_timed_scan, argument must be >0()\n");
			return 1;
		}

		printf("timer::kbd_test_timed_scan(%lu)\n", time);
		return kbd_test_timed_scan(time);
	} else {
		printf("keyboard: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned short parse_ushort(char *str, int base) {
	char *endptr;
	unsigned short val;

	/* Convert string to unsigned long */
	val = (unsigned short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_ushort: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

