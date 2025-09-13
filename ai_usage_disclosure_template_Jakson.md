# AI Usage Disclosure Details

**Student Name:** Jakson Zapata
**Student ID:** 5431370
**Assignment:** HW1

---

## Instructions

Complete this template with detailed information about your AI usage. Submit this file along with your signed PDF declaration form.

---

## AI Tool #1

### Tool Name
ChatGPT

### Version/Model
ChatGPT 5

### Date(s) Used
9/11/25 - 9/12/25

### Specific Parts of Assignment
I used ChatGPT for the file I/O part of this assignment because I didn’t know how to set it up in C. The code for opening the file with fopen, reading triples of integers (OP, L, M) with fscanf in a loop, tracking the lowest index, and closing the file came almost entirely from AI.

I also used it for debugging: JMP 0 45 → PC = 454 (It was jumping directly to 45, not going down from 499)

### Prompts Used
"Can you write the code that uses fopen, reads OP L M from a file line by line into my array, and then closes the file?"  (I gave it an input example)

"Can you check why I'm getting  JMP 0 45 45 439 440 Runtime error: invalid opcode 0?"

### AI Output/Results

The AI generated code that included fopen to open the input file, a while loop to read three integers (OP, L, M) per line. It Storred those values into my PAS array. It also included some error checking and called fclose at the end.

"You jumped to address 45 literally. But in this assignment the code lives at the top of PAS (index 499 downward), and the jump targets in the input are given as offsets from 499, not raw PAS indices."
It suggested me to add "static const int CODE_TOP = 499;   // first OP is stored at pas[499]"  and do "pc = CODE_TOP - IR.m;" That CODE_TOP was also used on JPC and CAL.


### How Output was Verified/Edited

I compiled the code on Eustis using the gcc -Wall command and with the input.txt file to check that instructions were being read correctly into the PAS array. I didn’t have to make many edits to the file I/O code since it worked great, but I adjusted variable names and changed some formatting so it matched the rest of our code.

I added the simple suggestion of adding the static const int CODE_TOP = 499 and changing the calculation of the Jump and it solved the problem.

### Multiple Iterations (if applicable)


### Learning & Reflection
ChatGPT helped me understand how to use file I/O since it didn't just give me the code/template, it gave me an explanation of what each line and command did.

It also helped me debug the problem that our code had, it was really simple but it was giving us a hard time. I think AI's are really good at finding those small mistakes that may be hard for us to even see.

---

## AI Tool #2 (if applicable)

### Tool Name
[Tool name if you used a different AI tool]

### Version/Model
[Version/model information]

### Date(s) Used
[Dates when this tool was used]

### Specific Parts of Assignment
[Which parts of the assignment involved this tool]

### Prompts Used
[Exact prompts given to this tool]

### AI Output/Results
[What this tool provided]

### How Output was Verified/Edited
[How you verified/modified the output from this tool]

### Multiple Iterations (if applicable)
[Conversation flow if multiple interactions occurred]

### Learning & Reflection
[What you learned from using this tool]

---

## AI Tool #3 (if applicable)

### Tool Name
[Tool name if you used a third AI tool]

### Version/Model
[Version/model information]

### Date(s) Used
[Dates when this tool was used]

### Specific Parts of Assignment
[Which parts of the assignment involved this tool]

### Prompts Used
[Exact prompts given to this tool]

### AI Output/Results
[What this tool provided]

### How Output was Verified/Edited
[How you verified/modified the output from this tool]

### Multiple Iterations (if applicable)
[Conversation flow if multiple interactions occurred]

### Learning & Reflection
[What you learned from using this tool]

---

## Overall Reflection

[Provide an overall reflection on your AI usage for this assignment]
[Consider: How did AI tools help your learning? What did you understand better? How did you ensure the work remained your own?]

---

## Notes

- Be as specific and detailed as possible in your responses
- Include exact prompts and AI outputs when possible
- Explain how you verified and modified AI-generated content
- Reflect on what you learned through the AI interaction
