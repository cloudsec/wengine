#ifndef OPCODE_H
#define OPCODE_H

struct ins_prefix {
        int op_lock;
        int op_rep1;
	int op_rep2;
        int op_seg_cs;
	int op_seg_ss;
        int op_seg_ds;
	int op_seg_es;
        int op_seg_fs;
	int op_seg_gs;
        int op_0x66;
	int op_0x67;
	int op_0x0f;
	int op_0xf0;
	int op_0x38;
	int op_0x3a;
	char op_lock_buf[8];
	char op_rep_buf[8];
	char op_seg_buf[8];
};

struct opcode_desc {
        uint8_t opcode;
        char *mnemonic;
        char *arg;
        char *op1;
        char *op2;
        char *op3;
};

struct rex_prefix {
        int w;
        int r;
        int x;
        int b;
};

struct mod_reg {
        int mod;
        int reg;
        int rm;
};

struct sib {
        int scale;
        int index;
        int base;
};

struct x86_opcode {
	int op_size;
	int addr_size;
        struct ins_prefix op_ins;
        struct rex_prefix op_rex;
        struct mod_reg op_reg;
	struct sib op_sib;
	int disp_value;
	int imm_value;
};

char *mod_reg_general[8*5] = {
	/* with rex prefix. */
        "al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil",
	/* without rex prefix. */
        "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
        "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
        "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
        "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"
        };

char *mod_reg_ex[8*4] = {
	"r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
	"r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",
	"r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d",
	"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
	};
	

char *mod_seg[16] =
        {"es", "cs", "ss", "ds", "fs", "gs", "res.", "res.",
        /* rex.r = 1 */
        "es", "cs", "ss", "ds", "fs", "gs", "res.", "res."};

char *mod_st[16] =
        {"st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7",
        /* rex.b = 1 */
        "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7"};

char *mod_mm[16] =
        {"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
        /* rex.r = 1 */
        "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7"};


char *mod_xmm[16] =
        {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
        /* rex.r = 1 */
        "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"};

char *mod_cr[16] =
        {"cr0", "invd", "cr2", "cr3", "cr4", "invd", "invd", "invd",
        /* rex.r = 1 */
        "cr8", "invd", "invd", "invd", "invd", "invd", "invd", "invd"};


char *mod_dr[16] =
        {"dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7",
        /* rex.r = 1 */
        "invd", "invd", "invd", "invd", "invd", "invd", "invd", "invd"};

struct opcode_desc one_byte_opcode_map[256] = {
        {0x00,  "add",  NULL,   "Eb",   "Gb", NULL},
        {0x01,  "add",  NULL,   "Ev",   "Gv", NULL},
        {0x02,  "add",  NULL,   "Gb",   "Eb", NULL},
        {0x03,  "add",  NULL,   "Gv",   "Ev", NULL},
        {0x04,  "add",  NULL,   "al",   "Ib", NULL},
        {0x05,  "add",  NULL,   "rax",  "Iz", NULL},
        /* es i64 */
        {0x06,  "push", "es",   NULL,   NULL, NULL},
        /* es i64 */
        {0x07,  "pop",  "es",   NULL,   NULL, NULL},
        {0x08,  "or",   NULL,   "Eb",   "Gb", NULL},
        {0x09,  "or",   NULL,   "Ev",   "Gv", NULL},
        {0x0a,  "or",   NULL,   "Gb",   "Eb", NULL},
        {0x0b,  "or",   NULL,   "Gv",   "Ev", NULL},
        {0x0c,  "or",   NULL,   "al",   "Ib", NULL},
        {0x0d,  "or",   NULL,   "rax",  "Iz", NULL},
        /* cs i64 */
        {0x0e,  "push", "cs",   NULL,   NULL, NULL},
        /* two bytes escape */
        {0x0f,  NULL,   NULL,   NULL,   NULL, NULL},

        {0x10,  "adc",  NULL,   "Eb",   "Gb", NULL},
        {0x11,  "adc",  NULL,   "Ev",   "Gv", NULL},
        {0x12,  "adc",  NULL,   "Gb",   "Eb", NULL},
        {0x13,  "adc",  NULL,   "Gv",   "Ev", NULL},
        {0x14,  "adc",  NULL,   "al",   "Ib", NULL},
        {0x15,  "adc",  NULL,   "rax",  "Iz", NULL},
        /* ss i64 */
        {0x16,  "push", "ss",   NULL,   NULL, NULL},
        /* ss i64 */
        {0x17,  "pop",  "ss",   NULL,   NULL, NULL},
        {0x18,  "sbb",  NULL,   "Eb",   "Gb", NULL},
        {0x19,  "sbb",  NULL,   "Ev",   "Gv", NULL},
        {0x1a,  "sbb",  NULL,   "Gb",   "Eb", NULL},
        {0x1b,  "sbb",  NULL,   "Gv",   "Ev", NULL},
        {0x1c,  "sbb",  NULL,   "al",   "Ib", NULL},
        {0x1d,  "sbb",  NULL,   "rax",  "Iz", NULL},
        /* ds i64 */
        {0x1e,  "push", "ds",   NULL,   NULL, NULL},
        /* ds i64 */
        {0x1f,  "pop",  "ds",   NULL,   NULL, NULL},

        {0x20,  "and",  NULL,   "Eb",   "Gb", NULL},
        {0x21,  "and",  NULL,   "Ev",   "Gv", NULL},
        {0x22,  "and",  NULL,   "Gb",   "Eb", NULL},
        {0x23,  "and",  NULL,   "Gv",   "Ev", NULL},
        {0x24,  "and",  NULL,   "al",   "Ib", NULL},
        {0x25,  "and",  NULL,   "rax",  "Iz", NULL},
        {0x26,  "seg",  "es",   NULL,   NULL, NULL},
        /* i64 */
        {0x27,  "daa",  NULL,   NULL,   NULL, NULL},
        {0x28,  "sub",  NULL,   "Eb",   "Gb", NULL},
        {0x29,  "sub",  NULL,   "Ev",   "Gv", NULL},
        {0x2a,  "sub",  NULL,   "Gb",   "Eb", NULL},
        {0x2b,  "sub",  NULL,   "Gv",   "Ev", NULL},
        {0x2c,  "sub",  NULL,   "al",   "Ib", NULL},
        {0x2d,  "sub",  NULL,   "rax",  "Iz", NULL},
        {0x2e,  "seg", 	"cs",   NULL,   NULL, NULL},
        /* das i64 */
        {0x2f,  "das",  NULL,   NULL,   NULL, NULL},

        {0x30,  "xor",  NULL,   "Eb",   "Gb", NULL},
        {0x31,  "xor",  NULL,   "Ev",   "Gv", NULL},
        {0x32,  "xor",  NULL,   "Gb",   "Eb", NULL},
        {0x33,  "xor",  NULL,   "Gv",   "Ev", NULL},
        {0x34,  "xor",  NULL,   "al",   "Ib", NULL},
        {0x35,  "xor",  NULL,   "rax",  "Iz", NULL},
        {0x36,  "seg",  "ss",   NULL,   NULL, NULL},
        /* i64 */
        {0x37,  "aaa",  NULL,   NULL,   NULL, NULL},
        {0x38,  "cmp",  NULL,   "Eb",   "Gb", NULL},
        {0x39,  "cmp",  NULL,   "Ev",   "Gv", NULL},
        {0x3a,  "cmp",  NULL,   "Gb",   "Eb", NULL},
        {0x3b,  "cmp",  NULL,   "Gv",   "Ev", NULL},
        {0x3c,  "cmp",  NULL,   "al",   "Ib", NULL},
        {0x3d,  "cmp",  NULL,   "rax",  "Iz", NULL},
        {0x3e,  "seg", 	"ds",   NULL,   NULL, NULL},
        /* das i64 */
        {0x3f,  "aas",  NULL,   NULL,   NULL, NULL},

        {0x40,  "rex",          NULL,   NULL,   NULL, NULL},
        {0x41,  "rex.b",        NULL,   NULL,   NULL, NULL},
        {0x42,  "rex.x",        NULL,   NULL,   NULL, NULL},
        {0x43,  "rex.xb",       NULL,   NULL,   NULL, NULL},
        {0x44,  "rex.r",        NULL,   NULL,   NULL, NULL},
        {0x45,  "rex.rb",       NULL,   NULL,   NULL, NULL},
        {0x46,  "rex.rx",       NULL,   NULL,   NULL, NULL},
        {0x47,  "rex.rxb",      NULL,   NULL,   NULL, NULL},
        {0x48,  "rex.w",        NULL,   NULL,   NULL, NULL},
        {0x49,  "rex.wb",       NULL,   NULL,   NULL, NULL},
        {0x4a,  "rex.wx",       NULL,   NULL,   NULL, NULL},
        {0x4b,  "rex.wxb",      NULL,   NULL,   NULL, NULL},
        {0x4c,  "rex.wr",       NULL,   NULL,   NULL, NULL},
        {0x4d,  "rex.wrb",      NULL,   NULL,   NULL, NULL},
        {0x4e,  "rex.wrx",      NULL,   NULL,   NULL, NULL},
        {0x4f,  "rex.wrxb",     NULL,   NULL,   NULL, NULL},

	/* d64 */
        {0x50,  "push", "rax/r8", 	NULL, NULL, NULL},
        {0x51,  "push", "rcx/r9", 	NULL, NULL, NULL},
        {0x52,  "push", "rdx/r10",	NULL, NULL, NULL},
        {0x53,  "push", "rbx/r11", 	NULL, NULL, NULL},
        {0x54,  "push", "rsp/r12", 	NULL, NULL, NULL},
        {0x55,  "push", "rbp/r13", 	NULL, NULL, NULL},
        {0x56,  "push", "rsi/r14", 	NULL, NULL, NULL},
        {0x57,  "push", "rdi/r15",	NULL, NULL, NULL},
        {0x58,  "pop", 	"rax/r8", 	NULL, NULL, NULL},
        {0x59,  "pop", 	"rcx/r9", 	NULL, NULL, NULL},
        {0x5a,  "pop", 	"rdx/r10", 	NULL, NULL, NULL},
        {0x5b,  "pop", 	"rbx/r11", 	NULL, NULL, NULL},
        {0x5c,  "pop", 	"rsp/r12", 	NULL, NULL, NULL},
        {0x5d,  "pop", 	"rbp/r13", 	NULL, NULL, NULL},
        {0x5e,  "pop", 	"rsi/r14", 	NULL, NULL, NULL},
        {0x5f,  "pop", 	"rdi/r15", 	NULL, NULL, NULL},

        {0x60,  NULL,           NULL,   NULL,   NULL, NULL},
        {0x61,  NULL,           NULL,   NULL,   NULL, NULL},
        {0x62,  NULL,           NULL,   NULL,   NULL, NULL},
        {0x63,  "movsxd",       NULL,   "Gv",   "Ev", NULL},
        {0x64,  "seg",          "fs",   NULL,   NULL, NULL},
        {0x65,  "seg",          "gs",   NULL,   NULL, NULL},
        {0x66,  NULL,           NULL,   NULL,   NULL, NULL},
        {0x67,  NULL,           NULL,   NULL,   NULL, NULL},
        {0x68,  "push",         NULL,   "Iz",   NULL,   NULL},
        {0x69,  "imul",         NULL,   "Gv",   "Ev",   "Iz"},
        {0x6a,  "push",         NULL,   "Ib",   NULL,   NULL},
        {0x6b,  "imul",         NULL,   "Gv",   "Ev",   "Ib"},
        {0x6c,  "ins/insb",     NULL,   "Yb",   "dx",   NULL},
        {0x6d,  "ins/insw/insd",NULL,   "Yz",   "dx",   NULL},
        {0x6e,  "outs/outsb",   NULL,   "dx",   "Xb",   NULL},
        {0x6f,  "outs/outsw/outsd",NULL,"dx",   "Xz",   NULL},

        {0x70, "jo",            NULL, NULL, NULL, NULL},
        {0x71, "jno",           NULL, NULL, NULL, NULL},
        {0x72, "jb/jnae/jc",    NULL, NULL, NULL, NULL},
        {0x73, "jnb/jae/jnc",   NULL, NULL, NULL, NULL},
        {0x74, "jz/je",         NULL, NULL, NULL, NULL},
        {0x75, "jnz/jne",       NULL, NULL, NULL, NULL},
        {0x76, "jbe/jna",       NULL, NULL, NULL, NULL},
        {0x77, "jnbe/ja",       NULL, NULL, NULL, NULL},
        {0x78, "js",            NULL, NULL, NULL, NULL},
        {0x79, "jns",           NULL, NULL, NULL, NULL},
        {0x7a, "jp/jpe",        NULL, NULL, NULL, NULL},
        {0x7b, "jnp/jpo",       NULL, NULL, NULL, NULL},
        {0x7c, "jl/jnge",       NULL, NULL, NULL, NULL},
        {0x7d, "jnl/jge",       NULL, NULL, NULL, NULL},
        {0x7e, "jle/jng",       NULL, NULL, NULL, NULL},
        {0x7f, "jnle/jg",       NULL, NULL, NULL, NULL},

        {0x80, "grp1-1a",       NULL, "Eb", "Ib", NULL},
        {0x81, "grp1-1a",       NULL, "Ev", "Iz", NULL},
        {0x82, "grp1-1a",       NULL, "Eb", "Ib", NULL},
        {0x83, "grp1-1a",       NULL, "Eb", "Ib", NULL},
        {0x84, "test",          NULL, "Eb", "Gb", NULL},
        {0x85, "test",          NULL, "Ev", "Gv", NULL},
        {0x86, "xchg",          NULL, "Eb", "Gb", NULL},
        {0x87, "xchg",          NULL, "Ev", "Gv", NULL},
        {0x88, "mov",           NULL, "Eb", "Gb", NULL},
        {0x89, "mov",           NULL, "Ev", "Gv", NULL},
        {0x8a, "mov",           NULL, "Gb", "Eb", NULL},
        {0x8b, "mov",           NULL, "Gv", "Ev", NULL},
        {0x8c, "mov",           NULL, "Ev", "Sw", NULL},
        {0x8d, "lea",           NULL, "Gv", "M", NULL},
        {0x8e, "mov",           NULL, "Sw", "Ew", NULL},
        {0x8f, "pop",           NULL, "Ev", NULL, NULL},

        {0x90, "nop",           NULL, 	NULL,		NULL,  NULL},
        {0x91, "xchg",          NULL,	"rcx/r9", 	"rax", NULL},
        {0x92, "xchg",          NULL,	"rdx/r10", 	"rax", NULL},
        {0x93, "xchg",          NULL,	"rbx/r11", 	"rax", NULL},
        {0x94, "xchg",          NULL,	"rsp/r12", 	"rax", NULL},
        {0x95, "xchg",          NULL,	"rbp/r13", 	"rax", NULL},
        {0x96, "xchg",          NULL,	"rsi/r14", 	"rax", NULL},
        {0x97, "xchg",          NULL,	"rdi/r15", 	"rax", NULL},
        {0x98, "cbw/cwde/cdqe", NULL, 	NULL, 		NULL, NULL},
        {0x99, "cwd/cdq/cqo",   NULL, 	NULL, 		NULL, NULL},
        {0x9a, "far call",      NULL, 	"Ap", 		NULL, NULL},
        {0x9b, "fwait/wait",    NULL, 	NULL, 		NULL, NULL},
        {0x9c, "pushf/d/q",     NULL, 	"Fv", 		NULL, NULL},
        {0x9d, "popf/d/q",      NULL, 	"Fv", 		NULL, NULL},
        {0x9e, "sahf",          NULL, 	NULL, 		NULL, NULL},
        {0x9f, "lahf",          NULL, 	NULL, 		NULL, NULL},

        {0xa0, "mov",   	NULL,   "al",	"Ob", 	NULL},
        {0xa1, "mov",   	NULL,   "rax", 	"Ov", 	NULL},
        {0xa2, "mov",   	NULL,   "Ob", 	"al", 	NULL},
        {0xa3, "mov",   	NULL,   "Ov", 	"rax", 	NULL},
        {0xa4, "movsb",    	NULL,   "Yb", 	"Xb", 	NULL},
        {0xa5, "movs*",		NULL,   "Yv", 	"Xv", 	NULL},
        {0xa6, "cmpsb",    	NULL,   "Xb", 	"Yb", 	NULL},
        {0xa7, "cmps*",     	NULL,   "Xv", 	"Yv", 	NULL},
        {0xa8, "test",  	NULL,   "al", 	"Ib", 	NULL},
        {0xa9, "test",  	NULL,   "rax", 	"Iz", 	NULL},
        {0xaa, "stosb",    	NULL,   "Yb", 	"al", 	NULL},
        {0xab, "stos*",		NULL,   "Yv", 	"rax", 	NULL},
        {0xac, "lodsb",    	NULL,   "al", 	"Xb", 	NULL},
        {0xad, "lods*",		NULL,   "rax", 	"Xv", 	NULL},
        {0xae, "scasb",    	NULL,   "al", 	"Yb",	NULL},
        {0xaf, "scas*",		NULL,   "rax", 	"Yv",	NULL},

        {0xb0, "mov",  	"NULL",	"al/r8b",       "Ib", NULL},
        {0xb1, "mov",  	"NULL",	"cl/r9b",       "Ib", NULL},
        {0xb2, "mov",  	"NULL",	"dl/r10b",      "Ib", NULL},
        {0xb3, "mov",   "NULL",	"bl/r11b",      "Ib", NULL},
        {0xb4, "mov",   "NULL",	"ah/r12b",      "Ib", NULL},
        {0xb5, "mov",   "NULL",	"ch/r13b",      "Ib", NULL},
        {0xb6, "mov",   "NULL",	"dh/r14b",      "Ib", NULL},
        {0xb7, "mov",   "NULL",	"bh/r15b",      "Ib", NULL},
        {0xb8, "mov",   "NULL",	"rax/r8",       "Iv", NULL},
        {0xb9, "mov",   "NULL",	"rcx/r9",       "Iv", NULL},
        {0xba, "mov",   "NULL",	"rdx/r10",      "Iv", NULL},
        {0xbb, "mov",   "NULL",	"rbx/r11",      "Iv", NULL},
        {0xbc, "mov",   "NULL",	"rsp/r12",      "Iv", NULL},
        {0xbd, "mov",   "NULL",	"rbp/r13",      "Iv", NULL},
        {0xbe, "mov",   "NULL",	"rsi/r14",      "Iv", NULL},
        {0xbf, "mov",   "NULL",	"rdi/r15",      "Iv", NULL},

        {0xc0, "grp2-1a", NULL, "Eb", "Ib", NULL},
        {0xc1, "grp2-1a", NULL, "Ev", "Ib", NULL},
        {0xc2, "ret", NULL, "Iw", NULL, NULL},
        {0xc3, "ret", NULL, NULL, NULL, NULL},
        {0xc4, NULL, NULL, NULL, NULL, NULL},
        {0xc5, NULL, NULL, NULL, NULL, NULL},
        {0xc6, "grp11-1a", NULL, "Eb", "Ib", NULL},
        {0xc7, "grp11-1a", NULL, "Eb", "Iz", NULL},
        {0xc8, "enter", NULL, "Iw", "Ib", NULL},
        {0xc9, "leave", NULL, NULL, NULL, NULL},
        {0xca, "far ret", NULL, "Iw", NULL, NULL},
        {0xcb, "far ret", NULL, NULL, NULL, NULL},
        {0xcc, "int3", NULL, NULL, NULL, NULL},
        {0xcd, "int", NULL, "Ib", NULL, NULL},
        {0xce, NULL, NULL, NULL, NULL, NULL},
        {0xcf, "iret*", NULL, NULL, NULL, NULL},

        {0xd0, "grp2-1a", NULL, "Eb", "1", NULL},
        {0xd1, "grp2-1a", NULL, "Ev", "1", NULL},
        {0xd2, "grp2-1a", NULL, "Eb", "CL", NULL},
        {0xd3, "grp2-1a", NULL, "Ev", "CL", NULL},
        {0xd4, NULL, NULL, NULL, NULL, NULL},
        {0xd5, NULL, NULL, NULL, NULL, NULL},
        {0xd6, NULL, NULL, NULL, NULL, NULL},
        {0xd7, "xlat/xlatb", NULL, NULL, NULL, NULL},
        {0xd8, NULL, NULL, NULL, NULL, NULL},
        {0xd9, NULL, NULL, NULL, NULL, NULL},
        {0xda, NULL, NULL, NULL, NULL, NULL},
        {0xdb, NULL, NULL, NULL, NULL, NULL},
        {0xdc, NULL, NULL, NULL, NULL, NULL},
        {0xdd, NULL, NULL, NULL, NULL, NULL},
        {0xde, NULL, NULL, NULL, NULL, NULL},
        {0xdf, NULL, NULL, NULL, NULL, NULL},

        {0xe0, "loopne/loopnz", NULL, "Jb", NULL, NULL},
        {0xe1, "loope/loopz", 	NULL, "Jb", NULL, NULL},
        {0xe2, "loop",		NULL, "Jb", NULL, NULL},
        {0xe3, "jrcxz", 	NULL, "Jb", NULL, NULL},
        {0xe4, "in", 		NULL, "al", "Ib", NULL},
        {0xe5, "in", 		NULL, "eax","Ib", NULL},
        {0xe6, "out", 		NULL, "Ib", "al", NULL},
        {0xe7, "out", 		NULL, "Ib", "eax", NULL},
        {0xe8, "call", 		NULL, "Jz", NULL, NULL},
        {0xe9, "jmp", 		NULL, "Jz", NULL, NULL},
        {0xea, "far jmp", 	NULL, "Ap", NULL, NULL},
        {0xeb, "short jmp", 	NULL, "Jb", NULL, NULL},
        {0xec, "in", 		NULL, "al", "dx", NULL},
        {0xed, "in", 		NULL, "eax","dx", NULL},
        {0xee, "out", 		NULL, "dx", "al", NULL},
        {0xef, "out", 		NULL, "dx", "eax", NULL},

        {0xf0, "LOCK", NULL, NULL, NULL, NULL},
        {0xf1, "int1", NULL, NULL, NULL, NULL},
        {0xf2, "repne xacquire", NULL, NULL, NULL, NULL},
        {0xf3, "rep xrelease/repe xrelease", NULL, NULL, NULL, NULL},
        {0xf4, "hlt", NULL, NULL, NULL, NULL},
        {0xf5, "cmc", NULL, NULL, NULL, NULL},
        {0xf6, "grp3-1a", NULL, "Eb", NULL, NULL},
        {0xf7, "grp3-1a", NULL, "Ev", NULL, NULL},
        {0xf8, "clc", NULL, NULL, NULL, NULL},
        {0xf9, "stc", NULL, NULL, NULL, NULL},
        {0xfa, "cli", NULL, NULL, NULL, NULL},
        {0xfb, "sti", NULL, NULL, NULL, NULL},
        {0xfc, "cld", NULL, NULL, NULL, NULL},
        {0xfd, "std", NULL, NULL, NULL, NULL},
        {0xfe, "grp4-1a", NULL, NULL, NULL, NULL},
        {0xff, "grp5-1a", NULL, NULL, NULL, NULL}
};

#endif
