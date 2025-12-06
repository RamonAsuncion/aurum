#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

struct interpreter_state {
  struct stack *stack;
  struct stack *loop_stack;
  struct stack *end_stack;
  struct hashmap *hashmap;
  char *memory;
};

struct interpreter_state *interpreter_init(void);
void interpreter_run(struct interpreter_state *state, const char *source_code);
void interpreter_cleanup(struct interpreter_state *state);

#endif /* _INTERPRETER_H_ */

