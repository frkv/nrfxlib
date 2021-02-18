#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_include_directories(../../include/)

append_with_prefix(src_psa ${ARM_MBEDTLS_PATH}/library/
        psa_crypto_slot_management.c
        psa_crypto_storage.c
)

# The 2.24 mbedtls breaks compilation if MBEDTLS_PK_PARSE_C is not defined
# so we need a replacement with the fix
append_with_prefix(src_psa_replacements
  ${NRF_SECURITY_ROOT}/src/psa/replacements/
  psa_crypto.c
)

if(CONFIG_PSA_EITS_BACKEND_ZEPHYR)
        append_with_prefix(src_psa ./
                psa_eits.c
                psa_eits_zephyr_settings.c)
endif()

zephyr_library_named(mbedcrypto_psa)
zephyr_library_sources(${src_psa} ${src_psa_replacements})
zephyr_library_link_libraries(mbedtls_common)
nrf_security_debug_list_target_files(mbedcrypto_psa)
zephyr_library_app_memory(k_mbedtls_partition)
