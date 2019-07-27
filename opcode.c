#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "opcode.h"

struct opcode_desc *lookup_1byte_opcode_map(uint8_t index)
{
	return &one_byte_opcode_map[index];
}

struct opcode_desc *lookup_2byte_opcode_map1(uint8_t index)
{

}

struct opcode_desc *lookup_2byte_opcode_map2(uint8_t index)
{

}

struct opcode_desc *lookup_3byte_opcode_map1(uint8_t index)
{

}

struct opcode_desc *lookup_3byte_opcode_map2(uint8_t index)
{

}

int select_operand_size(int op_flag, struct rex_prefix *rex)
{
	int op_size = -1;

        /* 64bit size. */
        if (rex->w == 1) {
                op_size = 8;
        }
        else {
                /* 16bit size. */
                if (op_flag == 1) {
                        op_size = 2;
                }
                /* 32bit size. */
                else {
                        op_size = 4;
                }
        }

	return op_size;
}

int select_operand_type(int op_size, char *op_desc)
{
	int n = op_size;

	if (!op_desc)
		return n;

        switch (op_desc[1]) {
        case 'b':
                /* force 1 byte. */
                n = 1;
                break;
        case 'w':
                /* force 2 byte. */
                n = 2;
                break;
        case 'd':
                /* force 4 byte. */
                n = 4;
                break;
        case 'q':
                /* force 8 byte. */
                n = 8;
                break;
        case 'v':
/*
                if (op_desc[0] == 'I')
                        n = 4;
                else
*/
                n = op_size;
                break;
        case 'a':
                break;
        case 'c':
                break;
        }

        return n;
}

int convert_size(int size)
{
	int n = size;

        if (size == 8)
                n = 4;
        if (size == 4)
                n = 3;

	return n;
}

char mnemonic_size[5] = {'b', 'w', 'l', 'q'};

int select_mnemonic_size(int op_flag, struct rex_prefix *rex)
{
        int n;

        n = select_operand_size(op_flag, rex);
        switch (n) {
        case 1:
                return mnemonic_size[0];
        case 2:
                return mnemonic_size[1];
        case 4:
                return mnemonic_size[2];
        case 8:
                return mnemonic_size[3];
        default:
                return -1;
        }
}

int get_operand_size(int op_flag, char *op_desc, struct rex_prefix *rex)
{
	int op_size, n;

	op_size = select_operand_size(op_flag, rex);
	if (op_size == -1)
		return -1;

	n = select_operand_type(op_size, op_desc);
	if (n == -1)
		return -1;

	return convert_size(n);
}

char *__select_effective_reg(int index, char op_flag, 
			     char *op_desc, 
			     struct rex_prefix *rex, 
			     int rex_flag)
{
	char *reg;
	int n;

	n = get_operand_size(op_flag, op_desc, rex);
	if (n == -1)
		return NULL;

	if (rex_flag == 1) {
		reg = mod_reg_ex[(n - 1) * 8 + index];
	}
	else {
		reg = mod_reg_general[n * 8 + index];
	}

	return reg;
}

char *select_effective_reg(int index, char op_flag, 
			char *op_desc, struct rex_prefix *rex)
{
	return __select_effective_reg(index, op_flag, op_desc, rex, rex->b);
}

char *select_general_reg(int index, char op_flag, 
			char *op_desc, struct rex_prefix *rex)
{
	return __select_effective_reg(index, op_flag, op_desc, rex, rex->r);
}

void parse_sib(uint8_t value, struct sib *sib)
{
	sib->scale = value >> 6;
	sib->index = (value >> 3) & 0x7;
	sib->base = value & 0x7;
}

void parse_mod_reg(uint8_t value, struct mod_reg *reg)
{
        reg->mod = value >> 6;
        reg->reg = (value >> 3) & 0x7;
        reg->rm = value & 0x7;
}

void decode_rex(uint8_t value, struct rex_prefix *rex)
{
        rex->w = (value & 0x8) >> 3;
        rex->r = (value & 0x4) >> 2;
        rex->x = (value & 0x2) >> 1;
        rex->b = value & 0x1;
}

void print_rex(struct rex_prefix *rex)
{
        printf("decode: rex.w=%d rex.r=%d rex.x=%d rex.b=%d\n",
               rex->w, rex->r, rex->x, rex->b);
}

void print_mod_reg(struct mod_reg *reg)
{
        printf("modr/m: mod=%d reg=%d rm=%d\n", 
		reg->mod, reg->reg, reg->rm);
}

void print_sib(struct sib *sib)
{
        printf("sib: scale=%d, index=%d, base=%d\n", 
		sib->scale, sib->index, sib->base);
}

int is_rex_enable(struct rex_prefix *rex)
{
	if (rex->w == 1 || rex->r == 1 || rex->x == 1 || rex->b == 1)
		return 1;
	return 0;
}

int is_regex_enable(struct rex_prefix *rex)
{
	if (rex->r == 1 || rex->x == 1 || rex->b == 1)
		return 1;
	return 0;
}

int select_reg_index(char *reg, struct rex_prefix *rex)
{
	int i = 0;

	if (is_regex_enable(rex)) {
		for (i = 0; i < 8; i++) {
			if (!strcmp(mod_reg_ex[24 + i], reg))
				return i % 8;
		}
		return -1;
	}

	for (i = 0; i < 8; i++) {
		if (!strcmp(mod_reg_general[16 + i], reg))
			return i % 8;
	}
	return -1;
}

int get_effective_reg(char *op_desc, char *reg_buf, 
			struct x86_opcode *op_code)
{
	struct rex_prefix *rex = &op_code->op_rex;
	char arg0[16], arg1[16];
	char *reg, *p;
	int op_size, index = -1;

        strcpy(arg0, op_desc);
        p = strchr(arg0, '/');
        if (p) {
        	strcpy(arg1, p + 1);
                *p = 0;
        }

        if (op_desc[0] != 'r' && op_desc[0] != 'e') {
		if (is_regex_enable(rex))
			strcpy(reg_buf, arg1);
		else
			strcpy(reg_buf, arg0);
		return 0;
	}

	if (op_desc[0] == 'e' && op_desc[1] == 's') {
		strcpy(reg_buf, arg0);
		return 0;
	}

	if (is_regex_enable(rex) && arg1[0] != '\0')
		p = arg1;
	else
		/* escape the 'r' or 'e' prefix. */
		p = arg0 + 1;

	index = select_reg_index(p, rex);
	if (index == -1)
		return -1;

	reg = select_effective_reg(index, 
			op_code->op_size, NULL, rex);
	if (!reg)
		return -1;

	strcpy(reg_buf, reg);
	return 0;
}

char *select_sib_reg(int index, char op_flag, 
		struct rex_prefix *rex, int rex_flag)
{
	char *reg;
        int op_size, n;

        op_size = select_operand_size(op_flag, rex);
        if (op_size == -1)
                return NULL;

        n = convert_size(op_size);
        if (rex_flag == 1) {
                reg = mod_reg_ex[(n - 1) * 8 + index];
        }
        else {
                reg = mod_reg_general[n * 8 + index];
        }

	return reg;
}

int select_sib_ss_index(char *op_buf, int op_buf_size,
		struct x86_opcode *op_code)
{
	struct rex_prefix *rex = &op_code->op_rex;
	struct sib *sib = &op_code->op_sib;
	char *reg;
		
	if (sib->index == 0x4 && rex->x == 0)
		return 0;

	reg = select_sib_reg(sib->index, op_code->op_size, rex, rex->x);
	if (!reg)
		return -1;

	switch (sib->scale) {
	case 0x0:
		snprintf(op_buf, op_buf_size - 1, "%s", reg);
		break;
	case 0x1:
		snprintf(op_buf, op_buf_size - 1, "%s*2", reg);
		break;
	case 0x2:
		snprintf(op_buf, op_buf_size - 1, "%s*4", reg);
		break;
	case 0x3:
		snprintf(op_buf, op_buf_size - 1, "%s*8", reg);
		break;
	default:
		return -1;
	}

	printf("si: %s\n", op_buf);
	return 0;
}

int select_sib_base(char *op_string, char *op_buf, 
		int op_buf_size, struct x86_opcode *op_code)
{
	struct rex_prefix *rex = &op_code->op_rex;
	struct mod_reg *reg = &op_code->op_reg;
	struct sib *sib = &op_code->op_sib;
	char *reg_name, *s = op_string;
	int n = 0, disp;
		
	reg_name = select_sib_reg(sib->base, 
				op_code->op_size, rex, rex->b);
	if (!reg_name)
		return -1;

	if (sib->base != 0x5) {
		snprintf(op_buf, op_buf_size, "%s", reg_name);
		printf("b: %s\n", op_buf);
		return 0;
	}
		
	switch (reg->mod) {
	case 0x0:
		disp = *(uint32_t *)s;
		n = 4;
		snprintf(op_buf, op_buf_size, "0x%x", disp);
		break;
	case 0x1:
		disp = *(uint8_t *)s;
		n = 1;
		snprintf(op_buf, op_buf_size, "%s+0x%x", reg_name, disp);
		break;
	case 0x2:
		disp = *(uint32_t *)s;
		n = 4;
		snprintf(op_buf, op_buf_size, "%s+0x%x", reg_name, disp);
		break;
	default:
		return -1;
	}

	printf("b: %s\n", op_buf);
	return n;	
}

int decode_sib(char *op_string, char *op_buf, 
		int op_buf_size, struct x86_opcode *op_code)
{
	char ss_index[16] = {0}, base[16] = {0};
	int n = 0;

	if (select_sib_ss_index(ss_index, 16, op_code) == -1)
		return -1;

	n = select_sib_base(op_string, base, 16, op_code);
	if (n == -1)
		return -1;

	if (ss_index[0] != '\0')
		snprintf(op_buf, op_buf_size, "%s+%s", base, ss_index);
	else
		snprintf(op_buf, op_buf_size, "%s", base);
	printf("sib: %s\n", op_buf);
	return n;
}

int decode_effective_sib(uint8_t *op_string, char *op_buf, 
			int op_buf_size, struct x86_opcode *op_code)
{
	struct ins_prefix *ins = &op_code->op_ins;
	struct rex_prefix *rex = &op_code->op_rex;
	struct mod_reg *reg = &op_code->op_reg;
	struct sib *sib = &op_code->op_sib;
	uint8_t *s = op_string;
	char sib_buf[32];
	int m = 0, i = 0;

	parse_sib(*s, sib);
	print_sib(sib);

	m = decode_sib(s + 1, sib_buf, sizeof(sib_buf) - 1, op_code);
	if (m == -1)
		return -1;

	if (ins->op_seg_buf[0] != '\0')
		i = snprintf(op_buf, op_buf_size - 1, "%s", ins->op_seg_buf);

	if (m == 0) {
		switch (reg->mod) {
		case 0x1:
			op_code->disp_value = *(uint8_t *)(s + 1);
			m += 1;
			break;
		case 0x2:
			op_code->disp_value = *(uint32_t *)(s + 1);
			m += 4;
			break;
		}
		printf("sib disp: 0x%x\n", op_code->disp_value);
		snprintf(op_buf + i, op_buf_size - i - 1, 
			"[%s+0x%x]", sib_buf, op_code->disp_value);
	}
	else {
		snprintf(op_buf + i, op_buf_size - i - 1, "[%s]", sib_buf);
	}

	printf("%s\n", op_buf);
	return m + 1;
}

int decode_effective_rip(char *op_buf, int op_buf_size, 
		struct x86_opcode *op_code, int value)
{
	struct ins_prefix *ins = &op_code->op_ins;
	struct rex_prefix *rex = &op_code->op_rex;
	int i = 0;


	if (ins->op_seg_buf[0] != '\0')
		i = snprintf(op_buf, op_buf_size - 1, "%s", ins->op_seg_buf);

	if (rex->w == 1)
		snprintf(op_buf + i, op_buf_size - i - 1, "[rip+0x%x]", value);
	else
		snprintf(op_buf + i, op_buf_size - i - 1, "[eip+0x%x]", value);
	return 0;
}

int decode_effective_reg(uint8_t *op_string, char *op_buf, 
			int op_buf_size, char *op_desc, 
			int flag, struct x86_opcode *op_code)
{
	struct ins_prefix *ins = &op_code->op_ins;
	struct rex_prefix *rex = &op_code->op_rex;
	struct mod_reg *reg = &op_code->op_reg;
	uint8_t *s = op_string;
	char *tmp;
	int i = 0, n = 0;

	tmp = select_effective_reg(reg->rm, op_code->op_size, op_desc, rex);
	if (!tmp)
		return -1;

	if (ins->op_seg_buf[0] != '\0')
		i = snprintf(op_buf, op_buf_size - 1, "%s", ins->op_seg_buf);
			
	switch (reg->mod) {
	case 0x0:
		snprintf(op_buf + i, op_buf_size - i - 1, "[%s]", tmp);
		break;
	case 0x1:
		if (flag == 1)
			op_code->disp_value = *(uint8_t *)(s + 1);
		else
			op_code->disp_value = *(uint8_t *)s;
		
		snprintf(op_buf + i, op_buf_size - i - 1, "[%s+0x%x]", 
			tmp, op_code->disp_value);
		n += 1;
		break;
	case 0x2:
		if (flag == 1)
			op_code->disp_value = *(uint32_t *)(s + 1);
		else
			op_code->disp_value = *(uint32_t *)s;
			
		snprintf(op_buf + i, op_buf_size - i - 1, "[%s+0x%x]", 
			tmp, op_code->disp_value);
		n += 4;
		break;
	case 0x3:
		snprintf(op_buf + i, op_buf_size - i - 1, "%s", tmp);
		break;
	}

	printf("%s\n", op_buf);
	return n;
}

int decode_effective_imm(uint8_t *op_string, char *op_buf, 
			int op_buf_size, char *op_desc, 
			struct x86_opcode *op_code)
{
	struct rex_prefix *rex = &op_code->op_rex;
	unsigned int size;
	int n = 0;

	size = get_operand_size(op_code->op_size, op_desc, rex);
	if (size <= 0)
		return -1;
			
	switch (size) {
	case 1:
		op_code->imm_value = *(uint8_t *)op_string;
		n += 1;
		break;
	case 2:
		op_code->imm_value = *(uint16_t *)op_string;
		n += 2;
		break;
	case 3:
	case 4:
		/* in 64bit mode, must immediate data is 4byte. */
		op_code->imm_value = *(uint32_t *)op_string;
		n += 4;
		break;
	default:
		return -1;
	}

	snprintf(op_buf, op_buf_size - 1, "0x%x", op_code->imm_value);
	return n;
}

int decode_operand(uint8_t *op_string, char *op_buf, int buf_size, 
		char *op_desc, struct x86_opcode *op_code)
{
	struct ins_prefix *ins = &op_code->op_ins;
	struct rex_prefix *rex = &op_code->op_rex;
	struct mod_reg *reg = &op_code->op_reg;
	uint8_t *s = op_string;
	char *tmp;
	int m = 0, n = 0, i = 0;

	if (op_desc[0] >= 'a' && op_desc[0] <= 'z') {
		if (get_effective_reg(op_desc, op_buf, op_code) == -1)
			return -1;
		printf("%s\n", op_buf);
	}

	/* decode effective address. */
	if (op_desc[0] == 'E') {
		/* the modreg r/m failed has not been decoded. */
		if (reg->mod == -1) {
			n += 1;
			parse_mod_reg(*s, reg);
		}
		print_mod_reg(reg);
	
		/* sib mode. */
		if (reg->mod != 0x3 && reg->rm == 0x4) {
			m = decode_effective_sib(s + n, op_buf, buf_size, op_code);
			if (m == -1)
				return -1;

			n += m;
			return n;
		}
		/* 64bit rip reltaive mode. */
		if (reg->mod == 0x0 && reg->rm == 0x5) {
			if (n == 1)
				op_code->disp_value = *(int *)(s + 1);
			else
				op_code->disp_value = *(int *)s;
			n += 4;
	
			decode_effective_rip(op_buf, buf_size, op_code, op_code->disp_value);
			return n;
		}

		/* decode effective general register. */
		m = decode_effective_reg(s, op_buf, buf_size, op_desc, n, op_code);
		if (m == -1)
			return -1;
		printf("%s\n", op_buf);
		return n + m;
	}

	/* modrm refer to general register. */
	if (op_desc[0] == 'G') {
                if (reg->mod == -1) {
			n += 1;
                        parse_mod_reg(*s, reg);
		}
		print_mod_reg(reg);

		tmp = select_general_reg(reg->reg, op_code->op_size, op_desc, rex);
		if (!tmp)
			return -1;
		snprintf(op_buf, buf_size, "%s", tmp);

		printf("%s\n", op_buf);
		return n;
	}

	/* modrm only refer to memory. */
	if (op_desc[0] == 'M') {
		/* sib mode. */
		if (reg->mod != 0x3 && reg->rm == 0x4) {
			m = decode_effective_sib(s + n, op_buf, buf_size, op_code);
			if (m == -1)
				return -1;

			n += m;
			return n;
		}

                /* 64bit rip reltaive mode. */
                if (reg->mod == 0x0 && reg->rm == 0x5) {
                	op_code->imm_value = *(int *)s;
			decode_effective_rip(op_buf, buf_size, 
					     op_code, op_code->imm_value);
                	n += 4;
			return n;
                }
		/* decode effective general register. */
		m = decode_effective_reg(s, op_buf, buf_size, op_desc, n, op_code);
		if (m == -1)
			return -1;
		printf("%s\n", op_buf);
		return n + m;
	}

	/* handle immediate data. */
	if (op_desc[0] == 'I') {
		m = decode_effective_imm(s, op_buf, buf_size, op_desc, op_code);
		if (m == -1)
			return -1;

		printf("%s\n", op_buf);
		n += m;
	}

	if (op_desc[0] == 'Y') {
		snprintf(op_buf, buf_size, "%s", "es:%rdi");
	}
		
	if (op_desc[0] == 'X') {
		snprintf(op_buf, buf_size, "%s", "ds:%rsi");
	}

	return n;
}

int select_reg_by_arg(struct x86_opcode *op_code, 
		struct opcode_desc *op_desc)
{
	struct rex_prefix *rex = &op_code->op_rex;
	char arg[8] = {0}, arg1[8] = {0};
	char tmp[32], op_buf[32];
	char *s;
	int op_size, r_flag = 0;

	if (!op_desc->arg) {
		printf("#%s\n", op_desc->mnemonic);
		return 0;
	}

	if (op_desc->arg[0] != 'r' && op_desc->arg[0] != 'e') {
		printf("#%s %s\n", op_desc->mnemonic, op_desc->arg);
		return 0;
	}

	if (op_desc->arg[0] == 'e' && op_desc->arg[1] == 's') {
		printf("#%s %s\n", op_desc->mnemonic, op_desc->arg);
		return 0;
	}

	strcpy(tmp, op_desc->arg);
	s = strchr(tmp, '/');
	if (s) {
		strcpy(arg1, s + 1);
		*s = 0;
		if (rex->b == 1 || rex->r == 1)
			r_flag = 1;
	}

	op_size = select_operand_size(op_code->op_size, rex);
	if (op_size == -1)
		return -1;

	if (op_desc->opcode >= 0x50 && 
		op_desc->opcode <= 0x5f && 
		op_code->op_size == 0)
		op_size = 8;

	switch (op_size) {
	case 1:
		/* 64bit mode does not support 8bit operand size. */
		break;
	case 2:
		if (r_flag == 1) {
			snprintf(op_buf, sizeof(op_buf) - 1,
				"%s %%%sw", op_desc->mnemonic, arg1);
		}
		else {
			snprintf(op_buf, sizeof(op_buf) - 1,
				"%s %%%s", op_desc->mnemonic, &tmp[1]);
		}
		break;
	case 4:
		if (r_flag == 1) {
			snprintf(op_buf, sizeof(op_buf) - 1,
				"%s %%%sd", op_desc->mnemonic, arg1);
		}
		else {
			snprintf(op_buf, sizeof(op_buf) - 1,
				"%s %%e%s", op_desc->mnemonic, &tmp[1]);
		}
		break;
	case 8:
		if (r_flag == 1) {
			snprintf(op_buf, sizeof(op_buf) - 1,
				"%s %%%s", op_desc->mnemonic, arg1);
		}
		else {
			snprintf(op_buf, sizeof(op_buf) - 1,
				"%s %%r%s", op_desc->mnemonic, &tmp[1]);
		}
		break;
	}

	printf("#%s\n", op_buf);
	return 0;
}

int is_rex_encode(uint8_t **op_buf, struct rex_prefix *rex)
{
	uint8_t *s = *op_buf;

	if (*s >= 0x40 && *s <= 0x4f) {
       		decode_rex(*s, rex);
		print_rex(rex);
                *op_buf++;
		return 0;
        }

	return -1;
}

struct opcode_desc *lookup_code_map(uint8_t index, 
				    struct x86_opcode *op_code)
{
	struct opcode_desc *op_desc;
	struct ins_prefix *ins = &op_code->op_ins;

	if (ins->op_0x66 || ins->op_rep1 || ins->op_rep2) {
		if (!ins->op_0x0f) {
			/* 0x66 is operand size prefix. */
			if (ins->op_0x66)
				op_code->op_size = 1;
			/* 0xf2 is repeat prefix. */
			if (ins->op_rep1)
				strcpy(ins->op_rep_buf, "repne");
			/* 0xf3 is repeat prefix. */
			if (ins->op_rep2)
				strcpy(ins->op_rep_buf, "repe");
				
			return lookup_1byte_opcode_map(index);
		}	
	
		/* lookup three byte code map1. */
		if (ins->op_0x38)
			op_desc = lookup_3byte_opcode_map1(index);
		/* lookup three byte code map2. */
		else if (ins->op_0x3a)
			op_desc = lookup_3byte_opcode_map2(index);
		/* lookup two byte code map2. */
		else
			op_desc = lookup_2byte_opcode_map2(index);
	}
	else if (ins->op_0x0f) {
		/* lookup three byte code map1. */
		if (ins->op_0x38)
			op_desc = lookup_3byte_opcode_map1(index);
		/* lookup three byte code map2. */
		else if (ins->op_0x3a)
			op_desc = lookup_3byte_opcode_map2(index);
		/* lookup two byte code map1. */
		else
			op_desc = lookup_2byte_opcode_map1(index);
	}
	else {
		/* lookup one byte code map. */
		op_desc = lookup_1byte_opcode_map(index);
	}

	return op_desc;
}

void print_opcode(struct opcode_desc *op_desc)
{
        printf("opcode: 0x%02x name: %s arg: %s op1: %s op2: %s op3: %s\n",
                op_desc->opcode, op_desc->mnemonic, op_desc->arg,
                op_desc->op1, op_desc->op2, op_desc->op3);
}

void print_ins(struct x86_opcode *op_code, struct opcode_desc *op_desc,
		char *op1_buf, char *op2_buf, char *op3_buf)
{
	struct ins_prefix *ins = &op_code->op_ins;
	char op_desc_buf[64];
	char mnemonic[16], s, *p;
	int n = 0;

	strcpy(mnemonic, op_desc->mnemonic);

	p = strchr(mnemonic, '*');
	if (p) {
		s = select_mnemonic_size(op_code->op_size, &op_code->op_rex);
		if (s == -1)
			return ;
		*p = s;
	}

	if (ins->op_lock_buf[0] != '\0')
		n = snprintf(op_desc_buf, sizeof(op_desc_buf) - 1,
				"%s ", ins->op_lock_buf);

	if (ins->op_rep_buf[0] != '\0')
		n += snprintf(op_desc_buf + n, sizeof(op_desc_buf) - n - 1,
				"%s ", ins->op_rep_buf);

	n += snprintf(op_desc_buf + n, sizeof(op_desc_buf) - n - 1,
			"%s ", mnemonic);

	if (op1_buf[0] != '\0')
		n += snprintf(op_desc_buf + n, sizeof(op_desc_buf) - n - 1,
				"%s", op1_buf);

	if (op2_buf[0] != '\0')
		n += snprintf(op_desc_buf + n, sizeof(op_desc_buf) - n - 1,
				", %s", op2_buf);

	if (op3_buf[0] != '\0')
		n += snprintf(op_desc_buf + n, sizeof(op_desc_buf) - n - 1,
				", %s", op3_buf);

	printf("#%s\n", op_desc_buf);
}

int decode_prefix(uint8_t *op, struct x86_opcode *op_code)
{
	struct ins_prefix *ins = &op_code->op_ins;
	struct rex_prefix *rex = &op_code->op_rex;
	uint8_t *s = op;

	switch (*s) {
	case 0x40 ... 0x4f:
		decode_rex(*s, rex);
		print_rex(rex);
		break;
	case 0x66: 
		ins->op_0x66 = 1;
		break;
	case 0x67:
		ins->op_0x67 = 1;
		break;
	case 0xf2:
		ins->op_rep1 = 1;
		break;
	case 0xf3:
		ins->op_rep2 = 1;
		break;
	case 0x38:
		ins->op_0x38 = 1;
		break;
	case 0x3a:
		ins->op_0x3a = 1;
		break;
	case 0x2e:
		ins->op_seg_cs = 1;
		strcpy(ins->op_seg_buf, "cs:");
		break;
	case 0x36:
		ins->op_seg_ss = 1;
		strcpy(ins->op_seg_buf, "ss:");
		break;
	case 0x3e:
		ins->op_seg_ds = 1;
		strcpy(ins->op_seg_buf, "ds:");
		break;
	case 0x26:
		ins->op_seg_es = 1;
		strcpy(ins->op_seg_buf, "es:");
		break;
	case 0x64:
		ins->op_seg_fs = 1;
		strcpy(ins->op_seg_buf, "fs:");
		break;
	case 0x65:
		ins->op_seg_gs = 1;
		strcpy(ins->op_seg_buf, "gs:");
		break;
	case 0x0f:
		ins->op_0x0f = 1;
		break;
	case 0xf0:
		ins->op_0xf0 = 1;
		strcpy(ins->op_lock_buf, "lock ");
		break;
	default:
		return -1;
	}

	return 0;
}

int decode_operand_arg(uint8_t *op_string, struct opcode_desc *op_desc, 
	       struct x86_opcode *op_code)
{
	char op1_buf[32], op2_buf[32], op3_buf[32];
	uint8_t *s = op_string;
	int n = 0, n1 = 0, n2 = 0;

	n = decode_operand(s, op1_buf, 31, op_desc->op1, op_code);
	if (n == -1)
		return -1;
	s += n;

	if (op_desc->op2) {
		n1 = decode_operand(s, op2_buf, 31, op_desc->op2, op_code);
		if (n1 == -1)
			return -1;
		s += n1;
	}
	if (op_desc->op3) {
		n2 = decode_operand(s, op3_buf, 31, op_desc->op3, op_code);
		if (n2 == -1)
			return -1;
		s += n2;
		print_ins(op_code, op_desc, op1_buf, op2_buf, op3_buf);
	}
	else {
		print_ins(op_code, op_desc, op1_buf, op2_buf, op3_buf);
	}

	return n + n1 + n2;
}

int decode_instruction(uint8_t *op_string, struct x86_opcode *op_code)
{
	struct opcode_desc *op_desc;
	uint8_t *s = op_string, *p;
	int n = 0;

	op_code->op_reg.mod = -1;
	while (*s) {
		if (!decode_prefix(s, op_code)) {
			s++;
			continue;
		}
		op_desc = lookup_code_map(*s, op_code);
		if (!op_desc)
			return -1;
		s++;
		break;
	}
	print_opcode(op_desc);

	/* decode opcode with no arguments. */
	if (!op_desc->op1 && !op_desc->op2 && !op_desc->op3) {
		if (select_reg_by_arg(op_code, op_desc) == -1)
			return -1;
		goto out;
	}

	n = decode_operand_arg(s, op_desc, op_code);
	if (n == -1)
		return -1;

	s += n;
out:
	return s - op_string;
}

int disass_opcode(uint8_t *op_string, int op_len)
{
	struct x86_opcode op_code;
	uint8_t *s = op_string;
	int i = 0, n = 0;

	while (i < op_len) {
		bzero(&op_code, sizeof(op_code));
		n = decode_instruction(s + i, &op_code);
		if (n == -1)
			return -1;
		i += n;
	}

	return 0;
}
