#include "util/types.h"
#include "util/span.h"
#include "util/bit.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define int_type_check(name, type)                                             \
type name##_var = 0;                                                         \
name##_var = 0;                                                              \
printf("-- " #name " (" #type ") --\n");                                     \
printf("size: %lu\n", sizeof(type) * 8);                                     \
printf("0 -> %d\n", (int)name##_var);                                        \
name##_var = 1;                                                              \
printf("1 -> %d\n", (int)name##_var);                                        \
name##_var = -1;                                                             \
printf("-1 -> %d\n", (int)name##_var);                                       \
printf("\n")

#define smart_type_check(type) int_type_check(type, type)

static int int_type_test() {

  int_type_check(n8, char);
  int_type_check(n16, short);
  int_type_check(n64, long);

  int_type_check(s8, signed char);
  int_type_check(s16, signed short);
  int_type_check(s64, signed long);

  int_type_check(u8, unsigned char);
  int_type_check(u16, unsigned short);
  int_type_check(u64, unsigned long);

  int_type_check(f32, float);
  int_type_check(f64, double);
  int_type_check(f128, long double);

  return ok;
}

static int types_test() {

  smart_type_check(u8);
  smart_type_check(u16);
  smart_type_check(u32);
  smart_type_check(u64);

  smart_type_check(s8);
  smart_type_check(s16);
  smart_type_check(s32);
  smart_type_check(s64);

  smart_type_check(f32);
  smart_type_check(f64);

  return ok;
}

static int bool_test() {
  bool b = true;

  if (b) {
    printf("%d is true|nonempty", b);
  } else {
    printf("%d is false|empty", b);
  }

  return ok;
}

static int span_test() {
  u8 v = 0x0f;
  span v_span = { &v, sizeof(v) };

  printf("[%d] == ", *v_span.ptr());
  printf("%d", v);

  *v_span.ptr() = (u8) 0xf0;

  printf("[%d] == ", *v_span.ptr());
  printf("%d", v);

  return ok;
}

static int mmap_test() {
  // mapping an array:
  // -----------------
  int N=5;
  int *ptr = reinterpret_cast<int *> (mmap ( NULL, N*sizeof(int),
                                            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 ));

  if(ptr == MAP_FAILED){
    printf("Mapping Failed\n");
    return 1;
  }

  // Fill the elements of the array
  for(int i=0; i<N; i++)
    ptr[i] = i*10;

  // Print the elements of the array
  printf("The elements of the array => ");
  for(int i=0; i<N; i++)
    printf("[%d] ",ptr[i]);

  printf("\n");
  int err = munmap(ptr, 10*sizeof(int));
  if(err != 0){
    printf("UnMapping Failed\n");
    return 1;
  }

  // mapping memory
  // --------------

  *ptr = *reinterpret_cast<int *> (mmap(NULL,N*sizeof(int),
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED | MAP_ANONYMOUS,
                                        0,0));    

  if(ptr == MAP_FAILED){
    printf("Mapping Failed\n");
    return 1;
  }

  for(int i=0; i < N; i++){
    ptr[i] = i + 1;
  }

  printf("Initial values of the array elements :\n");
  for (int i = 0; i < N; i++ ){
    printf(" %d", ptr[i] );
  }
  printf("\n");

  pid_t child_pid = fork();

  if (child_pid == 0 ){
    //child
    for (int i = 0; i < N; i++){
      ptr[i] = ptr[i] * 10;
    }
  }
  else{
    //parent
    waitpid ( child_pid, NULL, 0);
    printf("Parent:\n");

    printf("Updated values of the array elements :\n");
    for (int i = 0; i < N; i++ ){
      printf(" %d", ptr[i] );
    }
    printf("\n");
  }

  err = munmap(ptr, N*sizeof(int));

  if(err != 0){
    printf("UnMapping Failed\n");
    return 1;
  } else {
    if (child_pid == 0)
      printf("UnMapping Succeeded\n");
  }

  if (child_pid == 0)
    exit(1);

  // mapping file
  // ------------

  char **argv = reinterpret_cast<char**>(malloc(2*sizeof(char)));
  argv[0] = "main";
  argv[1] = "file.hex";
  int argc = sizeof(*argv);

  if(argc < 2){
    printf("File path not mentioned\n");
    exit(0);
  }

  printf("opening file %s\n", argv[1]);
  const char *filepath = argv[1];
  int fd = open(filepath, O_RDWR);
  if(fd < 0){
    printf("\n\"%s \" could not open\n",
           filepath);
    exit(1);
  }

  printf("stating file %s\n", argv[1]);
  struct stat statbuf;
  err = fstat(fd, &statbuf);
  if(err < 0){
    printf("\n\"%s \" could not open\n",
           filepath);
    exit(2);
  }

  printf("mapping file %s [%d] with size %ld\n", argv[1], fd, statbuf.st_size);
  /* char line[1024]; */
  /* scanf("%1023[^\n]", line); */
  *ptr = *reinterpret_cast<int *>(mmap(NULL,statbuf.st_size,
                   PROT_READ|PROT_WRITE,MAP_SHARED,
                   fd,0));
  if(ptr == MAP_FAILED){
    printf("Mapping Failed\n");
    return 1;
  }

  for (usize i = 0; i < statbuf.st_size; i++)
    printf("%d", ptr[i]);

  printf("\n");

  *ptr = 0xf0;
  for (usize i = 0; i < statbuf.st_size; i++) {
    printf("%d", ptr[i]);
  }

  printf("\n");

  printf("closing file %s\n", argv[1]);
  close(fd);

  /* printf("writing to file %s\n", argv[1]); */
  /* ssize_t n = write(1,ptr,statbuf.st_size); */
  /* if(n != statbuf.st_size){ */
  /*   printf("Write failed"); */
  /* } */

  err = munmap(ptr, statbuf.st_size);

  if(err != 0){
    printf("UnMapping Failed\n");
    return 1;
  }

  return ok;
}

int bit_test() {

  u8 v = 42;

  for (s8 i = (sizeof(v)*8); i >= 0; i--)
    printf("{ [%d] = %d }", i, bit_get(&v, i));

  printf("\n");

  for (s8 i = (sizeof(v)*8); i >= 0; i--)
    printf("%d", bit_get(&v, i));

  printf("\n");

  usize i = 0;
  printf("bit %d of %d == %d\n", i, v, bit_get(&v, i));
  i = 3;
  printf("bit %d of %d == %d\n", i, v, bit_get(&v, i));

  return ok;
}

int main(int argc, char **argv) {
  int result = bit_test();
  printf("\n");
  return result;
}
