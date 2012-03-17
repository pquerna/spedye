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

#ifndef _SPEDYE_H_
#define _SPEDYE_H_

#include "uv.h"

typedef enum {
  SPEDYE_WORKER_THREADED,
  SPEDYE_WORKER_PROCESS,
} spedye_worker_type_e;

typedef struct spedye_conf_t {
  int worker_count;
  spedye_worker_type_e worker_type;
  /* TODO: array of certificates */
  const char *certpath;
  const char *keypath;
} spedye_conf_t;


typedef enum {
  SPEDYE_STARTING,
  SPEDYE_RUNNING,
  SPEDYE_STOPING,
  SPEDYE_STOPPED,
} spedye_state_e;

typedef struct spedye_worker_t {
  spedye_worker_type_e worker_type;
  spedye_state_e state;
  /* TODO: subprocess support */
  uv_thread_t worker_thread;
  uv_async_t worker_wakeup;
  uv_loop_t *loop;
} spedye_worker_t;

typedef struct spedye_master_t {
  uv_thread_t master_thread;
  uv_async_t master_wakeup;
  spedye_state_e state;
  uv_loop_t *loop;
  spedye_conf_t *conf;
  spedye_worker_t **workers;
} spedye_master_t;


void spedye_process_init();
void spedye_process_destroy();

int spedye_master_create(spedye_master_t **m, spedye_conf_t *conf, uv_loop_t *loop);
int spedye_master_run(spedye_master_t *m);
void spedye_master_destroy(spedye_master_t *m);

int spedye_worker_create(spedye_worker_t **w, spedye_master_t *m);
int spedye_worker_run(spedye_worker_t *w);
void spedye_worker_destroy(spedye_worker_t *w);

#endif
