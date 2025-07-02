# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/device/
.PATH: ${.CURDIR}/dispatcher/
.PATH: ${.CURDIR}/graphics/
.PATH: ${.CURDIR}/logic/
.PATH: ${.CURDIR}/menu/
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/utils/
.PATH: ${.CURDIR}/view/

# source code files to be compiled
SRCS = proj.c timer.c utils.c keyboard.c mouse.c video_gr.c menu.c sprite.c state.c game_view.c game_model.c asprite.c dispatcher.c game_logic.c arena.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D _LCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
