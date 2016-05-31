CC=gcc

DEBUG=-g

SDIR=./src
ODIR=./obj
IDIR=./inc
BDIR=./bin

SRCS=$(SDIR)/BlobLL.c\
$(SDIR)/ColorClamp.c\
$(SDIR)/image.c\
$(SDIR)/tester.c\
$(SDIR)/BlobManager.c\
$(SDIR)/HeadBlobLL.c\
$(SDIR)/Queue.c

INCLUDE=$(addprefix -I,$(IDIR))
OBJS=${SRCS:$(SDIR)/%.c=$(ODIR)/%.o}
CFLAGS=$(DEBUG) $(INCLUDE)

TARGET = $(BDIR)/tester

all: $(TARGET)

$(TARGET): $(OBJS)
		${CC} ${CFLAGS} -o $@ $(OBJS) -lm

$(OBJS): $(ODIR)/%.o : $(SDIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS)

cleanall:
		rm -f $(OBJS) ./bin/tester
