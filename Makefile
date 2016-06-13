CC=gcc
DEBUG=-g

SDIR=./src
ODIR=./obj
IDIR=./inc
BDIR=./bin

SRCS=$(SDIR)/BlobLL.c\
$(SDIR)/image.c\
$(SDIR)/Statistics.c

EXE1=$(SDIR)/CalcStats.c
EXE2=$(SDIR)/Categorize.c

INCLUDE=$(addprefix -I,$(IDIR))
OBJS=${SRCS:$(SDIR)/%.c=$(ODIR)/%.o}
OBJSPHOTO=${SRCS:$(SDIR)/%.c=$(ODIR)/%.o}
CFLAGS=$(DEBUG) $(INCLUDE)

STATS = $(BDIR)/CalcStats
PHOTONESS = $(BDIR)/Categorize

all: $(STATS) $(PHOTONESS)

$(STATS): $(OBJS)
		${CC} ${CFLAGS} $(EXE1) $(OBJS) -o $@ -lm

$(PHOTONESS): $(OBJS)
		${CC} ${CFLAGS} ${EXE2} $(OBJS) -o $@ -lm

$(OBJS): $(ODIR)/%.o : $(SDIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS)

cleanall:
		rm -f $(OBJS) ./bin/CalcStats ./bin/Categorize
