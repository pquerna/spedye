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

void worker_shutdown_closecb(uv_handle_t* handle)
{
  spedye_worker_t *w = handle->data;
  uv_unref(w->loop);
}

static void
worker_shutdown(uv_async_t* handle, int status)
{
  spedye_worker_t *w = handle->data;
  w->state = SPEDYE_STOPING;
  uv_close((uv_handle_t*)&w->worker_wakeup, worker_shutdown_closecb);
}

static void
worker_entry(void* arg)
{
  spedye_worker_t *w = arg;
  w->state = SPEDYE_RUNNING;
  uv_run(w->loop);
}

int
spedye_worker_create(spedye_worker_t **w_out, spedye_master_t *m)
{
  int rc;
  spedye_worker_t *w;

  *w_out = NULL;

  w = calloc(1, sizeof(spedye_worker_t));
  w->loop = uv_loop_new();
  w->state = SPEDYE_STARTING;
  uv_ref(w->loop);

  rc = uv_async_init(w->loop, &w->worker_wakeup, worker_shutdown);

  w->worker_wakeup.data = m;

  *w_out = w;

  return rc;
  
}

int spedye_worker_run(spedye_worker_t *w)
{
  int rc;

  rc = uv_thread_create(&w->worker_thread, worker_entry, (void*)w);

  return 0;
}

void spedye_worker_destroy(spedye_worker_t *w)
{
  /* Send wakeup */
  uv_async_send(&w->worker_wakeup);
  uv_thread_join(&w->worker_thread);
  w->state = SPEDYE_STOPPED;
  uv_loop_delete(w->loop);
  free(w);
}
