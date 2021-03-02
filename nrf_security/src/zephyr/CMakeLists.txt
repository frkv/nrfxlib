#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Export mbedcrypto include directories to Zephyr interface.
# The function `zephyr_include_directories()` does not support generator
# expression therefore we must handle this directly.
target_include_directories(zephyr_interface
  INTERFACE
    $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_INCLUDE_DIRECTORIES>
)

# Add files required for NS build in case TF-M is used
if(CONFIG_BUILD_WITH_TFM)

  # Add common files from mbed TLS to NS build in case TF-M is used
  list(APPEND src_zephyr
    ${ARM_MBEDTLS_PATH}/library/platform.c
    ${ARM_MBEDTLS_PATH}/library/platform_util.c
    ${ARM_MBEDTLS_PATH}/library/debug.c
    ${ARM_MBEDTLS_PATH}/library/version.c
    ${ARM_MBEDTLS_PATH}/library/version_features.c
    ${ARM_MBEDTLS_PATH}/library/error.c
  )

  # Add missing files for mbedx509 library APIs
  if(TARGET ${MBEDTLS_TARGET_PREFIX}mbedx509 OR
    TARGET ${MBEDTLS_TARGET_PREFIX}mbedtls)
    list(APPEND src_zephyr
      ${ARM_MBEDTLS_PATH}/library/pk.c
      ${ARM_MBEDTLS_PATH}/library/pem.c
      ${ARM_MBEDTLS_PATH}/library/md.c
      ${ARM_MBEDTLS_PATH}/library/oid.c
      ${ARM_MBEDTLS_PATH}/library/asn1write.c
      ${ARM_MBEDTLS_PATH}/library/asn1parse.c
    )
  endif()

  # Add missing files for mbedtls library APIs
  if(TARGET ${MBEDTLS_TARGET_PREFIX}mbedtls)
    list(APPEND src_zephyr
      ${ARM_MBEDTLS_PATH}/library/ecp.c
      ${ARM_MBEDTLS_PATH}/library/bignum.c
      ${ARM_MBEDTLS_PATH}/library/dhm.c
      ${ARM_MBEDTLS_PATH}/library/ecdh.c
      ${ARM_MBEDTLS_PATH}/library/ecjpake.c
      ${ARM_MBEDTLS_PATH}/library/cipher.c
    )
  endif()
endif()

# Add mbed TLS heap library - Not for TF-M
if(CONFIG_MBEDTLS_ENABLE_HEAP)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/mbedtls/mbedtls_heap.c
  )

  # Add replacement for memory_buffer_alloc only for non TF-M builds
  # Use original non TF-M and for NS image when building TF-M
  if((CONFIG_CC3XX_BACKEND AND CONFIG_BUILD_TFM) OR
     (CONFIG_CC3XX_BACKEND AND NOT CONFIG_BUILD_WITH_TFM))
    list(APPEND src_zephyr
      ${NRF_SECURITY_ROOT}/src/mbedtls/replacements/memory_buffer_alloc.c
    )
  else()
    list(APPEND src_zephyr
      ${ARM_MBEDTLS_PATH}/library/memory_buffer_alloc.c
    )
  endif()
endif()

# Add entropy_poll only if NRF_CC3XX_PLATFORM is not added
# This file is not useful for TF-M build where entropy should come
# from SPE image, using psa_generate_random
if(NOT CONFIG_NRF_CC3XX_PLATFORM AND NOT CONFIG_BUILD_WITH_TFM)
    list(APPEND src_zephyr
      ${NRF_SECURITY_ROOT}/src/backend/entropy/entropy_poll.c
    )
endif()

# Add the zephyr-specific sources neccessary for nrf_security
# This library contains files that require knowledge of zephyr
if(DEFINED src_zephyr)
  zephyr_library_named(mbedtls_zephyr)

  zephyr_library_sources(${src_zephyr})

  zephyr_library_include_directories(
    $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_INCLUDE_DIRECTORIES>
  )

  zephyr_library_compile_definitions(
    $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_COMPILE_DEFINITIONS>
  )

  zephyr_library_app_memory(k_mbedtls_partition)
endif()

# Add the generic mbedtls_external library to interface with zephyr
add_library(mbedtls_external INTERFACE)

# Link to the NSPE versions of the libraries providing TLS/DTLS and x509 APIs
if(TARGET mbedtls)
  target_link_libraries(mbedtls_external INTERFACE mbedtls)
endif()

if(TARGET mbedx509)
  target_link_libraries(mbedtls_external INTERFACE mbedx509)
endif()

# mbedcrypto_static_alias will be resolved to mbedcrypto library or tfm_api
target_link_libraries(mbedtls_external INTERFACE ${mbedcrypto_target})
