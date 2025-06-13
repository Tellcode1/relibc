CC = cc
CFLAGS = -g -fno-builtin -std=c99
LDFLAGS = -g -fno-builtin -std=c99

CORDIC_TABLE_FILE = .cordic_atan_table.h

OBJS = relibtest.o relibc.c math.c

PRINTCC =       "CC          %-28s\n" "$@"
PRINTCFLAGS =   "CFLAGS      %-28s\n" "$(CFLAGS)"
PRINTLDFLAGS =  "LDFLAGS     %-28s\n" "$(LDFLAGS)"
PRINTLD =       "CCLD        %-28s\n" "$@"
PRINTBUILT =    "BUILT       %-28s\n" "$@"
PRINTRUN =      "RUN         %-28s\n" "$<"

relibtest: $(OBJS) | printinfo
	@printf $(PRINTLD)
	$(CC) $(LDFLAGS) $^ -o $@
	@printf $(PRINTBUILT)

%.o: %.c
	@printf $(PRINTCC)
	$(CC) -c $(CFLAGS) $< -o $@

run: relibtest
	@printf $(PRINTRUN)
	@./relibtest

printinfo:
	@printf $(PRINTCFLAGS)
	@printf $(PRINTLDFLAGS)

$(CORDIC_TABLE_FILE): gencordictable.py
	python gencordictable.py