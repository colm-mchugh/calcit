#include "context.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *error_message_table[] = { "No such variable", "Incorrect syntax" };

ErrorMessage *makeError(ErrorTag error_tag, char* data) {
	ErrorMessage *em = (ErrorMessage*)malloc(sizeof(ErrorMessage));
	em->error_tag = error_tag;
	sprintf(em->message, "%s: %s", error_message_table[error_tag], data);
	return em;
}

int iadd(int x, int y) { 
	return x + y; 
}

int isub(int x, int y) { 
	return x - y; 
}

int idiv(int x, int y) { 
	return x / y; 
}

int imul(int x, int y) { 
	return x * y; 
}

float fadd(float x, float y) { 
	return x + y; 
}

float fsub(float x, float y) { 
	return x - y; 
}

float fdiv(float x, float y) { 
	return x / y; 
}

float fmul(float x, float y) { 
	return x * y; 
}

EvalOps *createEvalOps() {
	EvalOps *eval_ops = (EvalOps*)malloc(sizeof(EvalOps));
	eval_ops->iops['+'] = iadd;
	eval_ops->iops['-'] = isub;
	eval_ops->iops['/'] = idiv;
	eval_ops->iops['*'] = imul;
	eval_ops->flops['+'] = fadd;
	eval_ops->flops['-'] = fsub;
	eval_ops->flops['/'] = fdiv;
	eval_ops->flops['*'] = fmul;
	return eval_ops;
}

ExprValue doEval(Context *ctx, ExprValue l, ExprValue r, char opcode) {
	ExprValue result;
	if (l.type == T_FLT_CONST || r.type == T_FLT_CONST) {
		result.type = T_FLT_CONST;
		if (l.type == T_INT_CONST) {
			l.value.float_value = l.value.int_value;
		}
		if (r.type == T_INT_CONST) {
			r.value.float_value = r.value.int_value;
		}
		flop fop = ctx->eval_ops->flops[opcode];
		result.value.float_value = fop(l.value.float_value, r.value.float_value);
	} else {
		result.type = T_INT_CONST;
		iop op = ctx->eval_ops->iops[opcode];
		result.value.int_value = op(l.value.int_value, r.value.int_value);
	}
	return result;
}

Context *createContext() {
	Context *new_context = (Context*)malloc(sizeof(Context));
	new_context->symbol_table = createHash(DEFAULT_NUM_BUCKETS);
	new_context->errors = createList();
	new_context->eval_ops = createEvalOps();
	return new_context;
}

bool is_assign(List *parse_tree) {
	return (list_size(parse_tree) == 1) &&
		((Node*)getElement(parse_tree, 0))->type == T_ASSIGN;
}

void cleanUpContext(Context* ctx) {
	if((list_size(ctx->errors) == 0) && is_assign(ctx->parse_tree)) {
		removeElement(ctx->parse_tree, 0);
		// don't remove the assign node itself,
		// it is in the symbol table
	}
	while(list_size(ctx->errors) > 0) {
		ErrorMessage *em = (ErrorMessage*)removeElement(ctx->errors, 0);
		free(em);
	}
	int num_nodes = list_size(ctx->parse_tree);
	// Remove all the nodes from the list
	for (int i = 0; i < num_nodes; i++) {
		Node *n = (Node*)removeElement(ctx->parse_tree, 0);
		deleteNode(n);
	}
}
void deleteContext(Context *ctx) {
	deleteList(&ctx->parse_tree, genericDeleteNode);
	deleteHash(&ctx->symbol_table, genericDeleteNode);
	free(ctx->eval_ops);
	deleteList(&ctx->errors, NULL);
}


