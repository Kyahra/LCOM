#include "i8254.h"
#include "test4.h"
#include <limits.h>
#include <string.h>
#include <errno.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv) {
	sef_startup();

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"packet <no. of packets>\"\n"
			"\t service run %s -args \"async <idle time>\"\n"
			"\t service run %s -args \"config\"\n"
			"\t service run %s -args \"gesture <y minimum length>\"\n"
			, argv[0],argv[0], argv[0]);
}

static unsigned short parse_ushort(char *str, int base) {
	char *endptr;
	unsigned short val;

	/* Convert string to unsigned short */
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

static short parse_short(char *str, int base) {
	char *endptr;
	short val;

	/* Convert string to unsigned short */
	val = (short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == SHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return SHRT_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_short: no digits were found in %s\n", str);
		return SHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

static int proc_args(int argc, char **argv) {
	unsigned short cnt, idle_time;
	short length;

	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_packet()\n");
			return 1;
		}
		cnt = parse_ushort(argv[2], 10);
		if (cnt == USHRT_MAX)
			return 1;

		if (cnt < 0) {
			printf("mouse: counter must be positive\n");
			return 1;
		}

		printf("mouse::test_packet(%d)\n", cnt);
		return test_packet(cnt);

	} else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_async()\n");
			return 1;
		}
		idle_time = parse_ushort(argv[2], 10);
		if (idle_time == USHRT_MAX)
			return 1;

		if (idle_time < 0) {
			printf("mouse: time must be positive\n");
			return 1;
		}

		printf("mouse::test_async(%d)\n", idle_time);
		return test_async(idle_time);

	} else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_gesture()\n");
			return 1;
		}
		length = parse_short(argv[2], 10);
		if (length == SHRT_MAX)
			return 1;


		printf("mouse::test_gesture(%d)\n", length);
		return test_gesture(length);

	} else if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if (argc != 2) {
			printf("mouse: wrong no. of arguments for test_config()\n");
			return 1;
		}

		printf("mouse::test_config()\n");

		return test_config();
	} else {
		printf("mouse: %s - no valid function!\n", argv[1]);
		return 1;
	}
}



