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
#include <stdlib.h>
#include <stdio.h>


static void
master_entry(void* arg)
{
  spedye_master_t *m = arg;
  m->state = SPEDYE_RUNNING;
  fprintf(stderr, "%p\n", m);
}

static void
master_shutdown(uv_async_t* handle, int status)
{
  spedye_master_t *m = handle->data;
  m->state = SPEDYE_STOPING;
  m->master_wakeup.data = NULL;
}

int
spedye_master_create(spedye_master_t **m_out, spedye_conf_t *conf, uv_loop_t *loop)
{
  int rc;
  spedye_master_t *m;

  *m_out = NULL;

  m = calloc(1, sizeof(spedye_master_t));
  m->loop = loop;
  m->state = SPEDYE_STARTING;
  uv_ref(m->loop);

  rc = uv_async_init(m->loop, &m->master_wakeup, master_shutdown);

  m->master_wakeup.data = m;

  *m_out = m;

  return rc;
}

int
spedye_master_run(spedye_master_t *m)
{
  int rc;

  rc = uv_thread_create(&m->master_thread, master_entry, (void*)m);

  return 0;
}

void
spedye_master_destroy(spedye_master_t *m)
{
  /* Send wakeup */
  uv_async_send(&m->master_wakeup);
  uv_thread_join(&m->master_thread);
  m->state = SPEDYE_STOPPED;
  uv_unref(m->loop);
  free(m);
}

