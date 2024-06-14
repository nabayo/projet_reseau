CC := gcc
CFLAGS := -Wall -Wextra -g
LDLIBS := 

HEADIR = header
SRCDIR = src
OBJDIR = obj
BINDIR = bin
DOCDIR = doc

all:
	mkdir -p obj
	mkdir -p bin
	make $(BINDIR)/proxy
	make $(BINDIR)/client
	make $(BINDIR)/server

$(BINDIR)/proxy: $(OBJDIR)/proxy.o $(OBJDIR)/socket_utils.o $(OBJDIR)/client_side.o $(OBJDIR)/server_side.o $(OBJDIR)/noisemaker.o $(OBJDIR)/utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/server: $(OBJDIR)/server.o $(OBJDIR)/socket_utils.o $(OBJDIR)/proxy_side.o $(OBJDIR)/decoder.o $(OBJDIR)/utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BINDIR)/client: $(OBJDIR)/client.o $(OBJDIR)/utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(OBJDIR)/%.o: $(SRCDIR)/proxy/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(OBJDIR)/%.o: $(SRCDIR)/server/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -rf $(OBJDIR)/*.o
	rm -f $(BINDIR)/*
