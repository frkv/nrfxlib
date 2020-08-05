/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @addtogroup mbedtls_timing
 * @{
 */
#ifndef MBEDTLS_TIMING_ALT_H
#define MBEDTLS_TIMING_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_TIMING_ALT)

#include <zephyr/types.h>

/**@brief Timer structure
 */
struct mbedtls_timing_hr_time
{
	uint32_t timer_val;
};

/**@brief Context for mbedtls_timing_set/get_delay()
 */
struct mbedtls_timing
{
	struct mbedtls_timing_hr_time timer;
	uint32_t int_ms;
	uint32_t fin_ms;
};

typedef struct mbedtls_timing mbedtls_timing;

#endif /* MBEDTLS_TIMING_ALT */

#endif /* MBEDTLS_TIMING_ALT_H */

/** @} */
