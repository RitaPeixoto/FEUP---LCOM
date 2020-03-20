PROG=proj

SRCS = proj.c video_gr.c keyboard.c timer.c menu.c maze.c player.c play.c mouse.c rtc.c auxiliar_functions.c interrupts.c

 #__LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
