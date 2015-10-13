#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <user/syscall.h>
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
<<<<<<< HEAD
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
=======
#include "filesys/file.h"
>>>>>>> 7e78dbe2516a04115c4dfdca837e663eb7fbcd08

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
  int *syscall_num = f->esp;
  switch(*syscall_num)
  { 
    case(SYS_HALT):
      shutdown_power_off();
      break;
    case(SYS_EXIT):
<<<<<<< HEAD
    {
      exit(getArgument(f,1));
=======
      f->eax = 0;
      thread_exit();
>>>>>>> 7e78dbe2516a04115c4dfdca837e663eb7fbcd08
      break;
    }
    case(SYS_EXEC):
    {
		f->eax = exec((const char *)getArgument(f,1));
      break;
    }
    case(SYS_WAIT):
      f->eax = wait(getArgument(f,1));
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
<<<<<<< HEAD
    {
      if(getArgument(f,1) == 1)
		    write(getArgument(f,1),getArgument(f,2),getArgument(f,3)); 
=======
      if(getArgument(f,1) == 1)
		    putbuf(getArgument(f,2), getArgument(f,3));
		  else if(getArgument(f,1)>1 && getArgument(f,1) < thread_current()->fd_index)
		     file_write(get_fd_file_pair(getArgument(f,1))->file, getArgument(f,2), getArgument(f,3));
>>>>>>> 7e78dbe2516a04115c4dfdca837e663eb7fbcd08
      break;
    }
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

<<<<<<< HEAD
void exit(int status)
{
	struct thread *cur = thread_current();
	if(thread_alive(cur->parent))
	{
		cur->cp->status = status;
	}
	printf("%s: exit(%d)\n",cur->name,status);
	thread_exit();
}

pid_t exec (const char *cmd_line)
{
	pid_t pid  = process_execute(cmd_line);
	return pid;
}
=======
//pid_t exec (const char *cmd_line)
//{
	//pid_t = process_execute(cmd_line);
//}
>>>>>>> 7e78dbe2516a04115c4dfdca837e663eb7fbcd08

int wait(pid_t pid)
{
	return process_wait(pid);
}

static int getArgument(struct intr_frame *f, int index)
{
  int arg1;
  memcpy( &arg1, ((int *)(f->esp))+index, sizeof(int));
  return arg1;
}

struct child_process* add_child(int pid)
{
	struct child_process* cp = malloc(sizeof(struct child_process));
	cp->pid = pid;
	cp->wait = false;
	cp->exit = false;
	list_push_back(&thread_current()->child_list, &cp->elem);
	return cp;
}

struct child_process* get_child(pid)
{
	struct thread * t = thread_current();
	struct list_elem* e;
	struct child_process* cp;
	for(e = list_begin(&t->child_list); e != list_end(&t->child_list); e = list_next(e))
	{
		cp =  list_entry(e, struct child_process, elem);
		if(cp->pid == pid)
		{
			return cp;
		}
	}
	return NULL;
}

void remove_child(struct child_process * cp)
{
	list_remove(&cp->elem);
	free(cp);
}
 
