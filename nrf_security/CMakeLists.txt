#
# Copyright (c) 2019-2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
set(NRF_SECURITY_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(mbedcrypto_target mbedcrypto)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_security_debug.cmake)

if(CONFIG_BUILD_WITH_TFM)
  nrf_security_debug("TFM is enabled, nRF Security will build as part of TF-M")

  # TF-M is enabled and we are not inside the TF-M build system.
  # Thus, do not build regular `mbedcrypto` library, instead export needed
  # settings to the TF-M build system using the `nrf_security_target`.

  # Add a library to propagate configurations and paths to TF-M
  add_custom_target(nrf_security_target)

  # Pass any settings for nrf security build in TF-M context here in the form of:
  # "key0=value0;key1=value1;..."
  get_mbedtls_dir(ARM_MBEDTLS_PATH)
  set_property(TARGET nrf_security_target
    PROPERTY CMAKE_ARGS
      "ZEPHYR_DOTCONFIG=${DOTCONFIG}"
      "GCC_M_CPU=${GCC_M_CPU}"
      "ARM_MBEDTLS_PATH=${ARM_MBEDTLS_PATH}"
      "ZEPHYR_AUTOCONF=${AUTOCONF_H}"
    )

  # Threading is not used for TF-M NS build
  # Force this to false to disable it.
  set(MBEDTLS_THREADING_ALT False)
  set(MBEDTLS_THREADING_C False)
else()
  nrf_security_debug("Building for Zephyr")

  #
  # Populate ARM_MBEDTLS_PATH
  #
  get_mbedtls_dir(ARM_MBEDTLS_PATH)
endif()

# If nrf_security generates config file, include cmake script that
# adds configurations according to Kconfig and converts them from
# CONFIG_MBEDTLS_XXXX to MBEDTLS_XXXX.
if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/kconfig_mbedtls_configure.cmake)
endif()

# Add nrf_security extension functions.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/extensions.cmake)

#
#  Add nrf_security source folder
#
add_subdirectory(src)
