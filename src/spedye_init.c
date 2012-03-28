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

#include <assert.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#if !USE_SYSTEM_SSL
#if OPENSSL_VERSION_NUMBER != SPEDYE_OPENSSL_VERSION_NUMBER
#error Invalid OpenSSL version number. Busted Include Paths?
#endif
#endif

static uv_rwlock_t* locks;

static unsigned long
crypto_id_cb(void)
{
#ifdef _WIN32
  return (unsigned long) GetCurrentThreadId();
#else /* !_WIN32 */
  return (unsigned long) pthread_self();
#endif /* !_WIN32 */
}


static void
crypto_lock_init(void)
{
  int i, n;

  n = CRYPTO_num_locks();
  locks = malloc(sizeof(uv_rwlock_t) * n);

  for (i = 0; i < n; i++) {
    if (uv_rwlock_init(locks + i))
      abort();
  }
}


static void
crypto_lock_cb(int mode, int n, const char* file, int line)
{
  assert((mode & CRYPTO_LOCK) || (mode & CRYPTO_UNLOCK));
  assert((mode & CRYPTO_READ) || (mode & CRYPTO_WRITE));

  if (mode & CRYPTO_LOCK) {
    if (mode & CRYPTO_READ) {
      uv_rwlock_rdlock(locks + n);
    }
    else {
      uv_rwlock_wrlock(locks + n);
    }
  }
  else {
    if (mode & CRYPTO_READ) {
      uv_rwlock_rdunlock(locks + n);
    }
    else {
      uv_rwlock_wrunlock(locks + n);
    }
  }
}


static int have_init_spedye = 0;

void
spedye_process_init()
{
  int rc;

  /* TODO: use atomics */
  have_init_spedye++;

  if (have_init_spedye == 1) {
#if !defined(OPENSSL_NO_COMP)
    STACK_OF(SSL_COMP)* comp_methods;
#endif

    rc = ares_library_init(ARES_LIB_INIT_ALL);
    assert(rc == ARES_SUCCESS);


    CRYPTO_malloc_init();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_digests();

    crypto_lock_init();
    CRYPTO_set_locking_callback(crypto_lock_cb);
    CRYPTO_set_id_callback(crypto_id_cb);

    /* Turn off compression. Saves memory, and SPDY has stream level compression. */
#if !defined(OPENSSL_NO_COMP)
#if OPENSSL_VERSION_NUMBER < 0x00908000L
    comp_methods = SSL_COMP_get_compression_method()
#else
    comp_methods = SSL_COMP_get_compression_methods();
#endif
    sk_SSL_COMP_zero(comp_methods);
    assert(sk_SSL_COMP_num(comp_methods) == 0);
#endif
  }
}

void
spedye_process_destroy()
{
  have_init_spedye--;

  if (have_init_spedye == 0) {
    CRYPTO_set_locking_callback(NULL);
    CRYPTO_set_id_callback(NULL);
    CRYPTO_set_dynlock_create_callback(NULL);
    CRYPTO_set_dynlock_lock_callback(NULL);
    CRYPTO_set_dynlock_destroy_callback(NULL);

    CONF_modules_unload(1);
    EVP_cleanup();
#if HAVE_ENGINE_LOAD_BUILTIN_ENGINES
    ENGINE_cleanup();
#endif
    ERR_remove_state(0);
  }
}

int
spedye_init(uv_loop_t *loop)
{
  loop = uv_default_loop();
  return 0;
}
