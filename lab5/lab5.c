#include <limits.h>
#include <string.h>
#include <errno.h>

#include "test5.h"
#include "pixmap.h"

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
			"\t service run %s -args \"init <mode,delay>\"\n"
			"\t service run %s -args \"square <x,y,size,color>\"\n"
			"\t service run %s -args \"line <xi,yi,xf,yf,xpm,color>\"\n"
			"\t service run %s -args \"xpm <xi,yi,xpm>\"\n"
			"\t service run %s -args \"move <xi,yi,xpm,hor,delta,time>\"\n"
			"\t service run %s -args \"controller\"\n",
			argv[0], argv[0], argv[0]);
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

static unsigned long parse_ulong(char *str, int base) {

	char *endptr;

	unsigned long val;

	/* Convert string to unsigned long */

	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {

		perror("strtoul");

		return ULONG_MAX;

	}

	if (endptr == str) {

		printf("timer: parse_ulong: no digits were found in %s\n", str);

		return ULONG_MAX;

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
	unsigned short mode, delay;
	unsigned short x, y, size;
	unsigned long color;
	unsigned short xi, yi, xf, yf;
	char **xpm;
	unsigned short hor;
	short delta;
	unsigned short time;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf(
					"video_card: wrong no. of arguments for test_init(mode,delay)\n");
			return 1;
		}

		mode = parse_ushort(argv[2], 16);
		if (mode == USHRT_MAX)
			return 1;

		delay = parse_ushort(argv[3], 10);
		if (delay == USHRT_MAX)
			return 1;

		printf("video_card::test_init(0x%x, %d)\n", mode, delay);

		test_init(mode, delay);

	}else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf(
					"video_card: wrong no. of arguments for test_square(x,y,size,color)\n");
			return 1;
		}

		x = parse_ushort(argv[2], 10);
		if (x == USHRT_MAX)
			return 1;

		y = parse_ushort(argv[3], 10);
		if (y == USHRT_MAX)
			return 1;

		size = parse_ushort(argv[4], 10);
		if (size == USHRT_MAX)
			return 1;

		color = parse_ulong(argv[5], 10);
		if (size == ULONG_MAX)
			return 1;

		if (x < 0 || y < 0 || size < 0 || color < 0) {
			printf(
					"video_card: invalid arguments for test_square(x,y,size,color)\n");
			return 1;
		}

		printf("video_card::test_square(%d,%d,%d,%d)\n", x, y, size, color);

		test_square(x, y, size, color);

	}else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf(
					"video_card: wrong no. of arguments for test_line(xi,yi,xf,yf,color)\n");
			return 1;
		}

		xi = parse_ushort(argv[2], 10);
		if (xi == USHRT_MAX)
			return 1;

		yi = parse_ushort(argv[3], 10);
		if (yi == USHRT_MAX)
			return 1;

		xf = parse_ushort(argv[4], 10);
		if (xf == USHRT_MAX)
			return 1;

		yf = parse_ushort(argv[5], 10);
		if (yf == USHRT_MAX)
			return 1;

		color = parse_ulong(argv[6], 10);
		if (color == ULONG_MAX)
			return 1;

		if (xi < 0 || yi < 0 || xf < 0 || yf < 0 || color < 0) {
			printf(
					"video_card: invalid arguments for test_line(xi,yi,xf,yf,color)\n");
			return 1;
		}

		printf("video_card::test_line(%d,%d,%d,%d,%d)\n", xi, yi, xf, yf,
				color);

		test_line(xi, yi, xf, yf, color);

	}else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf(
					"video_card: wrong no. of arguments for test_xpm(xi,yi,xpm)\n");
			return 1;
		}

		xi = parse_ushort(argv[2], 10);
		if (xi == USHRT_MAX)
			return 1;

		yi = parse_ushort(argv[3], 10);
		if (yi == USHRT_MAX)
			return 1;

		if (xi < 0 || yi < 0) {
			printf(
					"video_card: invalid arguments for test_square(x,y,size,color)\n");
			return 1;
		}

		if (strncmp(argv[4], "pic1", strlen("pic1")) == 0) {
			xpm = pic1;
		} else if (strncmp(argv[4], "pic2", strlen("pic2")) == 0) {
			xpm = pic2;
		} else if (strncmp(argv[4], "cross", strlen("cross")) == 0) {
			xpm = cross;
		} else if (strncmp(argv[4], "pic3", strlen("pic3")) == 0) {
			xpm = pic3;
		} else if (strncmp(argv[4], "penguin", strlen("penguin")) == 0) {
			xpm = penguin;
		} else {
			printf("video_card: invalid arguments for test_xpm(xi,yi,xpm)\n");
			return 1;

		}

		printf("test_xpm(%d, %d, %s)\n", xi, yi, xpm);
		return test_xpm(xi, yi, xpm);

	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {
			printf(
					"video_card: wrong no. of arguments for test_xpm(xi,yi,xpm,hor,delta,time)\n");
			return 1;
		}

		xi = parse_ushort(argv[2], 10);
		if (xi == USHRT_MAX)
			return 1;

		yi = parse_ushort(argv[3], 10);
		if (yi == USHRT_MAX)
			return 1;

		if (xi > 1024 || yi > 768 || xi < 0 || yi < 0) {
			printf("video_card: invalid arguments for test_xpm(xi,yi,xpm,hor,delta,time)\n");
			return 1;
		}

		hor = parse_ushort(argv[5], 10);
		if (hor == USHRT_MAX)
			return 1;

		delta = parse_ushort(argv[6], 10);
		if (delta == SHRT_MAX)
			return 1;

		time = parse_ushort(argv[7], 10);
		if (time == USHRT_MAX)
			return 1;

		if (strncmp(argv[4], "pic1", strlen("pic1")) == 0) {
			xpm = pic1;
		} else if (strncmp(argv[4], "pic2", strlen("pic2")) == 0) {
			xpm = pic2;
		} else if (strncmp(argv[4], "cross", strlen("cross")) == 0) {
			xpm = cross;
		} else if (strncmp(argv[4], "pic3", strlen("pic3")) == 0) {
			xpm = pic3;
		} else if (strncmp(argv[4], "penguin", strlen("penguin")) == 0) {
			xpm = penguin;
		} else {
			printf("video_card: invalid arguments for test_xpm(xi,yi,xpm)\n");
			return 1;

		}



		printf("test_move(%d, %d, %s, %d,%d,%d)\n", xi, yi, xpm,hor,delta,time);
		return test_move(xi, yi, xpm, hor, delta, time);

	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0){
		printf("test_controller()\n");
		return test_controller();
	}else{
		printf("Error!");
		return 1;
	}


}

