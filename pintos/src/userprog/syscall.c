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
      printf("create call");
      thread_exit();
      break;
    case(SYS_REMOVE):
      printf("remove call");
      thread_exit();
      break;
    case(SYS_OPEN):
      printf("open call");
      thread_exit();
      break;
    case(SYS_FILESIZE):
      printf("filesize call");
      thread_exit();
      break;
    case(SYS_READ):
      printf("read call");
      thread_exit();
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
      printf("tell call");
      thread_exit();
      break;
    case(SYS_CLOSE):
      printf("close call");
      thread_exit();
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

//pid_t exec (const char *cmd_line)
//{
	//pid_t = process_execute(cmd_line);
//}

static int 
getArgument(struct intr_frame *f, int index)
{
  int arg1;
  memcpy( &arg1, ((int *)(f->esp))+index, sizeof(int));
  return arg1;
}
  
 
