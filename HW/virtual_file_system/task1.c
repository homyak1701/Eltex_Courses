#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

int forwardReadAndPrintFile(int fd);
int backReadAndPrintFile(int fd);

int backReadAndPrintFile(int fd) {
  void *buffer = NULL;
  off_t fileSize = 0;
  off_t errorLseek = 0;
  size_t errorRead = 0;

  fileSize = lseek(fd, 0, SEEK_END);
  if (fileSize < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  buffer = malloc(fileSize + 1);
  ((char*)buffer)[fileSize] = '\0';

  char *pieceBuffer = (char *)buffer;

  for (off_t i = 0; i < fileSize; ++i) {
    errorLseek = lseek(fd, (i + 1) * (-1), SEEK_END);
    if (errorLseek < 0) {
      perror("Error");
      exit(EXIT_FAILURE);
    }

    errorRead = read(fd, pieceBuffer, sizeof(char));
    if (errorRead < 0) {
      perror("Error");
      exit(EXIT_FAILURE);
    }

    pieceBuffer++;
  }

  printf("\n%s\n\n", (char *)buffer);

  free(buffer);

  return EXIT_SUCCESS;
}

int forwardReadAndPrintFile(int fd) {
  void *buffer = NULL;
  off_t fileSize = 0;
  off_t errorLseek = 0;
  size_t errorRead = 0;

  fileSize = lseek(fd, 0, SEEK_END);
  if (fileSize < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  errorLseek = lseek(fd, 0, SEEK_SET);
  if (errorLseek < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  buffer = malloc(fileSize + 1);
  ((char*)buffer)[fileSize + 1] = '\0';

  errorRead = read(fd, buffer, fileSize);
  if (errorRead < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  printf("\n%s\n\n", (char *)buffer);
  free(buffer);

  return EXIT_SUCCESS;
}

int main(void) {
  int fd = 0;
  int errorClose = 0;
  ssize_t errorWrite = 0;
  ssize_t errorRead = 0;
  char *fileName = "test.txt";
  char *textToWrite = "The  open()  system  call \
opens the file specified by pathname";

  fd = open(fileName, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
  if (fd < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  errorWrite = write(fd, textToWrite, strlen(textToWrite));
  if (errorWrite < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  forwardReadAndPrintFile(fd);

  backReadAndPrintFile(fd);

  errorClose = close(fd);
  if (errorClose < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
