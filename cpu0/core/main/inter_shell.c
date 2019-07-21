#include "dev.h"

#include <string.h>
#include <stdlib.h>

#include "inter_shell.h"

#ifdef __WIFI__
struct enxsh_ctx *
enxsh_create(void *ctx)
{
	struct enxsh_ctx *priv;	
	
	priv = malloc(sizeof(struct enxsh_ctx));
	
	if (priv == NULL)
		return NULL;
	
	memset(priv, 0, sizeof(struct enxsh_ctx));
	
	priv->buflen = EYESH_CMDLEN;
	priv->buf = malloc(priv->buflen);
	
	if (priv->buf == NULL) {
		free(priv);
		return NULL;
	}

	memset(priv->buf, 0, priv->buflen);
	priv->ctx = ctx;
	return priv;
}

void
enxsh_addcmd(struct enxsh_ctx *priv, struct enxsh_cmd *cmd, void *ctx)
{
	cmd->ctx = ctx;
	if (priv->cmdlist == NULL) {
		priv->cmdlist = cmd;
	
	} 
	else 
	{
		struct enxsh_cmd *iter = priv->cmdlist;
		while (iter->next)
			iter = iter->next;		
		iter->next = cmd;
	}
	
	cmd->next = NULL;
}

void
enxsh_defcmd(struct enxsh_ctx *priv, struct enxsh_cmd *cmd, void *ctx)
{
	cmd->ctx = ctx;
	priv->defcmd = cmd;
}

void *
enxsh_data(struct enxsh_ctx *priv)
{
	return priv->data;
}

#if 0
static int
cmd_help_f(struct enxsh_ctx *priv, int argc, const char *argv[])
{
	struct enxsh_cmd *iter;
	
	(void) argc; /* ununsed */
	(void) argv; /* ununsed */
	
	//for (iter = priv->cmdlist; iter; iter = iter->next)
	//	enxsh_printf(priv, "%-20s %s\n\r", iter->label,iter->help ? iter->help : "");

	return 0;
}
#endif

static void
parse_line(char *line, int *argc, const char **argv)
{
	char *tok;
	*argc = 0;
	argv[*argc] = "";
	for (tok = strtok(line, " "); tok; tok = strtok(NULL, " ")) {
		argv[(*argc)++] = tok;
		if (*argc == EYESH_MAXARGS)
	    	return;
	}
}

static struct enxsh_cmd *
enxsh_find_cmd(struct enxsh_ctx *priv, const char *label)
{
	struct enxsh_cmd *cmd = NULL;
	struct enxsh_cmd *iter;
	int ambiguous = 0;
	
	if (strlen(label) == 0)
	    return NULL;
	
	for (iter = priv->cmdlist; iter; iter = iter->next) {
	
	    /* exact match */
	    if (!strcmp(iter->label, label))
			return iter;
	
	    /* partial match */
	    if (strncmp(iter->label, label, strlen(label)))
			continue;
	
	    /* ambiguous */
	    if (cmd)
			ambiguous = 1;
	
	    cmd = iter;
	}
	
	return ambiguous ? NULL : cmd;
}

void
enxsh_schedule_cmd(struct enxsh_ctx *priv)
{
    struct enxsh_cmd *cmd;
	if(priv->buf == NULL)     return;
	if(!(strlen(priv->buf)))  return;
	parse_line(priv->buf, &priv->argc, priv->argv);
	cmd = enxsh_find_cmd(priv, priv->argv[0]);
	if(cmd != NULL)
	{
		priv->data = cmd->ctx;
		cmd->cmd_f((struct enxsh_ctx *)priv, priv->argc, priv->argv);
	}
	else if(priv->defcmd)
	{
		priv->data = priv->defcmd->ctx;
		priv->defcmd->cmd_f((struct enxsh_ctx *)priv, priv->argc, priv->argv);
	}
    return;
}
#endif
