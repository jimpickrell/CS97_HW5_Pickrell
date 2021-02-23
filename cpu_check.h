// =============== cpu_check.h
#ifndef CPUCHECK
#define CPUCHECK

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

//static 
struct cpuid cpuid (unsigned int leaf, unsigned int subleaf);
//static 
_Bool rdrand_supported (void);
//static 
void hardware_rand64_init (void);

#endif