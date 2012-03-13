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

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef WIN32
#include <pthread.h>
#endif
#include "uv.h"

#include "spedye.h"

static int
process_args(spedye_conf_t *conf, int argc, char *argv[])
{
  int i;
  /* TODO: parse args */
  for (i = 0; i < argc; i++) {
    printf("[%d] =  %s\n", i, argv[i]);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int rv;
  spedye_conf_t conf;
  spedye_master_t *master;
  uv_loop_t *loop;

  memset(&conf, 0, sizeof(spedye_conf_t));

  spedye_process_init();

  rv = process_args(&conf, argc, argv);

  if (rv) {
    return rv;
  }

  loop = uv_default_loop();

  rv = spedye_master_create(&master, &conf, loop);

  if (rv) {
    return rv;
  }

  spedye_master_run(master);

  spedye_master_destroy(master);

  /* TOOD: spawn loops/other threads */
  /* TOOD: Create listeners */
  /* TOOD: handle HTTPS 1:1 -> HTTP */
  /* TODO: handle SPDY (m:n) -> HTTP*/
  spedye_process_destroy();

  return 0;
}
