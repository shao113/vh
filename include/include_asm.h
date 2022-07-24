#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#ifndef INCLUDE_ASM

#define INCLUDE_ASM(FOLDER, NAME)                                                                  \
   __asm__(".section .text\n"                                                                      \
           "\t.align\t2\n"                                                                         \
           "\t.globl\t" #NAME "\n"                                                                 \
           "\t.ent\t" #NAME "\n" #NAME ":\n"                                                       \
           ".include \"" FOLDER "/" #NAME ".s\"\n"                                                 \
           "\t.set reorder\n"                                                                      \
           "\t.set at\n"                                                                           \
           "\t.end\t" #NAME);
#endif

// omit .global
__asm__(".include \"include/include_asm.inc\"\n");

#endif
