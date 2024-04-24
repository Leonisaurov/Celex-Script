#include "compiler.h"
#include "vm.h"
#include <Utils/string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage:\n\t%s [fileName.cle]\n", argv[0]);
		return -1;
	}
	String fileName = asStr(argv[1]);
	String file = readF(fileName);
	freeStr(fileName);

	char *outcode;
	uint64_t len;
	COMPILER_RESULT result = compile(file->label, &outcode, &len);
	freeStr(file);
	if (result == COMPILER_RESULT_ERR) {
		printf("ERR\n");
		return -1;
	}

	fileName = asStr("a.out");

	int fd = open(fileName->label, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1) {
		printf("No se pudo escribir el archivo en memoria\n");
		freeStr(fileName);
		free(outcode);
		return -1;
	}

	write(fd, outcode, len);

	close(fd);

	execute(outcode);

	freeStr(fileName);

	free(outcode);

	return 0;
}
