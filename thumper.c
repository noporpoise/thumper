#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

const char *cmd = NULL;
const char shortopts[] = "Fn:";

#define die(fmt,...) do { \
  fprintf(stderr, "[%s:%i] Error: %s() "fmt"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
  exit(EXIT_FAILURE); \
} while(0)

void print_usage()
{
  fprintf(stderr, "usage: %s [-F|-n <N>] <out_path>\n", cmd);
  fprintf(stderr, "  Print many lines to a file\n");
  fprintf(stderr, "  -n <N>    Print N lines [default 10,000]\n");
  fprintf(stderr, "  -F        Flush after printing each line\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  bool flush = false;
  unsigned long i, limit = 10000;
  cmd = argv[0];

  if(argc < 2) print_usage();

  // Arg parsing
  int c;
  opterr = 0; // silence getopt error messages

  while((c = getopt(argc, argv, shortopts)) != -1) {
    switch(c) {
      case 'F': flush = true; break;
      case 'n': limit = atol(optarg); break;
      case 'h': print_usage();
      default: print_usage();
    }
  }

  if(optind + 1 != argc) print_usage();

  const char *path = argv[optind];

  fprintf(stderr, "Running Thumper...\n");
  fprintf(stdout, "Writing %lu lines to file: %s\n", limit, path);

  FILE *fout = fopen(path, "w");
  if(!fout) die("Cannot open file: %s", path);

  for(i = 0; i < limit; i++) {
    fprintf(fout, "%lu\n", i);
    if(flush) fflush(fout);
  }

  if(flush) fflush(fout);
  fclose(fout);
  fprintf(stderr, "Done.\n");
  return EXIT_SUCCESS;
}
