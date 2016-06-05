#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

const char *cmd = NULL;
const char shortopts[] = "Fn:b:";

#define die(fmt,...) do { \
  fprintf(stderr, "[%s:%i] Error: %s() "fmt"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
  exit(EXIT_FAILURE); \
} while(0)

void print_usage()
{
  fprintf(stderr, "usage: %s [-F|-n <N>] <out_path>\n", cmd);
  fprintf(stderr, "  Print many lines to a file\n");
  fprintf(stderr, "  -b <N>    Write N blocks of 8k\n");
  fprintf(stderr, "  -n <N>    Print N lines\n");
  fprintf(stderr, "  -F        Flush after printing each line\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  bool flush = false;
  unsigned long i, nlines = 0, nblocks = 0;
  cmd = argv[0];

  if(argc < 2) print_usage();

  // Arg parsing
  int c;
  opterr = 0; // silence getopt error messages

  while((c = getopt(argc, argv, shortopts)) != -1) {
    switch(c) {
      case 'F': flush = true; break;
      case 'n': nlines = atol(optarg); break;
      case 'b': nblocks = atol(optarg); break;
      case 'h': print_usage();
      default: print_usage();
    }
  }

  if(optind + 1 != argc || (!nlines && !nblocks)) print_usage();

  const char *path = argv[optind];

  fprintf(stderr, "Running Thumper...\n");

  const size_t block_len = 1024*2;
  FILE *fout = fopen(path, "w");
  if(!fout) die("Cannot open file: %s", path);

  if(flush) fprintf(stderr, "Calling flush after every write\n");

  fprintf(stderr, "Writing %lu blocks to file: %s\n", nblocks, path);
  int *data = calloc(sizeof(int), block_len);
  for(i = 0; i < block_len; i++) data[i] = i;
  for(i = 0; i < nblocks; i++) {
    fwrite(data, sizeof(int), block_len, fout);
    if(flush) fflush(fout);
  }
  free(data);

  fprintf(stderr, "Writing %lu lines to file: %s\n", nlines, path);
  for(i = 0; i < nlines; i++) {
    fprintf(fout, "%lu\n", i);
    if(flush) fflush(fout);
  }

  fclose(fout);
  fprintf(stderr, "Done.\n");
  return EXIT_SUCCESS;
}
