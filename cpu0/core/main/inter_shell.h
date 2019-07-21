#ifndef __INTER_SHELL_H__
#define __INTER_SHELL_H__

#ifndef EYESH_MAXARGS
# define EYESH_MAXARGS 12
#endif

#ifndef EYESH_CMDLEN
# define EYESH_CMDLEN  4096
#endif

struct enxsh_ctx;

struct enxsh_cmd {
	const char *label;
	int (*cmd_f)(struct enxsh_ctx *shell, int args, const char *argv[]);
	const char *help;
	void *ctx;
	struct enxsh_cmd *next;
};

struct enxsh_ctx {
	void *data;                             /* context, passed to cmd_f */
	struct enxsh_cmd *cmdlist;              /* list of commands */
	void *ctx;                              /* passed to outb */
	
	char *buf;
	int buflen;
	
	int argc;
	const char *argv[EYESH_MAXARGS];	
	struct enxsh_cmd *defcmd;
};

struct enxsh_ctx *
enxsh_create(void *ctx);

void
enxsh_addcmd(struct enxsh_ctx *priv, struct enxsh_cmd *cmd, void *ctx);

void
enxsh_schedule_cmd(struct enxsh_ctx *priv);

#endif
