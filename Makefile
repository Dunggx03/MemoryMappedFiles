CC = gcc

CFLAGS = -Wall -O2
 
TARGET = memorymappedfiles

SRC = main.c memory_monitor.c mmap_utils.c mmap_copy.c sharememory.c

HDR = memory_monitor.h mmap_utils.h mmap_copy.h sharememory.h maptable.h
 
$(TARGET): $(SRC) $(HDR)

	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
 
run: $(TARGET)

	./$(TARGET)
 
clean:

	rm -f $(TARGET) memory_usage.csv shared.txt test.txt copy.txt

 