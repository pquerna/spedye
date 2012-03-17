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

typedef struct spedye_conf_t {
  const char *certpath;
  const char *keypath;
} spedye_conf_t;

typedef enum {
  SPEDYE_STARTING,
  SPEDYE_RUNNING,
  SPEDYE_STOPING,
  SPEDYE_STOPPED,
} spedye_master_state;

typedef struct spedye_master_t {
  uv_thread_t master_thread;
  uv_async_t master_wakeup;
  spedye_master_state state;
  uv_loop_t *loop;
} spedye_master_t;


void spedye_process_init();
void spedye_process_destroy();

int spedye_master_create(spedye_master_t **m, spedye_conf_t *conf, uv_loop_t *loop);
int spedye_master_run(spedye_master_t *m);
void spedye_master_destroy(spedye_master_t *m);

#endif
