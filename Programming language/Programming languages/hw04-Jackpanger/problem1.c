#include <stdio.h>

int get_transcript_size(int student_id) { return 1; }

void load_transcript(int student_id, char* grades, int size) {
  for (int i = 0; i < size; i++)
    grades[i] = 'A';
}

char get_transcript_entry(int student_id, int entry) {
  int size = get_transcript_size(student_id);
  char grades[size];
  load_transcript(student_id, grades, size);
  return grades[entry];
}

int main() {
  int secret_id = 42; // replace by other values to test
  int entry = 0; // replace by other values to test
  char grade = get_transcript_entry(secret_id, entry);
  printf("Requested grade: %c\n", grade);
  return 0;
}