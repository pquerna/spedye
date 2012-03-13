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

#ifndef WIN32
#include <pthread.h>
#endif
#include "uv.h"

#include "spedye.h"

int main(int argc, char *argv[])
{
  uv_loop_t *loop;

  loop = uv_default_loop();

  spedye_process_init();
  /* TODO: parse args */
  /* TOOD: spawn loops/other threads */
  /* TOOD: Create listeners */
  /* TOOD: handle HTTPS 1:1 -> HTTP */
  /* TODO: handle SPDY (m:n) -> HTTP*/
  spedye_process_destroy();

  return 0;
}
