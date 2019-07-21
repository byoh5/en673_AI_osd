#include "ngx_rtmp_core.h"
#include "ngx_rtmp_amf.h"
#include "ngx_rtmp_connect.h"
#include "ngx_rtmp_stream.h"
#include "ngx_rtmp_handshake.h"

#ifdef RTMP_SUPPORT

ngx_uint_t          ngx_max_module;
ngx_uint_t  ngx_rtmp_max_module;

static ngx_rtmp_core_main_conf_t      *ngx_rtmp_core_main_conf;

static ngx_command_t  ngx_rtmp_commands[] = {

    { ngx_string("rtmp"),
      NGX_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_rtmp_block,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_command_t  ngx_rtmp_core_commands[] = {

    { ngx_string("server"),
      NGX_RTMP_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_rtmp_core_server,
      0,
      0,
      NULL },
	{ ngx_string("application"),
	  NGX_RTMP_SRV_CONF|NGX_CONF_BLOCK|NGX_CONF_TAKE1,
	  ngx_rtmp_core_application,
	  NGX_RTMP_SRV_CONF_OFFSET,
	  0,
	  NULL },

      ngx_null_command
};

static ngx_rtmp_module_t  ngx_rtmp_core_module_ctx = {
    NULL,                                   /* preconfiguration */
    NULL,                                   /* postconfiguration */
    ngx_rtmp_core_create_main_conf,         /* create main configuration */
    NULL,                                   /* init main configuration */
    ngx_rtmp_core_create_srv_conf,          /* create server configuration */
    ngx_rtmp_core_merge_srv_conf,           /* merge server configuration */
    ngx_rtmp_core_create_app_conf,          /* create app configuration */
    ngx_rtmp_core_merge_app_conf            /* merge app configuration */
};

static ngx_core_module_t  ngx_rtmp_module_ctx = {
    ngx_string("rtmp"),
    NULL,
    NULL
};

ngx_module_t  ngx_rtmp_module = {
    NGX_MODULE_V1,
    &ngx_rtmp_module_ctx,                  /* module context */
    ngx_rtmp_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,//ngx_rtmp_init_process,                 /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_command_t  ngx_rtmp_live_commands[] = {

    { ngx_string("live"),
      NGX_RTMP_MAIN_CONF|NGX_RTMP_SRV_CONF|NGX_RTMP_APP_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_flag_slot,
      NGX_RTMP_APP_CONF_OFFSET,
      offsetof(ngx_rtmp_live_app_conf_t, live),
      NULL },
      ngx_null_command
};



ngx_module_t  ngx_rtmp_core_module = {
    NGX_MODULE_V1,
    &ngx_rtmp_core_module_ctx,             /* module context */
    ngx_rtmp_core_commands,                /* module directives */
    NGX_RTMP_MODULE                       /* module type */

};

static ngx_rtmp_module_t  ngx_rtmp_live_module_ctx = {
    NULL,                                   /* preconfiguration */
    ngx_rtmp_live_postconfiguration,        /* postconfiguration */
    NULL,                                   /* create main configuration */
    NULL,                                   /* init main configuration */
    NULL,                                   /* create server configuration */
    NULL,                                   /* merge server configuration */
    ngx_rtmp_live_create_app_conf,          /* create app configuration */
    ngx_rtmp_live_merge_app_conf            /* merge app configuration */
};

ngx_module_t  ngx_rtmp_live_module = {
    NGX_MODULE_V1,
    &ngx_rtmp_live_module_ctx,              /* module context */
    ngx_rtmp_live_commands,                 /* module directives */
    NGX_RTMP_MODULE,                        /* module type */
    NULL,                                   /* init master */
    NULL,                                   /* init module */
    NULL,                                   /* init process */
    NULL,                                   /* init thread */
    NULL,                                   /* exit thread */
    NULL,                                   /* exit process */
    NULL,                                   /* exit master */
    NGX_MODULE_V1_PADDING
};


ngx_module_t *ngx_modules[] = {
	&ngx_rtmp_module,
    &ngx_rtmp_core_module,
    //&ngx_rtmp_live_module,
    NULL
};

 static ngx_uint_t argument_number[] = {
	 NGX_CONF_NOARGS,
	 NGX_CONF_TAKE1,
	 NGX_CONF_TAKE2,
	 NGX_CONF_TAKE3,
	 NGX_CONF_TAKE4,
	 NGX_CONF_TAKE5,
	 NGX_CONF_TAKE6,
	 NGX_CONF_TAKE7
 };

 char *
ngx_rtmp_merge_applications(ngx_conf_t *cf, ngx_array_t *applications,
            void **app_conf, ngx_rtmp_module_t *module, ngx_uint_t ctx_index)
{
    char                           *rv;
    ngx_rtmp_conf_ctx_t            *ctx, saved;
    ngx_rtmp_core_app_conf_t      **cacfp;
    ngx_uint_t                      n;
    ngx_rtmp_core_app_conf_t       *cacf;
    if (applications == NULL) {
        return NGX_CONF_OK;
    }

    ctx = (ngx_rtmp_conf_ctx_t *) cf->ctx;
    saved = *ctx;

    cacfp = applications->elts;
    for (n = 0; n < applications->nelts; ++n, ++cacfp) {

        ctx->app_conf = (*cacfp)->app_conf;

        rv = module->merge_app_conf(cf, app_conf[ctx_index],
                (*cacfp)->app_conf[ctx_index]);
        if (rv != NGX_CONF_OK) {
            return rv;
        }

        cacf = (*cacfp)->app_conf[ngx_rtmp_core_module.ctx_index];
        rv = ngx_rtmp_merge_applications(cf, &cacf->applications,
                                         (*cacfp)->app_conf,
                                         module, ctx_index);
        if (rv != NGX_CONF_OK) {
            return rv;
        }
    }

    *ctx = saved;

    return NGX_CONF_OK;
}

 char *
ngx_rtmp_core_application(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char                       *rv;
    ngx_int_t                   i;
    ngx_str_t                  *value;
    ngx_conf_t                  save;
    ngx_rtmp_module_t          *module;
    ngx_rtmp_conf_ctx_t        *ctx, *pctx;
    ngx_rtmp_core_srv_conf_t   *cscf;
    ngx_rtmp_core_app_conf_t   *cacf, **cacfp;

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_conf_ctx_t));
    if (ctx == NULL) {
        return NGX_CONF_ERROR;
    }

    pctx = cf->ctx;
    ctx->main_conf = pctx->main_conf;
    ctx->srv_conf = pctx->srv_conf;

    ctx->app_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_rtmp_max_module);
    if (ctx->app_conf == NULL) {
        return NGX_CONF_ERROR;
    }

    for (i = 0; ngx_modules[i]; i++) {
        if (ngx_modules[i]->type != NGX_RTMP_MODULE) {
            continue;
        }

        module = ngx_modules[i]->ctx;

        if (module->create_app_conf) {
            ctx->app_conf[ngx_modules[i]->ctx_index] =
                                module->create_app_conf(cf);
            if (ctx->app_conf[ngx_modules[i]->ctx_index] == NULL) {
                return NGX_CONF_ERROR;
            }
        }
    }

    cacf = ctx->app_conf[ngx_rtmp_core_module.ctx_index];
    cacf->app_conf = ctx->app_conf;

    value = cf->args->elts;

    cacf->name = value[1];
    cscf = pctx->srv_conf[ngx_rtmp_core_module.ctx_index];

    cacfp = ngx_array_push(&cscf->applications);
    if (cacfp == NULL) {
        return NGX_CONF_ERROR;
    }

    *cacfp = cacf;

    save = *cf;
    cf->ctx = ctx;
    cf->cmd_type = NGX_RTMP_APP_CONF;

   // rv = ngx_conf_parse(cf, NULL);

    *cf= save;

    return rv;
}



 ngx_int_t
ngx_rtmp_live_postconfiguration(ngx_conf_t *cf)
{
    ngx_rtmp_core_main_conf_t          *cmcf;
 //   ngx_rtmp_handler_pt                *h;

    cmcf = ngx_rtmp_conf_get_module_main_conf(cf, ngx_rtmp_core_module);

    /* register raw event handlers */

 //   h = ngx_array_push(&cmcf->events[NGX_RTMP_MSG_AUDIO]);
 //   *h = ngx_rtmp_live_av;

//    h = ngx_array_push(&cmcf->events[NGX_RTMP_MSG_VIDEO]);
 //   *h = ngx_rtmp_live_av;

    /* chain handlers */
/*
    next_publish = ngx_rtmp_publish;
    ngx_rtmp_publish = ngx_rtmp_live_publish;

    next_play = ngx_rtmp_play;
    ngx_rtmp_play = ngx_rtmp_live_play;

    next_close_stream = ngx_rtmp_close_stream;
    ngx_rtmp_close_stream = ngx_rtmp_live_close_stream;

    next_pause = ngx_rtmp_pause;
    ngx_rtmp_pause = ngx_rtmp_live_pause;

    next_stream_begin = ngx_rtmp_stream_begin;
    ngx_rtmp_stream_begin = ngx_rtmp_live_stream_begin;

    next_stream_eof = ngx_rtmp_stream_eof;
    ngx_rtmp_stream_eof = ngx_rtmp_live_stream_eof;
*/
    return NGX_OK;
}

 void *
ngx_rtmp_live_create_app_conf(ngx_conf_t *cf)
{
    ngx_rtmp_live_app_conf_t      *lacf;

    lacf = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_live_app_conf_t));
    if (lacf == NULL) {
        return NULL;
    }

    lacf->live = NGX_CONF_UNSET;
    lacf->nbuckets = NGX_CONF_UNSET;
    lacf->buflen = NGX_CONF_UNSET_MSEC;
    lacf->sync = NGX_CONF_UNSET_MSEC;
    lacf->idle_timeout = NGX_CONF_UNSET_MSEC;
    lacf->interleave = NGX_CONF_UNSET;
    lacf->wait_key = NGX_CONF_UNSET;
    lacf->wait_video = NGX_CONF_UNSET;
    lacf->publish_notify = NGX_CONF_UNSET;
    lacf->play_restart = NGX_CONF_UNSET;
    lacf->idle_streams = NGX_CONF_UNSET;

    return lacf;
}

 char *
ngx_rtmp_live_merge_app_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_rtmp_live_app_conf_t *prev = parent;
    ngx_rtmp_live_app_conf_t *conf = child;

    ngx_conf_merge_value(conf->live, prev->live, 0);
    ngx_conf_merge_value(conf->nbuckets, prev->nbuckets, 1024);
    ngx_conf_merge_msec_value(conf->buflen, prev->buflen, 0);
    ngx_conf_merge_msec_value(conf->sync, prev->sync, 300);
    ngx_conf_merge_msec_value(conf->idle_timeout, prev->idle_timeout, 0);
    ngx_conf_merge_value(conf->interleave, prev->interleave, 0);
    ngx_conf_merge_value(conf->wait_key, prev->wait_key, 1);
    ngx_conf_merge_value(conf->wait_video, prev->wait_video, 0);
    ngx_conf_merge_value(conf->publish_notify, prev->publish_notify, 0);
    ngx_conf_merge_value(conf->play_restart, prev->play_restart, 0);
    ngx_conf_merge_value(conf->idle_streams, prev->idle_streams, 1);

    conf->pool = ngx_create_pool(4096);
    if (conf->pool == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->streams = ngx_pcalloc(cf->pool,
            sizeof(ngx_rtmp_live_stream_t *) * conf->nbuckets);

    return NGX_CONF_OK;
}


char *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;
#if 0
    ngx_str_t        *value;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    if (*fp != NGX_CONF_UNSET) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } else if (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } else {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        return NGX_CONF_ERROR;
    }

    if (cmd->post) {
        post = cmd->post;
        return post->post_handler(cf, post, fp);
    }
#endif
    return NGX_CONF_OK;
}



 char *
ngx_rtmp_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char                        *rv;
    ngx_uint_t                   i, m, mi, s;
    ngx_conf_t                   pcf;
    ngx_array_t                  ports;
//    ngx_rtmp_listen_t           *listen;
    ngx_rtmp_module_t           *module;
    ngx_rtmp_conf_ctx_t         *ctx;
    ngx_rtmp_core_srv_conf_t    *cscf, **cscfp;
    ngx_rtmp_core_main_conf_t   *cmcf;

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_conf_ctx_t));
    if (ctx == NULL) {
        return NGX_CONF_ERROR;
    }

    *(ngx_rtmp_conf_ctx_t **) conf = ctx;

    /* count the number of the rtmp modules and set up their indices */

    ngx_rtmp_max_module = 0;
    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_RTMP_MODULE) {
            continue;
        }

        ngx_modules[m]->ctx_index = ngx_rtmp_max_module++;
    }


    /* the rtmp main_conf context, it is the same in the all rtmp contexts */

    ctx->main_conf = ngx_pcalloc(cf->pool,
                                 sizeof(void *) * ngx_rtmp_max_module);
    if (ctx->main_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * the rtmp null srv_conf context, it is used to merge
     * the server{}s' srv_conf's
     */

    ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_rtmp_max_module);
    if (ctx->srv_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * the rtmp null app_conf context, it is used to merge
     * the server{}s' app_conf's
     */

    ctx->app_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_rtmp_max_module);
    if (ctx->app_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * create the main_conf's, the null srv_conf's, and the null app_conf's
     * of the all rtmp modules
     */

    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_RTMP_MODULE) {
            continue;
        }

        module = ngx_modules[m]->ctx;
        mi = ngx_modules[m]->ctx_index;

        if (module->create_main_conf) {
            ctx->main_conf[mi] = module->create_main_conf(cf);
            if (ctx->main_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_srv_conf) {
            ctx->srv_conf[mi] = module->create_srv_conf(cf);
            if (ctx->srv_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_app_conf) {
            ctx->app_conf[mi] = module->create_app_conf(cf);
            if (ctx->app_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }
    }

    pcf = *cf;
    cf->ctx = ctx;

    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_RTMP_MODULE) {
            continue;
        }

        module = ngx_modules[m]->ctx;

        if (module->preconfiguration) {
            if (module->preconfiguration(cf) != NGX_OK) {
                return NGX_CONF_ERROR;
            }
        }
    }

    /* parse inside the rtmp{} block */

    cf->module_type = NGX_RTMP_MODULE;
    cf->cmd_type = NGX_RTMP_MAIN_CONF;
  	//rv = ngx_conf_parse(cf, NULL);
  	
  	////////////////////////////////////////////////////////
  	cf->args->nelts = 0;
  	ngx_str_t   *word = ngx_array_push(cf->args);
    if (word == NULL) {
		printf("word == NULL\n");
        return NGX_ERROR;
    }

    word->data = ngx_pnalloc(cf->pool, strlen("server"));
    if (word->data == NULL) {
		printf("word->data == NULL\n");
        return NGX_ERROR;
    }
	strcpy(word->data, "server");
	word->len = strlen("server");
	
	ngx_conf_handler(cf);
	///////////////////////////////////////////////////////
	cf->args->nelts = 0;
  	ngx_str_t   *word2 = ngx_array_push(cf->args);
    if (word2 == NULL) {
		printf("word == NULL\n");
        return NGX_ERROR;
    }

    word2->data = ngx_pnalloc(cf->pool, strlen("application"));
    if (word2->data == NULL) {
		printf("word->data == NULL\n");
        return NGX_ERROR;
    }
	strcpy(word2->data, "application");
	word2->len = strlen("application");
	
	ngx_conf_handler(cf);
	//////////////////////////////////////////////////////
	

	//if ( != NGX_OK) {
	//	printf("ngx_conf_handler rv != NGX_CONF_OK\n");
	//*cf = pcf;
	//  return rv;
	//}


    /* init rtmp{} main_conf's, merge the server{}s' srv_conf's */

    cmcf = ctx->main_conf[ngx_rtmp_core_module.ctx_index];
	printf("main_conf ngx_rtmp_core_module.ctx_index %d\n",ngx_rtmp_core_module.ctx_index);

    cscfp = cmcf->servers.elts;

    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_RTMP_MODULE) {
            continue;
        }

        module = ngx_modules[m]->ctx;
        mi = ngx_modules[m]->ctx_index;

        /* init rtmp{} main_conf's */

        cf->ctx = ctx;

        if (module->init_main_conf) {
            rv = module->init_main_conf(cf, ctx->main_conf[mi]);
            if (rv != NGX_CONF_OK) {
                *cf = pcf;
                return rv;
            }
        }
        for (s = 0; s < cmcf->servers.nelts; s++) {

            /* merge the server{}s' srv_conf's */

            cf->ctx = cscfp[s]->ctx;

            if (module->merge_srv_conf) {
                rv = module->merge_srv_conf(cf,
                                            ctx->srv_conf[mi],
                                            cscfp[s]->ctx->srv_conf[mi]);
				ngx_rtmp_core_srv_conf_t *testv = cscfp[s]->ctx->srv_conf[mi];
				ngx_rtmp_conf_ctx_t    *testctx = cf->ctx;
				ngx_rtmp_core_srv_conf_t *testv2 = testctx->srv_conf[0];
                if (rv != NGX_CONF_OK) {
                    *cf = pcf;
                    return rv;
                }
            }

            if (module->merge_app_conf) {

                /* merge the server{}'s app_conf */

                /*ctx->app_conf = cscfp[s]->ctx->loc_conf;*/

                rv = module->merge_app_conf(cf,
                                            ctx->app_conf[mi],
                                            cscfp[s]->ctx->app_conf[mi]);
                if (rv != NGX_CONF_OK) {
                    *cf = pcf;
                    return rv;
                }

                /* merge the applications{}' app_conf's */

                cscf = cscfp[s]->ctx->srv_conf[ngx_rtmp_core_module.ctx_index];

                rv = ngx_rtmp_merge_applications(cf, &cscf->applications,
                                            cscfp[s]->ctx->app_conf,
                                            module, mi);
                if (rv != NGX_CONF_OK) {
                    *cf = pcf;
                    return rv;
                }
            }

        }
    }

/*
    if (ngx_rtmp_init_events(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
*/
    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_RTMP_MODULE) {
            continue;
        }

        module = ngx_modules[m]->ctx;

        if (module->postconfiguration) {
            if (module->postconfiguration(cf) != NGX_OK) {
                return NGX_CONF_ERROR;
            }
        }
    }

//    *cf = pcf;
/*
    if (ngx_rtmp_init_event_handlers(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    if (ngx_array_init(&ports, cf->temp_pool, 4, sizeof(ngx_rtmp_conf_port_t))
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    listen = cmcf->listen.elts;

    for (i = 0; i < cmcf->listen.nelts; i++) {
        if (ngx_rtmp_add_ports(cf, &ports, &listen[i]) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
    }
*/

   return NGX_CONF_OK;//return ngx_rtmp_optimize_servers(cf, &ports);
}
 

ngx_int_t
ngx_conf_handler(ngx_conf_t *cf)
{

 char			*rv;
 void			*conf, **confp;
 ngx_uint_t 	 i, found;
 ngx_str_t		*name;
 ngx_command_t	*cmd;
 

 ngx_array_t *_args = (ngx_array_t *)cf->args;
 
 name = _args->elts;

 found = 0;
 for (i = 0; ngx_modules[i]; i++) {
	 cmd = ngx_modules[i]->commands;
	 if (cmd == NULL) {
		 continue;
	 }

	 for ( /* void */ ; cmd->name.len; cmd++) {

		 if (name->len != cmd->name.len) {
			 continue;
		 }

		 if (ngx_strcmp(name->data, cmd->name.data) != 0) {
			 continue;
		 }

		 found = 1;

		 if (ngx_modules[i]->type != NGX_CONF_MODULE
			 && ngx_modules[i]->type != cf->module_type)
		 {
			 continue;
		 }

		 /* is the directive's location right ? */

		 if (!(cmd->type & cf->cmd_type)) {
			 continue;
		 }

// 		   if (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
// 			   ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
// 								 "directive \"%s\" is not terminated by \";\"",
// 								 name->data);
// 			   return NGX_ERROR;
// 		   }

// 		   if ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
// 			   ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
// 								  "directive \"%s\" has no opening \"{\"",
// 								  name->data);
// 			   return NGX_ERROR;
// 		   }

		 /* is the directive's argument count right ? */

		 if (!(cmd->type & NGX_CONF_ANY)) {

			 if (cmd->type & NGX_CONF_FLAG) {

				 if (cf->args->nelts != 2) {
					 goto invalid;
				 }

			 } else if (cmd->type & NGX_CONF_1MORE) {

				 if (cf->args->nelts < 2) {
					 goto invalid;
				 }

			 } else if (cmd->type & NGX_CONF_2MORE) {

				 if (cf->args->nelts < 3) {
					 goto invalid;
				 }

			 } else if (cf->args->nelts > NGX_CONF_MAX_ARGS) {

				 goto invalid;

			 } else if (!(cmd->type & argument_number[cf->args->nelts - 1]))
			 {
				 goto invalid;
			 }
		 }

		 /* set up the directive's configuration context */

		 conf = NULL;

		 if (cmd->type & NGX_DIRECT_CONF) {
			 conf = ((void **) cf->ctx)[ngx_modules[i]->index];

		 } else if (cmd->type & NGX_MAIN_CONF) {
			 conf = &(((void **) cf->ctx)[ngx_modules[i]->index]);

		 } else if (cf->ctx) {
			 confp = *(void **) ((char *) cf->ctx + cmd->conf);

			 if (confp) {
				 conf = confp[ngx_modules[i]->ctx_index];
			 }
		 }

		 rv = cmd->set(cf, cmd, conf);

		 if (rv == NGX_CONF_OK) {
			 return NGX_OK;
		 }

		 if (rv == NGX_CONF_ERROR) {
			 return NGX_ERROR;
		 }


		 return NGX_ERROR;
	 }
 }

 if (found) {
	 printf("\"%s\" directive is not allowed here\n", name->data);

	 return NGX_ERROR;
 }

 printf("unknown directive \"%s\"\n", name->data);

 return NGX_ERROR;

invalid:

 printf("invalid number of arguments in \"%s\" directive\n",
					name->data);

 return NGX_ERROR;
}

ngx_chain_t *
ngx_rtmp_alloc_in_buf(ngx_rtmp_session_t *s)
{
    ngx_chain_t        *cl;
    ngx_buf_t          *b;
    size_t              size;

    if ((cl = ngx_alloc_chain_link(s->in_pool)) == NULL
       || (cl->buf = ngx_calloc_buf(s->in_pool)) == NULL)
    {
        return NULL;
    }

    cl->next = NULL;
    b = cl->buf;
    size = s->in_chunk_size + NGX_RTMP_MAX_CHUNK_HEADER;

    b->start = b->last = b->pos = ngx_palloc(s->in_pool, size);
    if (b->start == NULL) {
        return NULL;
    }
    b->end = b->start + size;

    return cl;
}
 ngx_int_t
ngx_rtmp_set_chunk_size(ngx_rtmp_session_t *s, ngx_uint_t size)
{
    ngx_rtmp_core_srv_conf_t           *cscf;
    ngx_chain_t                        *li, *fli, *lo, *flo;
    ngx_buf_t                          *bi, *bo;
    ngx_int_t                           n;


    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    s->in_old_pool = s->in_pool;
    s->in_chunk_size = size;
    s->in_pool = ngx_create_pool(4096);

    /* copy existing chunk data */
    if (s->in_old_pool) {
        s->in_chunk_size_changing = 1;
        s->in_streams[0].in = NULL;

        for(n = 1; n < cscf->max_streams; ++n) {
            /* stream buffer is circular
             * for all streams except for the current one
             * (which caused this chunk size change);
             * we can simply ignore it */
            li = s->in_streams[n].in;
            if (li == NULL || li->next == NULL) {
                s->in_streams[n].in = NULL;
                continue;
            }
            /* move from last to the first */
            li = li->next;
            fli = li;
            lo = ngx_rtmp_alloc_in_buf(s);
            if (lo == NULL) {
                return NGX_ERROR;
            }
            flo = lo;
            for ( ;; ) {
                bi = li->buf;
                bo = lo->buf;

                if (bo->end - bo->last >= bi->last - bi->pos) {
                    bo->last = ngx_cpymem(bo->last, bi->pos,
                            bi->last - bi->pos);
                    li = li->next;
                    if (li == fli)  {
                        lo->next = flo;
                        s->in_streams[n].in = lo;
                        break;
                    }
                    continue;
                }

                bi->pos += (ngx_cpymem(bo->last, bi->pos,
                            bo->end - bo->last) - bo->last);
                lo->next = ngx_rtmp_alloc_in_buf(s);
                lo = lo->next;
                if (lo == NULL) {
                    return NGX_ERROR;
                }
            }
        }
    }

    return NGX_OK;
}

ngx_rtmp_chain_t *
ngx_rtmp_alloc_shared_buf(ngx_rtmp_core_srv_conf_t *cscf)
{
 u_char 					*p = NULL;
 ngx_rtmp_chain_t			*out;
 ngx_rtmp_buf_t 			*b;
 size_t 					size;

 if (cscf->free) {
	 out = cscf->free;
	 cscf->free = out->next;
	 if(out->buf == NULL)
	 {
	 	//printf("out->buf == NULL!!!!!!!!!!!!!\n");
	 }

 } else {
  	//printf("%s cscf->chunk_size %d\n",__func__, cscf->chunk_size);
	 size = cscf->chunk_size + NGX_RTMP_MAX_CHUNK_HEADER;

	if(cscf->pool)
	{
	 	p = ngx_pcalloc(cscf->pool, NGX_RTMP_REFCOUNT_BYTES
			 + sizeof(ngx_chain_t)
			 + sizeof(ngx_buf_t)
			 + size);
	}
	
	 if (p == NULL) {
		 //printf("%s malloc NULL \n",__func__);
		 return NULL;
	 }
	 p += NGX_RTMP_REFCOUNT_BYTES;
	 out = (ngx_rtmp_chain_t *)p;

	 p += sizeof(ngx_rtmp_chain_t);
	 out->buf = (ngx_rtmp_buf_t *)p;

	 p += sizeof(ngx_rtmp_buf_t);
	 out->buf->start = p;
	 out->buf->end = p + size;
 	//printf("%s  size %d\n",__func__, size);
 }
 //printf("%s 3\n",__func__);

 out->next = NULL;
 b = out->buf;
 b->pos = b->last = b->start + NGX_RTMP_MAX_CHUNK_HEADER;
 b->memory = 1;

 /* buffer has refcount =1 when created! */
 ngx_rtmp_ref_set(out, 1);

 //printf("%s END \n",__func__);

 return out;
}



u_char *
rtmp_cpystrn(u_char *dst, u_char *src, size_t n)
{
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;

        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}

void
ngx_rtmp_cmd_fill_args(u_char name[RTMP_MAX_NAME],
        u_char args[RTMP_MAX_ARGS])
{
    u_char      *p;

    p = (u_char *)rtmp_strchr(name, '?');
    if (p == NULL) {
        return;
    }

    *p++ = 0;
    rtmp_cpystrn(args, p, RTMP_MAX_ARGS);
}

  u_char *
rtmp_strlchr(u_char *p, u_char *last, u_char c)
{
    while (p < last) {

        if (*p == c) {
            return p;
        }

        p++;
    }

    return NULL;
}



void
ngx_rtmp_send(void *ts)
{
	ngx_rtmp_session_t *s = (ngx_rtmp_session_t *)ts;

	if(s == NULL)
	{
		printf("%s s == NULL\n",__func__);
		return;
	}
	
    ngx_int_t                   n;
    ngx_rtmp_core_srv_conf_t   *cscf;
	u_int nbytes=0;
	
	
    if (s->out_chain == NULL && s->out_pos != s->out_last) {
        s->out_chain = s->out[s->out_pos];
        s->out_bpos = s->out_chain->buf->pos;
		
		//printf("[new size %d\r\n",s->out_chain->buf->last - s->out_chain->buf->pos);
    }
	
	if(s->tpcb == NULL)
	{
		printf("tpc NULL!\n");
		return;
	}
	
	// TCP PCB Setting
	s->tpcb->flags &= ~(TF_ACK_DELAY);
	tcp_nagle_disable(s->tpcb);
	//printf("sv%d sa%d ",s->m_sndvideo,s->m_sndaudio);
    if(s->out_chain) {
			
		s->sending = 1;
		if(s == NULL)
		{
			printf("%s s == NULL\n",__func__);
			return;
		}
		
		if(s->out_chain->buf == NULL)
		{
			printf("%s out_chain->buf == NULL\r\n",__func__);
			return;
		}

        nbytes = s->out_chain->buf->last - s->out_bpos;
				
		if(nbytes > 1460)
		{	
			nbytes = 1460;
		}

#if 0		
		if(s->tpcb->snd_buf < TCP_MSS/* && tcp_sndqueuelen(s->tpcb) < TCP_SND_QUEUELEN*/)
		{
			printf("%s snd_buf%d < TCP_MSS\r\n",__func__,s->tpcb->snd_buf);
			if(s->playstart==1)
			{
				//timer_sched_timeout_cb(1,TIMEOUT_ONESHOT, ngx_rtmp_send, s);
			}
						
			 return;
		}
		else
#endif			
		if(s->tpcb->snd_buf < nbytes)
		{
//			printf("%s snd_buf%d < nbytes %d \r\n",__func__,s->tpcb->snd_buf,nbytes);
			if(s->playstart==1)
			{
				//timer_sched_timeout_cb(1,TIMEOUT_ONESHOT, ngx_rtmp_send, s);
			}
						
			 return;
		}

		//printf("last %x, s->out_bpos %x nbytes %d\r\n",s->out_chain->buf->last, s->out_bpos, nbytes);

	    n = tcp_write(s->tpcb, (const void*)s->out_bpos,  nbytes, 0);

		if(rtmp_debug_on == 1 && rtmp_send_msg == 1)
		{
	        printf("--------------\n");
			int tti;
			for(tti = 0; tti<nbytes; tti ++)
				printf("%x ", *(s->out_bpos+tti));
			printf("--------------\n");  
		}
		

        if (n < 0) {
			
			printf("nbytes %d n %d\r\n",nbytes,n);			
			if(s->playstart==1)
			{
				//timer_sched_timeout_cb(1,TIMEOUT_ONESHOT, ngx_rtmp_send, s);
			}
            return;
        }
		
        s->out_bytes += nbytes;
		
        s->ping_reset = 1;
        //printf("s->out_bpos %x\r\n",s->out_bpos);
        s->out_bpos += nbytes;
        //printf("s->out_bpos %x out_bytes %d last %x\r\n",s->out_bpos,s->out_bytes , s->out_chain->buf->last);
        if (s->out_bpos == s->out_chain->buf->last) {
			
            s->out_chain = s->out_chain->next;
			
            if (s->out_chain == NULL) {
				
                cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
                ngx_rtmp_free_shared_chain(cscf, s->out[s->out_pos]);
				//printf("[s->out_pos %d out_last %d\r\n",s->out_pos, s->out_last);
                ++s->out_pos;
                s->out_pos %= s->out_queue;

                if (s->out_pos == s->out_last) {
					s->sending = 0;
					tcp_output(s->tpcb);
					/////////////
					//ngx_rtmp_free_shared_chain(cscf, s->out);
					/////////////
					//printf("End! out_bytes %d\r\n",s->out_bytes);
					s->out_bytes = 0;

					//if(s->m_sndvideo == 1)
					///	s->m_sndvideo = 2;
					//if (s->m_sndaudio == 1)
					//{
					//	s->m_sndaudio = 0;
					//	s->m_sndvideo = 0;
					//}
					//printf("Ev%d Ea%d ",s->m_sndvideo,s->m_sndaudio);
                    return;
                }
                s->out_chain = s->out[s->out_pos];
				//printf("s->out_pos %d]\r\n",s->out_pos);
            }
			
            s->out_bpos = s->out_chain->buf->pos;
			//printf("new size %d\r\n",s->out_chain->buf->last - s->out_chain->buf->pos);
			//tcp_output(s->tpcb);
        }


    }
	//if(s->out_chain&&s->playstart==1 && s->sending == 1)
	//{
	//	tcp_output(s->tpcb);
		//timer_sched_timeout_cb(1,TIMEOUT_ONESHOT, ngx_rtmp_send, s);
	//}

}


ngx_int_t
ngx_rtmp_send_message(ngx_rtmp_session_t *s, ngx_chain_t *out,
        u_int priority)
{
    u_int                      nmsg;

	//if(s->sending==1)  
	//{
		//printf("%s s->sending\n",__func__);
	//	return NGX_AGAIN;
	//}
    nmsg = (s->out_last - s->out_pos) % s->out_queue + 1;

    if (priority > 3) {
        priority = 3;
    }

    s->out[s->out_last++] = out;
    s->out_last %= s->out_queue;

    ngx_rtmp_acquire_shared_chain(out);

    if (priority && s->snd_buffer && nmsg < s->out_cork) {
        return NGX_OK;
    }

    ngx_rtmp_send(s);

    return NGX_OK;
}

 int
ngx_rtmp_send_shared_packet(ngx_rtmp_session_t *s, ngx_rtmp_chain_t *cl)
{
    ngx_rtmp_core_srv_conf_t       *cscf;
    int                       rc;

    if (cl == NULL) {
        return RTMP_ERROR;
    }

    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    rc = ngx_rtmp_send_message(s, cl, 0);

    ngx_rtmp_free_shared_chain(cscf, cl);
	
    return rc;
}

void *
ngx_rtmp_core_create_main_conf(ngx_conf_t *cf)
{
 ngx_rtmp_core_main_conf_t	*cmcf;

 cmcf = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_core_main_conf_t));
 if (cmcf == NULL) {
	 return NULL;
 }

 ngx_rtmp_core_main_conf = cmcf;

 if (ngx_array_init(&cmcf->servers, cf->pool, 4,
					sizeof(ngx_rtmp_core_srv_conf_t *))
	 != NGX_OK)
 {
	 return NULL;
 }

//    if (ngx_array_init(&cmcf->listen, cf->pool, 4, sizeof(ngx_rtmp_listen_t))
// 	   != NGX_OK)
//    {
// 	   return NULL;
//    }


 return cmcf;
}


void *
ngx_rtmp_core_create_srv_conf(ngx_conf_t *cf)
{
 ngx_rtmp_core_srv_conf_t	*conf;

 conf = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_core_srv_conf_t));
 if (conf == NULL) {
	 return NULL;
 }

 if (ngx_array_init(&conf->applications, cf->pool, 4,
					sizeof(ngx_rtmp_core_app_conf_t *))
	 != NGX_OK)
 {
	 return NULL;
 }

 conf->timeout = NGX_CONF_UNSET_MSEC;
 conf->ping = NGX_CONF_UNSET_MSEC;
 conf->ping_timeout = NGX_CONF_UNSET_MSEC;
 conf->so_keepalive = NGX_CONF_UNSET;
 conf->max_streams = NGX_CONF_UNSET;
 conf->chunk_size = NGX_CONF_UNSET;
 conf->ack_window = NGX_CONF_UNSET_UINT;
 conf->max_message = NGX_CONF_UNSET_SIZE;
 conf->out_queue = NGX_CONF_UNSET_SIZE;
 conf->out_cork = NGX_CONF_UNSET_SIZE;
 conf->play_time_fix = NGX_CONF_UNSET;
 conf->publish_time_fix = NGX_CONF_UNSET;
 conf->buflen = NGX_CONF_UNSET_MSEC;
 conf->busy = NGX_CONF_UNSET;


 return conf;
}


char *
ngx_rtmp_core_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child)
{
 ngx_rtmp_core_srv_conf_t *prev = parent;
 ngx_rtmp_core_srv_conf_t *conf = child;

 ngx_conf_merge_msec_value(conf->timeout, prev->timeout, 60000);
 ngx_conf_merge_msec_value(conf->ping, prev->ping, 60000);
 ngx_conf_merge_msec_value(conf->ping_timeout, prev->ping_timeout, 30000);

 ngx_conf_merge_value(conf->so_keepalive, prev->so_keepalive, 0);
 ngx_conf_merge_value(conf->max_streams, prev->max_streams, RTMP_MAX_STREAM);
 ngx_conf_merge_value(conf->chunk_size, prev->chunk_size, RTMP_CHUNK_SIZE/*1452 1400 4096*/);
 ngx_conf_merge_uint_value(conf->ack_window, prev->ack_window, RTMP_MAX_WINDOW);
 ngx_conf_merge_size_value(conf->max_message, prev->max_message, RTMP_MAX_MESSAGE_SIZE);
 ngx_conf_merge_size_value(conf->out_queue, prev->out_queue, RTMP_OUT_CNT/*180 256*/);
 ngx_conf_merge_size_value(conf->out_cork, prev->out_cork,
		 conf->out_queue / 8);
 ngx_conf_merge_value(conf->play_time_fix, prev->play_time_fix, 1);
 ngx_conf_merge_value(conf->publish_time_fix, prev->publish_time_fix, 1);
 ngx_conf_merge_msec_value(conf->buflen, prev->buflen, 1000);
 ngx_conf_merge_value(conf->busy, prev->busy, 0);

 if (prev->pool == NULL) {
	 prev->pool = ngx_create_pool(4096/*, &cf->cycle->new_log*/);
	 if (prev->pool == NULL) {
		 return NGX_CONF_ERROR;
	 }
 }

 conf->pool = prev->pool;

 return NGX_CONF_OK;
}


void *
ngx_rtmp_core_create_app_conf(ngx_conf_t *cf)
{
 ngx_rtmp_core_app_conf_t	*conf;

 conf = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_core_app_conf_t));
 if (conf == NULL) {
	 return NULL;
 }

 if (ngx_array_init(&conf->applications, cf->pool, 1,
					sizeof(ngx_rtmp_core_app_conf_t *))
	 != NGX_OK)
 {
	 return NULL;
 }

 return conf;
}


char *
ngx_rtmp_core_merge_app_conf(ngx_conf_t *cf, void *parent, void *child)
{
 ngx_rtmp_core_app_conf_t *prev = parent;
 ngx_rtmp_core_app_conf_t *conf = child;

 (void)prev;
 (void)conf;

 return NGX_CONF_OK;
}

char *
ngx_rtmp_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
 char						*rv;
 void						*mconf;
 ngx_uint_t 				 m;
 ngx_conf_t 				 pcf;
 ngx_rtmp_module_t			*module;
 ngx_rtmp_conf_ctx_t		*ctx, *rtmp_ctx;
 ngx_rtmp_core_srv_conf_t	*cscf, **cscfp;
 ngx_rtmp_core_main_conf_t	*cmcf;

 ctx = ngx_pcalloc(cf->pool, sizeof(ngx_rtmp_conf_ctx_t));
 if (ctx == NULL) {
	 return NGX_CONF_ERROR;
 }

 rtmp_ctx = cf->ctx;
 ctx->main_conf = rtmp_ctx->main_conf;

 /* the server{}'s srv_conf */

 ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_rtmp_max_module);
 if (ctx->srv_conf == NULL) {
	 return NGX_CONF_ERROR;
 }

 ctx->app_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_rtmp_max_module);
 if (ctx->app_conf == NULL) {
	 return NGX_CONF_ERROR;
 }

 for (m = 0; ngx_modules[m]; m++) {
	 if (ngx_modules[m]->type != NGX_RTMP_MODULE) {
		 continue;
	 }

	 module = ngx_modules[m]->ctx;

	 if (module->create_srv_conf) {
		 mconf = module->create_srv_conf(cf);
		 if (mconf == NULL) {
			 return NGX_CONF_ERROR;
		 }

		 ctx->srv_conf[ngx_modules[m]->ctx_index] = mconf;
	 }

	 if (module->create_app_conf) {
		 mconf = module->create_app_conf(cf);
		 if (mconf == NULL) {
			 return NGX_CONF_ERROR;
		 }

		 ctx->app_conf[ngx_modules[m]->ctx_index] = mconf;
	 }
 }

 /* the server configuration context */

 cscf = ctx->srv_conf[ngx_rtmp_core_module.ctx_index];
 cscf->ctx = ctx;

 cmcf = ctx->main_conf[ngx_rtmp_core_module.ctx_index];

 cscfp = ngx_array_push(&cmcf->servers);
 if (cscfp == NULL) {
	 return NGX_CONF_ERROR;
 }

 *cscfp = cscf;


 /* parse inside server{} */

 pcf = *cf;
 cf->ctx = ctx;
 cf->cmd_type = NGX_RTMP_SRV_CONF;

//    rv = ngx_conf_parse(cf, NULL);

 *cf = pcf;

 return rv;
}


 void
ngx_rtmp_recv(ngx_rtmp_session_t         *s)
{
    ngx_int_t                   n;
    ngx_connection_t           *c;
    ngx_rtmp_core_srv_conf_t   *cscf;
    ngx_rtmp_header_t          *h;
    ngx_rtmp_stream_t          *st, *st0;
    ngx_chain_t                *in, *head;
    ngx_buf_t                  *b;
    u_char                     *p, *pp, *old_pos;
    size_t                      size, fsize, old_size;
    uint8                     fmt, ext;
    uint32                    csid, timestamp;
	
	int i, j;

//    c = rev->data;
//    s = c->data;
    old_pos = NULL;
    old_size = 0;
    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
//
//    if (c->destroyed) {
//        return;
//    }

    //for( ;; ) 
	{
		 printf("in s->in_csid %d\n",s->in_csid);
		 st = &s->in_streams[s->in_csid];

        /* allocate new buffer */
        if (st->in == NULL) {
            st->in = ngx_rtmp_alloc_in_buf(s);
            if (st->in == NULL) {
                printf("in buf NULL\n");
                ngx_rtmp_finalize_session(s);
                return;
            }
        }

        h  = &st->hdr;
        in = st->in;
        b  = in->buf;
		//in->buf = s->hs_buf;
		//b = s->hs_buf;

//		printf("ngx_rtmp_recv pos %x %x %x %x last-pos %d last %x %x %x %x\n",
//			*b->pos, *(b->pos+1), *(b->pos+2), *(b->pos+3), b->last - b->pos,*(b->last-3),*(b->last-2),*(b->last-1),*b->last);

        if (old_size) {


            b->pos = b->start;
            b->last = ngx_movemem(b->pos, old_pos, old_size);

 //           if (s->in_chunk_size_changing) {
 //               ngx_rtmp_finalize_set_chunk_size(s);
 //           }

        } else {

            if (old_pos) {
                b->pos = b->last = b->start;
            }

//            n = c->recv(c, b->last, b->end - b->last);

//            if (n == NGX_ERROR || n == 0) {
//                ngx_rtmp_finalize_session(s);
//                return;
//            }

//            if (n == NGX_AGAIN) {
//                if (ngx_handle_read_event(c->read, 0) != NGX_OK) {
//                    ngx_rtmp_finalize_session(s);
//                }
//                return;
//            }

//            s->ping_reset = 1;
//            ngx_rtmp_update_bandwidth(&ngx_rtmp_bw_in, n);
//            b->last += n;
//            s->in_bytes += n;

//            if (s->in_bytes >= 0xf0000000) {
//                printf("resetting byte counter");
//                s->in_bytes = 0;
//                s->in_last_ack = 0;
//            }

//            if (s->ack_size && s->in_bytes - s->in_last_ack >= s->ack_size) {
//
//                s->in_last_ack = s->in_bytes;
//
//                printf("sending RTMP ACK(%ld)", s->in_bytes);
//
//                if (ngx_rtmp_send_ack(s, s->in_bytes)) {
//                    ngx_rtmp_finalize_session(s);
//                    return;
//                }
//            }
        }

        old_pos = NULL;
        old_size = 0;


        /* parse headers */
        if (b->pos == b->start) {
			printf("b->pos == b->start \n");
            p = b->pos;

            /* chunk basic header */
            fmt  = (*p >> 6) & 0x03;
            csid = *p++ & 0x3f;

            if (csid == 0) {
                if (b->last - p < 1)
                    return;//continue;
                csid = 64;
                csid += *(uint8*)p++;

            } else if (csid == 1) {
                if (b->last - p < 2)
                    return;//continue;
                csid = 64;
                csid += *(uint8*)p++;
                csid += (uint32)256 * (*(uint8*)p++);
            }

            printf("RTMP bheader fmt=%d csid=%d\n",
                    (int)fmt, csid);

            if (csid >= (uint32)cscf->max_streams) {
                printf("RTMP in chunk stream too big: %d >= %d",
                    csid, cscf->max_streams);
                ngx_rtmp_finalize_session(s);
                return;
            }

#if 1
            /* link orphan */
            if (s->in_csid == 0) {

                /* unlink from stream #0 */
                st->in = st->in->next;

                /* link to new stream */
                s->in_csid = csid;
                st = &s->in_streams[csid];
                if (st->in == NULL) {
                    in->next = in;
                } else {
                    in->next = st->in->next;
                    st->in->next = in;
                }
                st->in = in;
                h = &st->hdr;
                h->csid = csid;
            }
#endif
            ext = st->ext;
            timestamp = st->dtime;
            if (fmt <= 2 ) {
                if (b->last - p < 3)
                    return;//continue;
                /* timestamp:
                 *  big-endian 3b -> little-endian 4b */
                pp = (u_char*)&timestamp;
                pp[1] = *p++;
                pp[2] = *p++;
                pp[3] = *p++;
                pp[0] = 0;
				printf("timestamp %d\n",timestamp);

                ext = (timestamp == 0x00ffffff);

                if (fmt <= 1) {
                    if (b->last - p < 4)
                        return;//continue;
                    /* size:
                     *  big-endian 3b -> little-endian 4b
                     * type:
                     *  1b -> 1b*/
                    pp = (u_char*)&h->mlen;
                    pp[1] = *p++;
                    pp[2] = *p++;
                    pp[3] = *p++;
                    pp[0] = 0;
                    h->type = *(uint8*)p++;
					printf("mlen %d h->type %d\n",h->mlen,h->type);

                    if (fmt == 0) {
                        if (b->last - p < 4)
                            return;//continue;
                        /* stream:
                         *  little-endian 4b -> little-endian 4b */
                        pp = (u_char*)&h->msid;
                        pp[0] = *p++;
                        pp[1] = *p++;
                        pp[2] = *p++;
                        pp[3] = *p++;
						printf("h->msid %d\n",h->msid);
                    }
                }
            }

            /* extended header */
            if (ext) {
                if (b->last - p < 4)
                    return;//continue;
                pp = (u_char*)&timestamp;
                pp[3] = *p++;
                pp[2] = *p++;
                pp[1] = *p++;
                pp[0] = *p++;
				printf("ext timestamp %d\n",timestamp);
            }

//            if (st->len == 0) {
//                /* Messages with type=3 should
//                 * never have ext timestamp field
//                 * according to standard.
//                 * However that's not always the case
//                 * in real life */
//                st->ext = (ext && cscf->publish_time_fix);
//                if (fmt) {
//                    st->dtime = timestamp;
//                } else {
//                    h->timestamp = timestamp;
//                    st->dtime = 0;
//                }
//            }

            //printf("RTMP mheader fmt=%d %s (%d) time=%d+%d mlen=%d len=%d msid=%d\n",
            //        (int)fmt, ngx_rtmp_message_type(h->type), (int)h->type,
            //        h->timestamp, st->dtime, h->mlen, st->len, h->msid);

            /* header done */
            b->pos = p;

            if (h->mlen > cscf->max_message) {
               printf("too big message: %d\n", cscf->max_message);
                ngx_rtmp_finalize_session(s);
                return;
            }
        }

        size = b->last - b->pos;
        fsize = h->mlen - st->len;
		
		//printf("size %d fsize %d  s->in_chunk_size %d \n",size, fsize,  s->in_chunk_size);

        if (size < ngx_min(fsize, s->in_chunk_size))
            return;//continue;

        /* buffer is ready */
		//printf("fsize %d > s->in_chunk_size %d\n", fsize , s->in_chunk_size);
#if 0
        if (fsize > s->in_chunk_size) {
			printf("fsize %d > s->in_chunk_size %d\n", fsize , s->in_chunk_size);
            /* collect fragmented chunks */
            st->len += s->in_chunk_size;
            b->last = b->pos + s->in_chunk_size;
            old_pos = b->last;
            old_size = size - s->in_chunk_size;

        } else 
#endif        
        {
#if 0
 			/* handle! */
            head = st->in->next;
            st->in->next = NULL;
            b->last = b->pos + fsize;
            old_pos = b->last;
            old_size = size - fsize;
            st->len = 0;
            h->timestamp += st->dtime;
#endif			
			//printf("\n datas \n");
			//int testi;
			//for(testi=0;testi<b->last-b->pos;testi++)
			//	printf("%x ",*(b->pos+testi));
			//printf("\n datas end\n");

			switch(h->type)
			{
				case NGX_RTMP_MSG_AMF3_CMD:
					h->mlen -= 1;
				case NGX_RTMP_MSG_AMF_CMD:
					printf("\n h->type %d s->cur_state %d\n",h->type,s->hs_stage);
					
//#ifdef TEST_SERVER1 				
				if(s->hs_stage == NGX_RTMP_HANDSHAKE_SERVER_DONE)
				{
					if(h->msid==0
						&&!(*(b->pos+(b->last-b->pos-3)) == 0x00 &&
						*(b->pos+(b->last-b->pos-2)) == 0x00 &&
						*(b->pos+(b->last-b->pos-1)) == 0x09)
						&&!(*(b->pos+(b->last-b->pos-3)) == 0x00 &&
						*(b->pos+(b->last-b->pos-2)) == 0x00 &&
						*(b->pos+(b->last-b->pos-1)) == 0x05)
						&&!(*(b->pos+(b->last-b->pos-3)) == 0x00 &&
						*(b->pos+(b->last-b->pos-2)) == 0x0B &&
						*(b->pos+(b->last-b->pos-1)) == 0xB8)
						&&!(*(b->pos+(b->last-b->pos-3)) == 0x00 &&
						*(b->pos+(b->last-b->pos-2)) == 0x00 &&
						*(b->pos+(b->last-b->pos-1)) == 0x00))
					{
						printf("not finished data!\n");
						return;
					}
//#else
					for(i= 0; i<in->buf->last-in->buf->pos; i++)
					{
						if(*(in->buf->pos+i) == 0xC3)
						{
							printf("skip %d[%x] \n",i,*(in->buf->pos+i));
							for(j= i; j<(in->buf->last - in->buf->pos); j++)
							{
								*(in->buf->pos+j) = *(in->buf->pos+(j+1));
								//printf("%x <= %x ",*(in->buf->pos+j),*(in->buf->pos+(j+1)));
							}
							in->buf->last--;
						}
					}
				}
//#endif					
					//printf("\n datas \n");
					//int testi;
					//for(testi=0;testi<in->buf->last - in->buf->pos;testi++)
					//	printf("%x ",*(in->buf->pos+testi));
					//printf("\n datas end\n");
					
					if(ngx_rtmp_amf_message_handler(s, h, in) != RTMP_OK)
					{
						printf("ngx_rtmp_amf_message_handler:	error\n");
						dprintf("call : rtmp_recv : Close(disconnect)\n");
						ngx_rtmp_finalize_session(s);
						return;
					}
					printf("ngx_rtmp_amf_message_handler END\n");
				break;
				case NGX_RTMP_MSG_USER:
					printf("\n h->type (%d) \n",h->type);
					//if(ngx_rtmp_user_message_handler(s, h, in) != RTMP_OK)
                    //{
					//	printf("ngx_rtmp_user_message_handler:	error\n");
					//	dprintf("call : rtmp_recv : Close(disconnect)\n");
					//	ngx_rtmp_finalize_session(s);
					//	return;
					//}          
					
        			//s->in_csid = 0;
					b->pos += h->mlen;
					b->start = b->pos;
					printf("ngx_rtmp_recv a\r\n");
					ngx_rtmp_recv(s);
                    break;          
				case NGX_RTMP_MSG_CHUNK_SIZE:
				case NGX_RTMP_MSG_ACK_SIZE:
				case NGX_RTMP_MSG_BANDWIDTH:
					printf("\n h->type (%d) \n",h->type);
					if(ngx_rtmp_protocol_message_handler(s, h, in) != RTMP_OK)
					{
						printf("ngx_rtmp_protocol_message_handler:	error\n");
						dprintf("call : rtmp_recv : Close(disconnect)\n");
						ngx_rtmp_finalize_session(s);
						return;
					}
					
        			//s->in_csid = 0;
					b->pos += h->mlen;
					b->start = b->pos;
					printf("ngx_rtmp_recv b\r\n");
					ngx_rtmp_recv(s);
					break;
			}

//            if (ngx_rtmp_receive_message(s, h, head) != NGX_OK) {
//                ngx_rtmp_finalize_session(s);
//                return;
//            }
			if(s==NULL)
				return;
            if (s->in_chunk_size_changing) {
                /* copy old data to a new buffer */
 //               if (!old_size) {
 //                   ngx_rtmp_finalize_set_chunk_size(s);
 //               }

            } else {
                /* add used bufs to stream #0 */
				if(s->in_streams)
				{
	                st0 = &s->in_streams[0];
					if(st0->in)
					{
		                st->in->next = st0->in;
		                st0->in = head;
		                st->in = NULL;
					}
				}
            }
        }

        s->in_csid = 0;
		s->lenofRTMP = 0;	   
		//printf("recv end!!\n");
		return;
    }
}

void
ngx_rtmp_cycle(ngx_rtmp_session_t *s)
{
//    ngx_connection_t 		  *c;

//    c = s->connection;
//    c->read->handler =  ngx_rtmp_recv;
//    c->write->handler = ngx_rtmp_send;

//    s->ping_evt.data = c;
//    s->ping_evt.log = c->log;
//    s->ping_evt.handler = ngx_rtmp_ping;
//    ngx_rtmp_reset_ping(s);

//    ngx_rtmp_recv(c->read);
}

#endif
