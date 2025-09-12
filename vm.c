/*
Assignment:
vm.c - Implement a P-machine virtual machine

Authors: Jakson Zapata, Jacob Smith

Language: C (only)

To Compile:
gcc -O2 -Wall -std=c11 -o vm vm.c

To Execute (on Eustis):
./vm input.txt

where:
input.txt is the name of the file containing PM/0 instructions;
each line has three integers (OP L M)

Notes:
- Implements the PM/0 virtual machine described in the homework
  instructions.
- No dynamic memory allocation or pointer arithmetic.
- Does not implement any VM instruction using a separate function.
- Runs on Eustis.

Class: COP 3402 - Systems Software - Fall 2025
Instructor : Dr. Jie Lin
Due Date: Friday , September 12th , 2025
*/

#include <stdio.h>
#include <stdlib.h>

// Single process address
static int pas[500] = {0};

// Registers
static int pc = 0; // Program Counter
static int bp = 0; // Base Pointer
static int sp = 0; // Stack Pointer

// Instruction Register
typedef struct
{
  int op;
  int l;
  int m;
} Instr;

static Instr IR;

int base(int BP, int L);
void print_trace(const char *name, int L, int M);

// from HW1 PDF
int base(int BP, int L)
{
  int arb = BP; // activation record base
  while (L > 0)
  {
    arb = pas[arb]; // follow static link
    L--;
  }
  return arb;
}

// print instruction + PC/BP/SP + a readable stack
void print_trace(const char *instr_name, int L, int M)
{
  static int first = 1;
  if (first)
  {
    printf("L M PC BP SP stack\n");
    printf("Initial values: %d %d %d\n", pc, bp, sp);
    first = 0;
    return;
  }

  /* Instruction/regs line */
  printf("%-3s %d %d %d %d %d  ", instr_name, L, M, pc, bp, sp);

  /* Mark activation-record bases by walking static links */
  int bar[500] = {0};
  int walk = bp;
  while (walk > 0 && walk < 500)
  {
    bar[walk] = 1;        /* put a '|' before cell at this index */
    int next = pas[walk]; /* SL is stored at [BP] */
    if (next == walk)
      break; /* defensive: avoid loop if corrupted */
    walk = next;
  }

  /* Decide how far to print:
     - Always print from sp up through the current AR base (bp).
     - Extend further to include caller frames by following SL bases.
  */
  int stop = bp;
  int probe = bp;
  while (probe > 0 && probe < 500 && bar[probe])
  {
    int next = pas[probe]; /* caller's base via SL */
    if (next <= 0 || next >= 500 || next == probe)
      break;
    if (next > stop)
      stop = next; /* include caller base in printout */
    probe = next;
  }

  for (int i = sp; i <= stop; i++)
  {
    if (bar[i])
      printf("| ");
    printf("%d ", pas[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  /* 1) Argument check: exactly one input file */
  if (argc != 2)
  {
    fprintf(stderr, "Error: expected one input file (each line: OP L M).\n");
    return 1;
  }

  /* 2) Load code into PAS from the top (499,498,497 ... downward) */
  FILE *in = fopen(argv[1], "r");
  if (!in)
  {
    fprintf(stderr, "Error: could not open input file: %s\n", argv[1]);
    return 1;
  }

  int load_idx = 499; /* OP goes here; L at -1; M at -2 */
  int op, l, m;
  int last_m_index_used = 500; /* lowest (smallest) address touched by code */

  while (fscanf(in, "%d %d %d", &op, &l, &m) == 3)
  {
    if (load_idx - 2 < 0)
    {
      fprintf(stderr, "Error: program too large for PAS.\n");
      fclose(in);
      return 1;
    }
    pas[load_idx] = op;
    pas[load_idx - 1] = l;
    pas[load_idx - 2] = m;

    last_m_index_used = load_idx - 2; /* track lowest code addr used */
    load_idx -= 3;
  }
  fclose(in);

  /* 3) Initialize registers per spec */
  pc = 499;               /* before first fetch */
  sp = last_m_index_used; /* top of stack starts right below code */
  bp = sp - 1;            /* base is one above the (empty) stack */

  /* Print initial line */
  print_trace("INIT", 0, 0);

  /* 4) Fetch–Execute loop */
  int halt = 0;
  while (!halt)
  {
    /* ---- Fetch (code stored downward; 3 ints per instruction) ----
       IR.op = pas[pc];
       IR.l  = pas[pc-1];
       IR.m  = pas[pc-2];
       pc    = pc - 3;
    */
    IR.op = pas[pc];
    IR.l = pas[pc - 1];
    IR.m = pas[pc - 2];
    pc -= 3;

    /* ---- Execute ---- */
    switch (IR.op)
    {
    case 1:
    { /* LIT 0 n : push literal n */
      sp = sp - 1;
      pas[sp] = IR.m;
      print_trace("LIT", IR.l, IR.m);
    }
    break;

    case 2:
    { /* OPR 0 m : arithmetic / logical / return */
      switch (IR.m)
      {
      case 0:
      {
        sp = bp + 1;
        bp = pas[sp - 2]; /* DL */
        pc = pas[sp - 3]; /* RA */
        print_trace("RTN", IR.l, IR.m);
      }
      break;

      case 1: /* ADD */
        pas[sp + 1] = pas[sp + 1] + pas[sp];
        sp = sp + 1;
        print_trace("ADD", IR.l, IR.m);
        break;

      case 2: /* SUB */
        pas[sp + 1] = pas[sp + 1] - pas[sp];
        sp = sp + 1;
        print_trace("SUB", IR.l, IR.m);
        break;

      case 3: /* MUL */
        pas[sp + 1] = pas[sp + 1] * pas[sp];
        sp = sp + 1;
        print_trace("MUL", IR.l, IR.m);
        break;

      case 4: /* DIV (integer) */
        pas[sp + 1] = pas[sp + 1] / pas[sp];
        sp = sp + 1;
        print_trace("DIV", IR.l, IR.m);
        break;

      case 5: /* EQL */
        pas[sp + 1] = (pas[sp + 1] == pas[sp]);
        sp = sp + 1;
        print_trace("EQL", IR.l, IR.m);
        break;

      case 6: /* NEQ */
        pas[sp + 1] = (pas[sp + 1] != pas[sp]);
        sp = sp + 1;
        print_trace("NEQ", IR.l, IR.m);
        break;

      case 7: /* LSS */
        pas[sp + 1] = (pas[sp + 1] < pas[sp]);
        sp = sp + 1;
        print_trace("LSS", IR.l, IR.m);
        break;

      case 8: /* LEQ */
        pas[sp + 1] = (pas[sp + 1] <= pas[sp]);
        sp = sp + 1;
        print_trace("LEQ", IR.l, IR.m);
        break;

      case 9: /* GTR */
        pas[sp + 1] = (pas[sp + 1] > pas[sp]);
        sp = sp + 1;
        print_trace("GTR", IR.l, IR.m);
        break;

      case 10: /* GEQ */
        pas[sp + 1] = (pas[sp + 1] >= pas[sp]);
        sp = sp + 1;
        print_trace("GEQ", IR.l, IR.m);
        break;

      default:
        fprintf(stderr, "Runtime error: invalid OPR subcode %d\n", IR.m);
        return 1;
      }
    }
    break;

    case 3:
    { /* LOD L a : load from base(bp,L)-a */
      sp = sp - 1;
      pas[sp] = pas[base(bp, IR.l) - IR.m];
      print_trace("LOD", IR.l, IR.m);
    }
    break;

    case 4:
    { /* STO L o : store top into base(bp,L)-o; pop */
      pas[base(bp, IR.l) - IR.m] = pas[sp];
      sp = sp + 1;
      print_trace("STO", IR.l, IR.m);
    }
    break;

    case 5:
    { /* CAL L a : call procedure; create AR */
      /* Reserve 3 words just above current top (stack grows down):
         [sp-1] = SL = base(bp, L)
         [sp-2] = DL = bp
         [sp-3] = RA = pc
         Then set bp and jump.
      */
      pas[sp - 1] = base(bp, IR.l); /* SL */
      pas[sp - 2] = bp;             /* DL */
      pas[sp - 3] = pc;             /* RA (pc already moved past fetch) */
      bp = sp - 1;
      pc = IR.m;
      print_trace("CAL", IR.l, IR.m);
    }
    break;

    case 6:
    { /* INC 0 n : allocate n locals */
      sp = sp - IR.m;
      print_trace("INC", IR.l, IR.m);
    }
    break;

    case 7:
    { /* JMP 0 a : unconditional jump */
      pc = IR.m;
      print_trace("JMP", IR.l, IR.m);
    }
    break;

    case 8:
    { /* JPC 0 a : jump if top == 0; then pop */
      if (pas[sp] == 0)
      {
        pc = IR.m;
      }
      sp = sp + 1;
      print_trace("JPC", IR.l, IR.m);
    }
    break;

    case 9:
    { /* SYS 0 {1|2|3} */
      if (IR.m == 1)
      {
        /* Output top of stack; then pop */
        printf("Output result is: %d\n", pas[sp]);
        sp = sp + 1;
        print_trace("SYS", IR.l, IR.m);
      }
      else if (IR.m == 2)
      {
        /* Read integer with exact prompt */
        int x;
        printf("Please Enter an Integer: ");
        fflush(stdout);
        if (scanf("%d", &x) != 1)
        {
          fprintf(stderr, "Input error.\n");
          return 1;
        }
        sp = sp - 1;
        pas[sp] = x;
        print_trace("SYS", IR.l, IR.m);
      }
      else if (IR.m == 3)
      {
        /* Halt */
        print_trace("SYS", IR.l, IR.m);
        halt = 1;
      }
      else
      {
        fprintf(stderr, "Runtime error: invalid SYS code %d\n", IR.m);
        return 1;
      }
    }
    break;

    default:
      fprintf(stderr, "Runtime error: invalid opcode %d\n", IR.op);
      return 1;
    }
  }

  return 0;
}
