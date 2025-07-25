include config.mk

SRC = main.c client.c error.c responses.c
OBJ = ${SRC:.c=.o}


all: release 

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: LDFLAGS := $(LDFLAGS_DEBUG)
debug: optionsdebug main

release: CFLAGS := $(CFLAGS_RELEASE)
release: LDFLAGS := $(LDFLAGS_RELEASE)
release: optionsrel main

optionsrel:
	@echo Change in config.mk if needed:
	@echo "CFLAGS = ${CFLAGS_RELEASE}"
	@echo "Release Version -> ${VERSION}"
	@echo "LDFLAGS = ${LDFLAGS_RELEASE}"
	@echo "CC = ${CC}"
	@echo "--------------------------"

optionsdebug:
	@echo Change in config.mk if needed:
	@echo "CFLAGS = ${CFLAGS_DEBUG}"
	@echo "Debug Version -> ${VERSION}"
	@echo "LDFLAGS = ${LDFLAGS_DEBUG}"
	@echo "CC = ${CC}"
	@echo "--------------------------"

.c.o:
	${CC} -c -o $@ ${CFLAGS_RELEASE} $< 

main: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS_RELEASE} ${LDFLAGS_RELEASE}
clean:
	rm main *.o 
