#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    	int fd = open("/proc/cpuinfo", O_RDONLY);

	int file_length = 1000; // We cannot determine the file size of a proc file
    	char* buffer = (char*) malloc(file_length);

	int length = read(fd, buffer, file_length);

	char *vendor_id_string_p = (char*) malloc(100);

	sscanf(buffer, "%*s : %*d %*s : %s", vendor_id_string_p);
	printf("Vendor ID: %s\n", vendor_id_string_p);

	free(vendor_id_string_p);
  free(buffer);
}
