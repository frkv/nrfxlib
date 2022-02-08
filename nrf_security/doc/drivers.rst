.. _nrf_security_drivers:

nrf_security drivers
####################

.. contents::
   :local:
   :depth: 2

The nrf_security module supports multiple enabled PSA drivers at the same time.
This mechanism is intended to extend the available feature set of hardware-accelerated cryptography or to provide alternative implementations of the PSA Crypto APIs.

Enabling a cryptographic feature or algorithm is done using PSA Crypto API configurations that follows the format ``PSA_WANT_ALG_XXXX``.

Enabling more than one PSA driver may add support for additional key sizes or modes of operation. 

It is possible to disable specific features on PSA driver level to optimize the code size.

The nrf_security module supports the following PSA drivers:

* Arm CryptoCell cc3xx
* nrf_oberon binary library

.. note::
   Note that whenever this documentation mentions 'original' Mbed TLS, it refers to the open-source `Arm Mbed TLS project`_, not the customized version available in Zephyr.
   There is an option to utilize a 'builtin' driver, which corresponds to software implemented cryptography from the 'original' Mbed TLS deliverables. 
   This is provided to ensure that the cryptographic toolbox supports all requested features.

.. _nrf_security_drivers_cc3xx:

Arm CryptoCell cc3xx driver
***************************

The Arm CryptoCell cc3xx driver is a runtime library that provides hardware-accelerated cryptography using the Arm CryptoCell cc310/cc312 hardware.

The Arm CryptoCell cc3xx driver is only available on the following devices:

* nRF52840
* nRF9160
* nRF5340


Enabling the Arm CryptoCell cc3xx driver
========================================

The Arm CryptoCell cc3xx driver can be enabled by setting the :kconfig:`PSA_CRYPTO_DRIVER_CC3XX` Kconfig variable.


Using the Arm CryptoCell cc3xx driver
=====================================

To use the :ref:`nrf_cc3xx_mbedcrypto_readme` PSA driver, the Arm CryptoCell cc310/cc312 hardware must be first initialized.

The Arm CryptoCell cc3xx hardware is initialized in :file:`<NCS>/nrf/drivers/hw_cc310/hw_cc310.c` and is controlled with the :kconfig:`CONFIG_HW_CC3XX` Kconfig variable.
The Kconfig variable has a default value of 'y' when cc3xx is available in the SoC.

.. _nrf_security_driver_oberon:

nrf_oberon driver
*****************

The :ref:`nrf_oberon_readme` is distributed as a closed-source binary that provides select cryptographic algorithms optimized for use in nRF devices.
This provides faster execution than the original Mbed TLS implementation.

The nrf_oberon driver provides support for AES ciphers, SHA-1, SHA-256, SHA-384, SHA-512, ECDH and ECDSA using NIST curve secp224r1 and secp256r1, and ECJPAKE using NIST curve secp256r1.

Enabling the nrf_oberon driver
==============================

To use the :ref:`nrf_oberon_readme` as a backend, set the :kconfig:`PSA_CRYPTO_DRIVER_OBERON` Kconfig variable to true.

.. _nrf_security_backends_orig_mbedtls:

Builtin Mbed TLS
****************

Builtin Mbed TLS uses the open-source software from the `Arm Mbed TLS project`_.

The software is unaltered from the Arm Mbed TLS project source distribution, meaning it is in its `vanilla` form.

You can use the builtin Mbed TLS support to add support for features not available in the `Arm CryptoCell cc3xx backend`_, for example AES-256 or ECC Brainpool curve types.

Similarly, you can use the builtin Mbed TLS to add support for features not available in the `nrf_oberon backend`_, for example to support more ECC curve types.

.. note:
   Certain changes has been made tp the file ``pss_crypto_driver_wrappers`` to provide support for the PSA drivers.


Enabling the builtin Mbed TLS support
=====================================

To enable the builtin Mbed TLS support, set the :kconfig:`PSA_CRYPTO_DRIVER_BUILTIN` Kconfig variable to true.

It is generally not needed to enable builtin Mbed TLS support manually, as there is Kconfig logic that does that, depending on the enabled cryptographic features or algorithms and the configuration of PSA drivers.

Legacy Mbed TLS
***************

Some legacy Mbed TLS APIs are still supported, for instance for TLS and DTLS support and backwards compatibility in Zephyr subsystems.

Enabling legacy APIs requires enabling one of the available backends.

.. note::
   The legacy Mbed TLS APIs no longer support glued functionality and must choose one out of the selected backends
   Legacy configurations has no effect on the configurations for the secure image of a TF-M build.

Enabling legacy Mbed TLS support
================================

The legacy Mbed TLS APIs can be configured by setting the option kconfig:`NORDIC_SECURITY_BACKEND` instead of setting the option kconfig:`NRF_SECURITY`.

Additionally the the options :kconfig:`CC3XX_BACKEND`or :kconfig:`OBERON_BACKEND` must be set, to enable the desired backend support.

.. note::
   Enabling the CryptoCell backend non-secure image of a TF-M build will have no effect.
