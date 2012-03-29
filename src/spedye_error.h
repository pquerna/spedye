/**
 *  Copyright 2007-2010 Paul Querna.
 *  Copyright 2006 Garrett Rooney.
 *
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

/**
 * Based off of Selene's errors, which are based off of 
 * ETL's error types (which are based off of Subversion's)
 */

/**
 * @file spedye_error.h
 */

#include <stdint.h>

#ifndef _spedye_error_h_
#define _spedye_error_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Check if the @c spedye_error_t returned by @a expression is equal to
 * @c SPEDYE_SUCCESS.  If it is, do nothing, if not, then return it.
 */
#define SPEDYE_ERR(expression) do {                      \
          spedye_error_t *spedye__xx__err = (expression);  \
          if (spedye__xx__err)                           \
            return spedye__xx__err;                      \
        } while (0)

/** A low level error code. */
typedef int spedye_status_t;

/** Successful return value for a function that returns @c spedye_error_t. */
#define SPEDYE_SUCCESS NULL

/** The available buffer space was exhausted. */
#define SPEDYE_ENOSPACE -1

/** The input was invalid. */
#define SPEDYE_EINVAL   -2

/** The requested functionality has not been implemented. */
#define SPEDYE_ENOTIMPL -3

/** The I/O operation in question failed. */
#define SPEDYE_EIO      -4

/* Unable to allocate memory */
#define SPEDYE_ENOMEM -5

/** An exception object. */
typedef struct {
  /** The underlying status code. */
  spedye_status_t err;

  /** A human readable error message. */
  const char *msg;

  /** The line on which the error occurred. */
  uint32_t line;

  /** The file in which the error occurred. */
  const char *file;
} spedye_error_t;

/**
 * Return a new @c spedye_error_t with underlying @c spedye_status_t @a err
 * and message @a msg.
 */
#define spedye_error_create(err, msg) spedye_error_create_impl(err,    \
                                                           msg,        \
                                                           __LINE__,   \
                                                           __FILE__)

/**
 * The underlying function that implements @c spedye_error_t_error_create.
 *
 * This is an implementation detail, and should not be directly called
 * by users.
 */
spedye_error_t *
spedye_error_create_impl(spedye_status_t err, const char *msg,
                          uint32_t line,
                          const char *file);

/**
 * Return a new @c spedye_error_t with underlying @c spedye_status_t @a err
 * and message created @c printf style with @a fmt and varargs.
 */
#define spedye_error_createf(err, fmt, ...) spedye_error_createf_impl(err,         \
                                                                  __LINE__,    \
                                                                  __FILE__,    \
                                                                  fmt,         \
                                                                  __VA_ARGS__)

/**
 * The underlying function that implements @c spedye_error_createf.
 *
 * This is an implementation detail, and should not be directly called
 * by users.
 */
spedye_error_t *
spedye_error_createf_impl(spedye_status_t err,
                         uint32_t line,
                         const char *file,
                         const char *fmt,
                         ...);

/** Destroy @a err. */
void
spedye_error_clear(spedye_error_t *err);

/** Duplicates an error object */
spedye_error_t *
spedye_error_dup(spedye_error_t *err);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
