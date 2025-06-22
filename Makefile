CC = cc
CFLAGS = -O2 -g  -fno-builtin # -nostdlib -ffreestanding
LDFLAGS = -O2 -g -fno-builtin # -nostdlib -ffreestanding

CORDIC_TABLE_FILE = .cordic_atan_table.h

BUILD = build/

MATH_SOURCES = src/math/comp.c src/math/exp.c src/math/fenv.c src/math/hypot.c src/math/log.c src/math/sqrt.c src/math/trig.c src/math/mod.c
SOURCES = src/core.c src/print.c src/relibc.c src/relibtest.c src/sysalloc.c $(MATH_SOURCES)
OBJS = $(patsubst %.c,build/%.o,$(SOURCES))

PRINTCC =       "CC          %-28s\n" "$@"
PRINTCFLAGS =   "CFLAGS      %-28s\n" "$(CFLAGS)"
PRINTLDFLAGS =  "LDFLAGS     %-28s\n" "$(LDFLAGS)"
PRINTLD =       "CCLD        %-28s\n" "$@"
PRINTBUILT =    "BUILT       %-28s\n" "$@"
PRINTRUN =      "RUN         %-28s\n" "$<"

relibtest: $(OBJS) | printinfo
	@printf $(PRINTLD)
	@$(CC) $(LDFLAGS) $^ -o $@
	@printf $(PRINTBUILT)

build/%.o: %.c
	@mkdir -p $(dir $@)
	@printf $(PRINTCC)
	@$(CC) -c $(CFLAGS) $< -o $@

run: relibtest
	@printf $(PRINTRUN)
	@./relibtest

printinfo:
	@printf $(PRINTCFLAGS)
	@printf $(PRINTLDFLAGS)

$(CORDIC_TABLE_FILE): gencordictable.py
	python gencordictable.py
