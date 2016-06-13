CC=gcc
DEBUG=-g
FINDSTATS=-D GETSTATS=1

SDIR=./src
ODIR=./obj
IDIR=./inc
BDIR=./bin

SRCS=$(SDIR)/BlobLL.c\
$(SDIR)/image.c\
$(SDIR)/tester.c\
$(SDIR)/Statistics.c

INCLUDE=$(addprefix -I,$(IDIR))
OBJS=${SRCS:$(SDIR)/%.c=$(ODIR)/%.o}
OBJSPHOTO=${SRCS:$(SDIR)/%.c=$(ODIR)/%.o}
CFLAGS=$(DEBUG) $(INCLUDE)

STATS = $(BDIR)/FindImageStats
PHOTONESS = $(BDIR)/ComputePhotoness

all: $(STATS)

$(STATS): $(OBJS)
		${CC} ${CFLAGS} -o $@ $(OBJS) -lm ${FINDSTATS}

$(OBJS): $(ODIR)/%.o : $(SDIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS)

cleanall:
		rm -f $(OBJS) ./bin/FindImageStats ./bin/ComputePhotoness
