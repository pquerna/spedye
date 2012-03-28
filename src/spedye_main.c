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

static int long_arg(const char *sname, const char *lname, int i, int argc, char *argv[], char **arg){
  *arg = NULL;

  if (strcmp(sname, argv[i]) == 0 ||
      strcmp(lname, argv[i]) == 0)
  {
    if (i + 1 < argc) {
      *arg =  argv[i+1];
      return 1;
    }
  }

  return 0;
}

static int
process_args(spedye_conf_t *conf, int argc, char *argv[])
{
  char *arg = NULL;
  char *p = NULL;
  int i;

  /* TODO: support configuration file for complex configurations */

  for (i = 0; i < argc; i++) {
    if (long_arg("-b", "--bind", i, argc, argv, &arg)) {
      i++;

      p = strrchr(arg, ':');
      if (p) {
        conf->listeners->port = atoi(p+1);
        *p = '\0';
      }

      if (strcmp("*", arg) == 0) {
        conf->listeners->address = strdup("0.0.0.0");
      }
      else {
        conf->listeners->address = strdup(arg);
      }
      /* TODO: proper parse IP:port, handle v6 */
      continue;
    }

    if (long_arg("-f", "--forward", i, argc, argv, &arg)) {
      i++;

      p = strrchr(arg, ':');

      if (p) {
        conf->listeners->vhosts->destport = atoi(p+1);
        *p = '\0';
      }

      conf->listeners->vhosts->destaddress = strdup(arg);
    }

  }
  conf->worker_count = 10;
  return 0;
}

int main(int argc, char *argv[])
{
  int rv;
  spedye_conf_t *conf;
  spedye_master_t *master;
  uv_loop_t *loop;

  spedye_process_init();

  rv = spedye_conf_create(&conf);

  rv = process_args(conf, argc, argv);

  if (rv) {
    return rv;
  }

  loop = uv_default_loop();

  rv = spedye_master_create(&master, conf, loop);

  if (rv) {
    return rv;
  }

  spedye_master_run(master);

  spedye_master_destroy(master);
  /* TOOD: spawn loops/other threads */
  /* TOOD: Create listeners */
  /* TOOD: handle HTTPS 1:1 -> HTTP */
  /* TODO: handle SPDY (m:n) -> HTTP*/

  spedye_conf_destroy(conf);

  spedye_process_destroy();

  return 0;
}
