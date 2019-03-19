#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb, const char *filename);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *sb, char *name);
/* -------------------------------------------------------------------------------- */

#define BUFSIZE 512


int  main(int argc, char *argv[])
{
  struct stat sb;
  
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  if (lstat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_SUCCESS);
  }
  
  print_type(&sb);
  print_name(&sb, argv[1]);
  print_ino(&sb);
  print_perms(&sb, argv[1]);
  print_linkc(&sb);
  print_owner(&sb);
  print_size(&sb);
  print_laststch(&sb);
  print_lastacc(&sb);
  print_lastmod(&sb);
  
  exit(EXIT_SUCCESS);
}
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb){
  printf("File type:                ");
  switch (sb->st_mode & S_IFMT) {
  case S_IFLNK:  printf("symlink\n");                 break;
  case S_IFREG:  printf("regular file\n");            break;
  case S_IFBLK:  printf("block device\n");            break;
  case S_IFDIR:  printf("directory\n");               break;
  case S_IFCHR:  printf("character device\n");        break;
  case S_IFIFO:  printf("FIFO queue\n");              break;
  default:       printf("unknown?\n");                break;
  }
}
/* -------------------------------------------------------------------------------- */

static void print_ino(const struct stat *sb){
  printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------------------- */

// Funkcja pomocnicza.
static const char* yes_no(int x){
  return x ? "yes" : "no";
}

static void print_perms(const struct stat *sb, const char *filename){
  unsigned long x = sb->st_mode & 0777;
  printf("Mode:                     %lo (octal)\n", x);
  printf("  (your permissions: read: %s, write: %s, execute: %s)\n",
      yes_no( !access(filename, R_OK) ),
      yes_no( !access(filename, W_OK) ),
      yes_no( !access(filename, X_OK) )
  );
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
  struct passwd *pws = getpwuid(sb->st_uid);
  struct group *grp = getgrgid(sb->st_gid);

  printf("Ownership:                UID=%s(%ld)   GID=%s(%ld)\n",
    pws->pw_name, (long)sb->st_uid, grp->gr_name, (long)sb->st_gid);
}
/* -------------------------------------------------------------------------------- */

static void print_size(const struct stat *sb){
  float formatted_size = (float)(sb->st_size);
  const char *units[] = {"", "k", "M", "G", "T"};
  int unit_idx = 0;

  while(formatted_size > 1023 && unit_idx < 4){
    formatted_size /= 1024;
    ++unit_idx;
  }

  printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
  printf("File size:                %.1f %sB\n", formatted_size, units[unit_idx]);
  printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */

static void print_laststch(const struct stat *sb){
  printf("Last status change:       %s", ctime(&sb->st_ctime));
}
/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
  printf("Last file access:         %s", ctime(&sb->st_atime));
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
  printf("Last file modification:   %s", ctime(&sb->st_mtime));
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
  char* bname = basename(name);

  if(S_ISLNK(sb->st_mode)){
    char realname[BUFSIZE];
    int nbytes = readlink(name, realname, BUFSIZE - 1);
    realname[nbytes] = '\0';

    printf("Name of the file:         %s -> %s\n", bname, realname);
  }else
    printf("Name of the file:         %s\n", bname);
}
/* -------------------------------------------------------------------------------- */
