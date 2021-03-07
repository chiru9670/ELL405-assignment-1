#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Add the 2 given int arguments and return the sum
int 
sys_add(void) 
{
  int num1, num2;
  argint(0, &num1);
  argint(1, &num2);
  return num1 + num2;
}

// Globals
#ifndef SYS_fork  // If the macros in syscall.h are not yet defined
#include "syscall.h"
#endif
/*
* Stores names of all system calls
*/
const char *sysCallName[] = {
[SYS_fork]          "sys_fork",
[SYS_exit]          "sys_exit",
[SYS_wait]          "sys_wait",
[SYS_pipe]          "sys_pipe",
[SYS_read]          "sys_read",
[SYS_kill]          "sys_kill",
[SYS_exec]          "sys_exec",
[SYS_fstat]         "sys_fstat",
[SYS_chdir]         "sys_chdir",
[SYS_dup]           "sys_dup",
[SYS_getpid]        "sys_getpid",
[SYS_sbrk]          "sys_sbrk",
[SYS_sleep]         "sys_sleep",
[SYS_uptime]        "sys_uptime",
[SYS_open]          "sys_open",
[SYS_write]         "sys_write",
[SYS_mknod]         "sys_mknod",
[SYS_unlink]        "sys_unlink",
[SYS_link]          "sys_link",
[SYS_mkdir]         "sys_mkdir",
[SYS_close]         "sys_close",
[SYS_add]           "sys_add",
[SYS_toggle]        "sys_toggle",
[SYS_print_count]   "sys_print_count",
[SYS_ps]            "sys_ps",
};
int numSysCalls[NELEM(sysCallName)] = {0};
int toggle_state = 0;

int 
sys_toggle(void) 
{
  if(toggle_state==0){
    toggle_state=1;
    for(int i=0;i<NELEM(sysCallName);i++){
      numSysCalls[i] = 0;
    }
  }else{
    toggle_state=0;
  }
  return 0;
}

int 
sys_print_count(void) 
{
  for(int i=1; i<NELEM(sysCallName);i++){
    cprintf("%s %d\n",sysCallName[i],numSysCalls[i]);
  }
  return 0;
}

int 
sys_ps(void) 
{
  process_analyzer();
  return 0;
}
