#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Internal function to append all unnamed parameters with 'prefix'
# and add it to the list 'var'
#
function(append_with_prefix var prefix)
  set(listVar ${${var}})
  foreach(f ${ARGN})
    nrf_security_debug("Adding: ${f} to ${var}")
    list(APPEND listVar "${prefix}${f}")
  endforeach(f)
  set(${var} ${listVar} PARENT_SCOPE)
endfunction(append_with_prefix)

#
# Internal macro to append all unnamed parameters with 'prefix' if condition
# is met
#
macro(append_with_prefix_ifdef var cond prefix)
  if (cond)
    append_with_prefix(prefix ${ARGN})
  endif()
endmacro()

nrf_security_debug("Creating list of files for base mbed TLS lib")

#
# Files only in the base library (not glued, unmodified from vanilla mbed TLS)
#
append_with_prefix(src_crypto ${ARM_MBEDTLS_PATH}/library/
    aesni.c
    arc4.c
    aria.c
    asn1parse.c
    asn1write.c
    base64.c
    bignum.c
    blowfish.c
    camellia.c
    cipher.c
    cipher_wrap.c
    ctr_drbg.c
    des.c
    debug.c
    entropy.c
    entropy_poll.c
    error.c
    gcm.c
    havege.c
    hkdf.c
    hmac_drbg.c
    md.c
    md2.c
    md4.c
    md5.c
    nist_kw.c
    oid.c
    padlock.c
    pk.c
    pk_wrap.c
    pkcs12.c
    pkcs5.c
    pkparse.c
    pkwrite.c
    platform.c
    platform_util.c
    psa_crypto_slot_management.c
    psa_crypto_storage.c
    ripemd160.c
    timing.c
    version.c
    version_features.c
    xtea.c
)

# The TF-M build needs a standard version of memory_buffer_alloc.c
# This currently doesn't do a mutex on heap access according to cc3xx rtos
# methods...
if(BUILD_TFM)
  list(APPEND src_crypto ${ARM_MBEDTLS_PATH}/library/memory_buffer_alloc.c)
endif()


nrf_security_debug("Creating list of files for x509 lib")
#
# x509 files
#
append_with_prefix(src_x509 ${ARM_MBEDTLS_PATH}/library/
    certs.c
    pkcs11.c
    x509.c
    x509_create.c
    x509_crl.c
    x509_crt.c
    x509_csr.c
)

if(NOT CONFIG_BUILD_WITH_TFM)
  list(APPEND src_x509
    ${ARM_MBEDTLS_PATH}/library/x509write_csr.c
  )
endif()

nrf_security_debug("Creating list of files for TLS lib")
#
# TLS files
#
append_with_prefix(src_tls ${ARM_MBEDTLS_PATH}/library/
    net_sockets.c
    ssl_cache.c
    ssl_cli.c
    ssl_cookie.c
    ssl_srv.c
    ssl_ticket.c
    ssl_tls.c
    ssl_msg.c
)

# ----------  Replacement sources - modified mbedTLS sources  -------------<

nrf_security_debug("Adding replacement files (modified mbed TLS)")

append_with_prefix(src_crypto_replacement
  ${NRF_SECURITY_ROOT}/src/mbedtls/replacements/
  pem.c
  psa_crypto.c
)

append_with_prefix(src_tls_replacement
  ${NRF_SECURITY_ROOT}/src/mbedtls/replacements/
  ssl_ciphersuites.c
)

if(NOT CONFIG_BUILD_WITH_TFM)
  #
  # Creating prerequisites for mbedcrypto
  #
  # This configures:
  # - mbedcrypto_xxxx backends (is enabled)
  # - mbedcrypto_base_vanilla - All files not in any backend
  #

  #
  # Create mbedcrypto_shared (noglue) library
  #
  nrf_security_debug("-------- Adding src/mbedtls/shared")
  add_subdirectory(shared)

  #
  # Create mbedcrypto_cc3xx (noglue) library
  #
  if (CONFIG_CC3XX_BACKEND)
    nrf_security_debug("-------- Adding src/mbedtls/cc310")
    add_subdirectory(cc310)
  endif()

  #
  # Create mbedcrypto_oberon (noglue) library
  #
  if (CONFIG_OBERON_BACKEND)
    nrf_security_debug("-------- Adding src/mbedtls/oberon")
    add_subdirectory(oberon)
  endif()

  #
  # Create mbedcrypto_vanilla (noglue) library
  #
  if (CONFIG_MBEDTLS_VANILLA_BACKEND)
    nrf_security_debug("-------- Adding src/mbedtls/vanilla")
    add_subdirectory(vanilla)
  endif()

  #
  # Add mbedcrypto_base_vanilla for everything that is not in a backend
  #
  nrf_security_library(BASE mbedcrypto_base_vanilla
    FILES
      ${src_crypto}
      ${src_crypto_replacement}
    INCLUDES
      $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_INCLUDE_DIRECTORIES>
    DEFINES
      $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_COMPILE_DEFINITIONS>
    LINK_LIBRARIES
      $<TARGET_NAME_IF_EXISTS:mbedcrypto_shared>
      $<GENEX_EVAL:$<TARGET_PROPERTY:${mbedcrypto_target},PRIVATE_LINK_LIBRARIES>>
  )
endif()

#
# Library for x.509
#
if (CONFIG_MBEDTLS_X509_LIBRARY)
  add_library(mbedx509 STATIC
    ${src_x509}
  )

  # Add options from Zephyr build
  nrf_security_add_zephyr_options(mbedx509)

  target_link_libraries(mbedx509 PRIVATE ${mbedcrypto_target})

endif()

#
# Library for TLS support
#
if (CONFIG_MBEDTLS_TLS_LIBRARY)
  add_library(mbedtls STATIC
    ${src_tls}
    ${src_tls_replacement}
  )

  # Add options from Zephyr build
  nrf_security_add_zephyr_options(mbedtls)

  target_link_libraries(mbedtls PRIVATE ${mbedcrypto_target})

endif()
