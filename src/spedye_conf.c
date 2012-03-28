/*
 *  Copyright 2012 The Spedye Authors. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "spedye.h"

static void
init_blank_config(spedye_conf_t *conf)
{
  spedye_vhost_t *vhost = calloc(1, sizeof(spedye_vhost_t));
  spedye_listener_t *lrec = calloc(1, sizeof(spedye_listener_t));
  conf->listeners = lrec;
  vhost->destport = 80;
  lrec->port = 443;
  lrec->vhosts = vhost;
}

int
spedye_conf_create(spedye_conf_t **p_conf)
{
  spedye_conf_t *conf;

  conf = calloc(1, sizeof(spedye_conf_t));

  init_blank_config(conf);

  *p_conf = conf;

  return 0;
}

void
spedye_conf_destroy(spedye_conf_t *conf)
{
  spedye_listener_t *lrec;
  spedye_listener_t *tmplrec;
  spedye_vhost_t *vhost;
  spedye_vhost_t *tmpvhost;

  for (lrec = conf->listeners; lrec != NULL; lrec = tmplrec) {
    tmplrec = lrec->next;
    for (vhost = lrec->vhosts; vhost != NULL; vhost = tmpvhost) {
      tmpvhost = vhost->next;
      if (vhost->sslctx) {
        SSL_CTX_free(vhost->sslctx);
      }
      if (vhost->name) {
        free((void*)vhost->name);
      }
      if (vhost->certpath) {
        free((void*)vhost->certpath);
      }
      if (vhost->keypath) {
        free((void*)vhost->keypath);
      }
      if (vhost->destaddress) {
        free((void*)vhost->destaddress);
      }
      free(vhost);
    }

    if (lrec->sslctx) {
      SSL_CTX_free(lrec->sslctx);
    }

    if (lrec->address) {
      free((void*)lrec->address);
    }

    free(lrec);
  }
}

