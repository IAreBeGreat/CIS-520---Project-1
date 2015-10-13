#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);
static int getArgument(struct intr_frame *, int);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

 static void
 syscall_handler (struct intr_frame *f UNUSED) 
{ 
  int syscall_num;
  struct fd_file_pair *f_pair;
  memcpy( &syscall_num, f->esp, sizeof(int));
  
  switch(syscall_num)
  { 
    case(SYS_HALT):
      shutdown_power_off();
      break;
    case(SYS_EXIT):
      f->eax = 0;
      thread_exit();
      break;
    case(SYS_EXEC):
    {
		
      thread_exit();
      break;
    }
    case(SYS_WAIT):
      printf("wait call");
      thread_exit();
      break;
    case(SYS_CREATE):
      f->eax = filesys_create(getArgument(f,1),getArgument(f,2));
      break;
    case(SYS_REMOVE):
      f->eax = filesys_remove(getArgument(f,1));
      break;
    case(SYS_OPEN):
      f_pair = calloc(1, sizeof (struct fd_file_pair));
      f_pair->file = file_open(file_get_inode(getArgument(f,1)));
      if(f_pair->file == NULL)
      {
        printf("File opening failed");
        f->eax = -1;
        thread_exit();
        break;
      }
      int fd = thread_current()->fd_index;
      thread_current()->fd_index++;
      f_pair->fd = fd;
      list_push_front(&thread_current()->fd_file_list, &f_pair->file_elem);
      f->eax=fd;
      break;
    case(SYS_FILESIZE):
      printf("filesize call");
      thread_exit();
      break;
    case(SYS_READ): 
      if(getArgument(f,1))
        input_getc();
      else if(getArgument(f,1)<thread_current()->fd_index)
      {
        f_pair = get_fd_file_pair(fd);
        f->eax=(file_read(f_pair->file, getArgument(f,2), getArgument(f,3)));
      }
      break;
    case(SYS_WRITE):
      if(getArgument(f,1) == 1)
		    putbuf(getArgument(f,2), getArgument(f,3));
		  else if(getArgument(f,1)>1 && getArgument(f,1) < thread_current()->fd_index)
		     file_write(get_fd_file_pair(getArgument(f,1))->file, getArgument(f,2), getArgument(f,3));
      break;
    case(SYS_SEEK):
      printf("seek call");
      thread_exit();
      break;
    case(SYS_TELL):
      if(getArgument(f,1)<2 || getArgument(f,1) > thread_current()->fd_index)
      {
        printf("tell fail"); 
        thread_exit();
      }
      f->eax = file_tell(get_fd_file_pair(getArgument(f,1))->file);
      break;
    case(SYS_CLOSE):
      if(getArgument(f,1)<2 || getArgument(f,1) > thread_current()->fd_index)
      {
        printf("close fail"); 
        thread_exit();
      }
      f_pair = get_fd_file_pair(getArgument(f,1));
      file_close(f_pair->file);
      break;
    default: 
      printf("default, something went wrong");
      thread_exit();
  }
}

int write (int fd, const void *buffer, unsigned size)
{
  if (fd == 1)
    {
      putbuf(buffer, size);
      return size;
    }
  return 0;
}

static int 
getArgument(struct intr_frame *f, int index)
{
  int arg1;
  memcpy( &arg1, ((int *)(f->esp))+index, sizeof(int));
  return arg1;
}
  
 
