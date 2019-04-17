.. _nrf_cc310_mbedcrypto_readme:

nrf_cc310_mbedcrypto library
############################

The nrf_cc310_mbedcrypto library is software library to interface with the ARM
CryptoCell CC310 hardware accelerator that is available on nRF52840 and nRF9160
SoCs. The library adds hardware support for select cryptographic algorithms.

Integration with mbed TLS
=========================
The nrf_cc310_mbedcrypto library provides low-level integration towards select
versions of mbed TLS deliverables. The APIs expressed in this library implements
the alternative implementation abstraction layer inside mbed TLS for select
modules.

.. note:: It is not recommended to link to this library directly. Please use the
`nrf_security`_ module.


Supported cryptographic algorithms
==================================

The following tabls shows the supported cryptographic algorithms in the ARM
CryptoCell CC310 hardware accelerator in nRF52840 and nRF9160 SoCs, as well as
the current state of support in the nrf_cc310_mbedcrypto library.

..note:: If `no mbed TLS support` is listed in limitations it means that the
hardware supports it but it is not exposed in an API that works with mbed TLS.
If `mbedtls_extra only` is listed in limitations, it means that an API similar
to mbed TLS recent versions is made available with the library.


AES - Advanced Encryption Standard
----------------------------------
+-----------------------+-------------------------------+
+ Cipher                | Limitations                   |
+=======================+===============================+
| CTR                   | 128-bit                       |
+-----------------------+-------------------------------+
| CBC                   | 128-bit                       |
+-----------------------+--------------------------------
| OFB                   | 128-bit, no mbed TLS support  |
+-----------------------+-------------------------------+
| CFB                   | 128-bit, no mbed TLS support  |
+-----------------------+-------------------------------+
| CMAC                  | 128-bit                       |
+-----------------------+-------------------------------+

AEAD - Authenticated Encryption with Associated Data
----------------------------------------------------
+-----------------------+-------------------------------+
+ Cipher                | Limitations                   |
+=======================+===============================+
| CCM/CCM*              | 128-bit                       |
+-----------------------+-------------------------------+
| ChaCha-Poly           | 128-bit, mbedtls_extra only   |
+-----------------------+-------------------------------+

Diffie-Hellman-Merkel
---------------------
Supported for key sizes <= 2048 bits

RSA
---
PKCS#1 v1.5 and v2.1 is supported for signing and encryption including:
* RSASSA-PSS
* RSAES-OEAP

Supported for key sizes <= 2048 bits

Secure Hash
-----------
SHA-1 and SHA-256 is supported

ECDSA and ECDH
--------------
ECDSA and ECDH is supported for the following curve types:

* secp160r1
* secp192r1
* secp224r1
* secp256r1
* secp384r1
* secp521r1
* secp160k1
* secp192k1
* secp224k1
* secp256k1
* Ed25519
* Curve25519

Additional items in mbedtls_extra
---------------------------------
The following is a list of features available in mbedtls_extra as non-Standard
mbed TLS APIs:
* AES key wrap functions
* ChaCha20 and Poly1305
* ECIES
* HKDF
* SRP, up to 3072 bits


Using the library
=================

Providing platform specific calloc/free
---------------------------------------

The following API must be used to change the default calloc/free function:

.. code-block:: C
        :caption: Setting custom calloc/free
        int ret;
        ret = mbedtls_platform_set_calloc_free(alloc_fn, free_fn);
        if (ret != 0)
        {
                /** Failed to set the alternative calloc/free */
                return ret;
        }

.. note:: This API should be called prior to calling `mbedtls_platform_Setup`.

.. note:: The library will default to use clib calloc/free functions if the
mbedt_platform_set_calloc is not used.


Initializing the library
------------------------
The library requires initialization before use. This is done by calling the
`mbedtls_platform_setup`/`mbedtls_platform_teardown`-functions.

.. code-block:: C
        :caption: Initializing the library
        int ret;
        mbedtls_platform_context platform_context = {0};
        ret = mbedtls_platform_setup(&platform_context);
        if (ret != 0)
        {
                /** Failed to initialize nrf_cc310_mbedcrypto platform */
                return ret,
        }



.. note:: There is no need to enable/disable the CC310 hardware by writing to
the `NRF_CRYPTOCELL->ENABLE` register. This happens automatically when
calling APIs in this library*

.. note:: There is a mutex-protected single-user restriction on APIs in this
library.

API documentation
#################

nrf_cc310_mbedcrypto library
****************************
.. doxygengroup:: nrf_cc310_mbedcrypto
   :project: nrfxlib
   :members: