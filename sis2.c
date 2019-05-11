#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>




#define typename(x) _Generic((x),                                                 \
        _Bool: "_Bool",                  unsigned char: "unsigned char",          \
         char: "char",                     signed char: "signed char",            \
    short int: "short int",         unsigned short int: "unsigned short int",     \
          int: "int",                     unsigned int: "unsigned int",           \
     long int: "long int",           unsigned long int: "unsigned long int",      \
long long int: "long long int", unsigned long long int: "unsigned long long int", \
        float: "float",                         double: "double",                 \
  long double: "long double",                   char *: "pointer to char",        \
       void *: "pointer to void",                int *: "pointer to int",         \
      default: "other")


static int pid_mem = 1;
char* buff;

static void print_data(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        unsigned long end  = 0;
        mm = task->mm;
        printk("\nThis mm_struct has %d vmas.\n", mm->map_count);
        for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
                printk ("\nVma number %d: \n", ++count);
                printk("  Starts at 0x%lx, Ends at 0x%lx\n",
                          vma->vm_start, vma->vm_end);
        end = vma->vm_end;
        }

        printk(
                 "Data  Segment start = 0x%lx, end = 0x%lx\n",
                 
                 mm->start_data, mm->end_data);
    }
static void print_stack(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
    unsigned long end  = 0, i = 0, cnt = 0;
        mm = task->mm;
        printk("\nThis mm_struct has %d vmas.\n", mm->map_count);
        for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
        end = vma->vm_end;
        }

      //printk("OOOK");
        printk("Stack Segment start = 0x%lx\n",
                 mm->start_stack);
    int z = 0;
    //printk("mmStartStack - %ld\n",*(&(mm->start_stack)));
    
    
    int *ptr = (int *)(mm->start_stack);
    printk("value is %d\n", ptr);
    printk("Typename is %s\n", typename(ptr));
    
    
    for (i = mm->start_stack; i <= end; ++i) {
        int *ptr = (int *)(i);
        if (typename(ptr) == "pointer to int") {
            cnt++;
        }
    }
    printk("Counter of integer types is  %d\n", cnt);
    
}
static void print_brk(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        
        mm = task->mm;
        printk("Heap Segment start = 0x%lx  end = 0x%lx\n",
                 mm->start_brk, mm->brk);
    unsigned long diff = mm->brk - mm->start_brk;
    printk("Heap size is %lx", diff);
    printk ("");
}
static void print_code(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        
        mm = task->mm;

        printk("Code Segment start = 0x%lx  end = 0x%lx\n",
                 mm->start_code, mm->end_code);
}

static int mm_exp_load(void){
        struct task_struct *task;
        printk("\nGot the process id to look up as %d.\n", pid_mem);
        for_each_process(task) {
                if ( task->pid == pid_mem) {
                        printk("%s[%d]\n", task->comm, task->pid);
                        print_data(task);
            print_stack(task);
            print_brk(task);
            print_code(task);    
                }
        }
       
        return 0;
}



static void mm_exp_unload(void)
{
        printk("\nPrint segment information module exiting.\n");
}

module_init(mm_exp_load);
module_exit(mm_exp_unload);
module_param(pid_mem, int, 0);