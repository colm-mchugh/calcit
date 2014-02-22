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

bool is_assign_stmt(Context *ctx) {
	return (list_size(ctx->parse_tree) == 1) &&
		((Node*)getElement(ctx->parse_tree, 0))->type == T_ASSIGN;
}

bool zero_errors(Context* ctx) {
	return (list_size(ctx->errors) == 0);
}

void cleanUpContext(Context* ctx) {
	// Delete parse_tree memory. Don't delete assign nodes because they are in symbol table
	deleter del_fn = ((zero_errors(ctx) && is_assign_stmt(ctx)) ? NULL : genericDeleteNode);
	deleteList(&ctx->parse_tree, del_fn);
	// Delete error messages
	while(list_size(ctx->errors) > 0) {
		ErrorMessage *em = (ErrorMessage*)removeElement(ctx->errors, 0);
		free(em);
	}
}
void deleteContext(Context **ctx_ptr) {
	if ((ctx_ptr != NULL) && (*ctx_ptr != NULL)) {
		Context *ctx = *ctx_ptr;
		deleteList(&ctx->parse_tree, genericDeleteNode);
		deleteHash(&ctx->symbol_table, genericDeleteNode);
		free(ctx->eval_ops);
		deleteList(&ctx->errors, NULL);
		free(ctx);
		*ctx_ptr = NULL;
	}
}


