include config.mk

SRC = src/main.c src/client.c src/error.c src/headers.c
OBJ = $(patsubst src/%.c,build/%.o,$(SRC))

all: release

# include directory
CFLAGS_RELEASE += -Iinclude
CFLAGS_DEBUG += -Iinclude

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

# build directory rule
build/%.o: src/%.c
	@mkdir -p build
	${CC} -c -o $@ ${CFLAGS} $<

main: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS} ${LDFLAGS}

clean:
	rm -rf main build
