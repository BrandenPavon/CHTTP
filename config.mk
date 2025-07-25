VERSION=1.1

CFLAGS_RELEASE = -O2 -Wall -Wextra -Werror -march=native -D_FORTIFY_SOURCE=2 -fstack-protector-strong \
				 -fstack-clash-protection -fsanitize=address
LDFLAGS_RELEASE =
CFLAGS_DEBUG = -O1 -Wall -march=native -g -pipe -D_FORTIFY_SOURCE=2 -fstack-protector-strong \
			   -fstack-clash-protection -fno-omit-frame-pointer -fsanitize=address
LDFLAGS_RELEASE = -fsanitize=address

CC=cc
