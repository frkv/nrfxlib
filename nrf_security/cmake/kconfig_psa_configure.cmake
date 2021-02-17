#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
# The purpose of this file is to set specific MBEDTLS config file defines based
# on the Kconfig settings.
# When all values has been processed, the mbedtls config file will be generated.


# Enabling core functionality
#
kconfig_mbedtls_config("MBEDTLS_PSA_CRYPTO_C")
kconfig_mbedtls_config("MBEDTLS_PSA_CRYPTO_STORAGE_C")

kconfig_mbedtls_config_val("MBEDTLS_PLATFORM_STD_MEM_HDR"	"${CONFIG_MBEDTLS_PLATFORM_STD_MEM_HDR}")
kconfig_mbedtls_config_val("MBEDTLS_PLATFORM_STD_CALLOC"	"${CONFIG_MBEDTLS_PLATFORM_STD_CALLOC}")
kconfig_mbedtls_config_val("MBEDTLS_PLATFORM_STD_FREE"	"${CONFIG_MBEDTLS_PLATFORM_STD_FREE}")
kconfig_mbedtls_config_val("MBEDTLS_PLATFORM_STD_EXIT"	"${CONFIG_MBEDTLS_PLATFORM_STD_EXIT}")

