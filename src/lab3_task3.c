/*
 * Name: Derin Atila
 * Student ID: 251ADC003
 */
#include <stdio.h>

int my_strlen(const char* str) {
  int len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

void my_strcpy(char* dest, const char* src) {
  int i = 0;
  while (src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
}

int main() {
  char text[] = "Programming in C";
  int len = my_strlen(text);
  printf("Length: %d\n", len);

  char buffer[100];
  my_strcpy(buffer, text);
  printf("Copy: %s\n", buffer);
  return 0;
}