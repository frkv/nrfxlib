.. _nrf_security_config:

Configuration
#############

.. contents::
   :local:
   :depth: 2

The Nordic Security Module can be enabled using `PSA driver support` or with `Legacy crypto support`.

PSA driver support
******************

The Nordic Security Module with PSA driver support can be enabled by setting :kconfig:`CONFIG_NRF_SECURITY`, and additional configurations according to :ref:`nrf_security_driver_config`.

Enabling PSA driver support expects PSA Crypto APIs to be used.

Legacy crypto support
*********************

Legacy crypto mode can be enabled by setting the Kconfig variable :kconfig:CONFIG_NORDIC_SECURITY_BACKEND`, and additional configurations according to :ref:`nrf_security_legacy_config`.

Enabling legacy crypto support allows backwards compatibility for software that require usage of Mbed TLS crypto toolbox functions prefixed with `mbedtls_`.

Custom Mbed TLS configuration files
***********************************

The Kconfig options for the Nordic Security Module (nrf_security) is used to generate an Mbed TLS configuration files used during compilation.

Although not recommended, it is possible to provide a custom Mbed TLS configuration file by disabling :kconfig:`CONFIG_GENERATE_MBEDTLS_CFG_FILE`.
See :ref:`nrf_security_tls_header`.

Building with TF-M
******************

If :kconfig:`CONFIG_BUILD_WITH_TFM` is enabled together with :kconfig:`CONFIG_NRF_SECURITY`, the TF-M secure image will enable the use of the hardware acceleration of Arm CryptoCell.
In this case, the Kconfig configurations in the Nordic Security Module control the features enabled in TF-M.
