/*
 *  Portable interface to the CPU cycle counter
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printk
#endif

#if defined(MBEDTLS_TIMING_C)

#include "mbedtls/timing.h"

#if defined(MBEDTLS_TIMING_ALT)

#include <zephyr.h>
#include <zephyr/types.h>
#include <time.h>

unsigned long mbedtls_timing_hardclock(void)
{
	return k_cycle_get_32();
}

unsigned long mbedtls_timing_get_timer(struct mbedtls_timing_hr_time *val,
				       int reset)
{
	uint32_t t = k_uptime_get_32();

	if (reset) {
		val->timer_val = t;
		return 0;
	}

	return t - val->timer_val;
}

void mbedtls_set_alarm( int seconds )
{
	;
}

/* Set delays to watch */
void mbedtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms)
{
	mbedtls_timing *ctx = (mbedtls_timing *)data;

	ctx->int_ms = int_ms;
	ctx->fin_ms = fin_ms;

	if (fin_ms != 0) {
		(void)mbedtls_timing_get_timer(&ctx->timer, 1);
	}
}

/* Get number of delays expired */
int mbedtls_timing_get_delay(void *data)
{
	mbedtls_timing *ctx = (mbedtls_timing *)data;
	uint32_t elapsed_ms;

	if (ctx->fin_ms == 0) {
		return -1;
	}

	elapsed_ms = mbedtls_timing_get_timer(&ctx->timer, 0);

	if (elapsed_ms >= ctx->fin_ms) {
		return 2;
	}

	if (elapsed_ms >= ctx->int_ms) {
		return 1;
	}

	return 0;
}

#endif /* MBEDTLS_TIMING_ALT */

#if defined(MBEDTLS_SELF_TEST)
/*
 *@brief Checkup routine
 *
 * Warning: The timing test in the original is "work in progress".
 *          Thus it has been removed and is replaced with a non-error
 *          return code.
 */
int mbedtls_timing_self_test(int verbose)
{
	return 0;
}

#endif /* MBEDTLS_SELF_TEST */

#endif /* MBEDTLS_TIMING_C */
