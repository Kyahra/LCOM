COMPILER_TYPE= gnu

CC=gcc

PROG=	proj

SRCS= main.c keyboard.c vbe.c video_gr.c bitmap.c timer.c 
SRCS+= ball.c game.c menu.c mouse.c play.c explosion.c over.c 
SRCS+= RTC.c RTC_asm.S obstacle.c
CCFLAGS= -Wall -03

DPADD+=	${LIBDRIVER} ${LIBSYS}

LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>