.. _nrf_security_readme:

Nordic Security Module
######################

The nrf_security module provides an integration between mbed TLS and software
libraries that provide hardware accelerated cryptographic funcitonality on
select Nordic Semiconductor SoCs. This module includes an mbed TLS glue layer to
enable both hardware accelerated and vanilla mbed TLS software implementation
at the same time.

.. note:: The nrf_security module interfaces with the `nrf_cc310_mbedcrypto`_
library. This library conforms to a specific version of mbed TLS.


Prerequisites
=============
The nrf_security module requires mbed TLS. The nrf_security module will check
for the presence of mbed TLS in a few ways:
* Detection of an environmental variable ``ARM_MBEDTLS_PATH``
* If west is present, it will try to locate mbed TLS during build

If west is not installed and mbed TLS isn't located, cmake will report the
following error:

.. code-block:: console

        west not installed, please provide ARM_MBEDTLS_PATH to CMake to support
        security libraries

If west is installed and mbed TLS isn't located, cmake will report the
following error:

.. code-block:: console

        Failed to find mbedtls, cannot build security libraries

Configuration
=============
The configuration of the nrf_security module is done through Kconfig. To enable
the Nordic Security Module, set the :option:`NORDIC_SECURITY_BACKEND` Kconfig
variable in the `Nordic Security` menu.

Setting this variable will allow for additional KConfig variables according
to the number of features requested. These configurations are then used to
generate a mbed TLS config-file used in compilation.

It is possible to provide your own custom mbed TLS configuration file by Setting
the :option:`CUSTOM_MBEDTLS_CFG_FILE` Kconfig variable.

.. note:: Setting the :option:`CUSTOM_MBEDTLS_CFG_FILE` Kconfig variable
is not recommended.


Enabling backends
=================

The nrf_security module support multiple enabled backends at the same time.
This is done to extend the feature set of hardware accelerated cryptography in
case this is limited. Enabling one or more backends will add more configuration
options grouped into classes of cryptographic algorithms.

.. note:: Configuration options added after enabling one or more backends will
change based on the number of enabled backends. Some configuration options allow
adding support from multiple backends by utilizing the mbed TLS glue layer,
while other will provide a selection between the enabled backends (as radio
buttons).

The nrf_security module supports two backends:
* Arm CryptoCell CC310 (in nRF52840 and nRF9160)
* Vanilla mbed TLS

Enabling the ARM CryptoCell CC310 backend
-----------------------------------------

To enable the ARM CryptoCell CC310 backend, set the :option:`CC310_BACKEND`
Kconfig variable to true.

.. note:: This backend is only available in nRF52840 and nRF9160 SoCs.

Enabling the Vanilla mbed TLS backend
-------------------------------------

To enable the vanilla mbed TLS backend, set the :option:`MBEDTLS_VANILLA_BACKEND`
Kconfig variable to true.


Using the nrf_cc310_mbedcrypto as backend
-----------------------------------------

To use the `nrf_cc310_mbedcrypto`_ library as a backend it must be initialized
using standard mbed TLS APIs for platform abstraction

.. code-block:: C
        :caption: Initializign the nrf_cc310_mbedcrypto backend.
        mbedtls_platform_context platform_context = {0};
        int ret;
        ret = mbedtls_platform_set_calloc_free(alloc_fn, free_fn);
        if (ret != 0)
        {
                /** Failed to set the alternative calloc/free */
                return ret;
        }

        ret = mbedtls_platform_setup(&platform_context);
        if (ret != 0)
        {
                /** Failed to initialize nrf_cc310_mbedcrypto platform */
                return ret,
        }


mbed TLS glue layer
===================
The mbed TLS glue layer allows using hardware accelerated and software
implementation of cryptography at the same time. A typical use-case is if the
cryptographic algorithm is limited in hardware support (e.g. the AES key size or
supported ciphers). In this case the mbed TLS glue layer enables a dynamic check
to see if the cryptographic algorithm is supported in hardware. If it isn't
supported, the glue layer will call into a software implementation as a
fallback.


mbed TLS glue layer - Support and priority
------------------------------------------

The mbed TLS glue layer implements a functionality check with priority reporting
to select the backend. This is done through a `check function` that is called
in APIs that provides configuration changes that merits selection between
the enabled backends.

.. code-block:: C
        :caption: CC310 backend AES CCM support and priority check (example)

        static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits)
        {
                return (keybits == 128) ? 2 : 0;
        }

In the former example, the AES CCM support in the backend will report priority
level `2` if the key size is 128, or `0` if the key size is different than that.
The CC310 backend doesn't support a larger key-size, in which case the vanilla
mbed TLS will be used (if the backend is enabled).

.. note:: The check function can be called from multiple APIs in the mbed TLS
glue layer. An example of a location that merits backend selection is an API
to set an encryption key, in which case the keysize may change, demanding
a new selection of available and prioritized backends.

.. note:: Hardware accelerated cryptogrphy through the CC310 backend will be
prioritized if it is supported. Vanilla mbed TLS is only used as a fallback.


Enabling the mbed TLS glue layer
--------------------------------

The mbed TLS glue layer is automatically enabled when two backends are enabled
for one or more of the following groups of algorithms:
* AES (common and CCM)
* CMAC
* DHM

.. note:: Note that the mbed TLS glue layer will be extended in upcoming
versions.


mbed TLS glue layer mechanisms
------------------------------

The mbed TLS glue layer relies on symbol renaming of known APIs in mbed TLS to
prevent collisions of equally named functions in multiple backends. The backend
implementation is reached using a table of function pointers corresponding to
the renamed symbols.

.. code-block:: C
        :caption: CC310 backend ECDH function table (example)

        const mbedtls_ecdh_funcs mbedtls_ecdh_cc310_backend_funcs = {
                .check = mbedtls_ecdh_check,
                .gen_public = mbedtls_ecdh_gen_public,
                .compute_shared = mbedtls_ecdh_compute_shared,
        };

`mbedtls_ecdh-cc310_backend_funcs` point to mbed TLS APIs in `nrf_cc310_mbedcrypto`_
which will be renamed if mbed TLS glue layer is enabled. The function pointers
`gen_public` and `compute_shared` have signatures equal to the corresponding
mbed TLs APIs.


The complete list of APIs that can be renamed in the mbed TLS glue layer can be
found in :file:`nrfxlib/nrf_security/src/mbedcrypto_glue/symbol_rename.template.txt`


nrf_security backend configurations
===================================

The following chapter covers the configurations available when one or more
nrf_security backends are enabled. This includes linking directly to the
backend library or utilizing the mbed TLS glue layer.


AES configurations
------------------

AES cipher mode support is dependent on setting the :option:`AES_CONFIGURATION`
and :option:`MBEDTLS_AES_C` Kconfig variables. Setting these variables enables
core AES support and allows other AES cipher modes like `CTR`, `OFB`, `CFB`,
`CBC`, `XTS` and `CMAC`.

If multiple backends are enabled the individual backends can be enabled by
setting :option:`VANILLA_MBEDTLS_AES_C` and/or :option:`CC310_MBEDTLS_AES_C`.

.. note:: Currently AES cipher modes `OFB`, `CFB` and `XTS` is only supported by
vanilla mbed TLS and are only accessible if :option:`VANILLA_MBEDTLS_AES_C`
Kconfig variable is set.

.. note:: The CC310 backend is limited to key-sizes of 128 bits.

The individual cipher modes can be enabled according to the following table:
+--------------+----------------+-----------------------------------+
| Cipher mode  | Support        | Configurations                    |
+==============+================+===================================+
| ECB          | Glue           | CC310: CC310_MBEDTLS_AES_C        |
|              |                | Vanilla: VANILLA_MBEDTLS_AES_C    |
+--------------+----------------+-----------------------------------+
| CTR          | Glue           | MBEDTLS_CIPHER_MODE_CTR           |
+--------------+----------------+-----------------------------------+
| CBC          | Glue           | MBEDTLS_CIPHER_MODE_CBC           |
+--------------+----------------+-----------------------------------+
| CFB          | Vanilla only   | MBEDTLS_CIPHER_MODE_CFB           |
+--------------+----------------+-----------------------------------+
| OFB          | Vanilla only   | MBEDTLS_CIPHER_MODE_OFB           |
+--------------+----------------+-----------------------------------+
| XTS          | vanilla only   | MBEDTLS_CIPHER_MODE_XTS           |
+--------------+----------------+-----------------------------------+
| CMAC         | Choice         | CC310: CC310_MBEDTLS_CMAC_C or    |
|              |                | Vanilla: VANILLA_MBEDTLS_CMAC_C   |
+--------------+----------------+-----------------------------------+

.. note:: All selections in this table is dependent on setting the
:option:`AES_CONFIGURATION` and :option:`MBEDTLS_AES_C` Kconfig variables.


AEAD configurations
-------------------

The Authenticated Encryption with Associated Data can be enabled according to
the cipher in question. To enable AEAD support, set the
:option:`AEAD_CONFIGURATION` Kconfig variable and set additional configurations
for cipher mode and backend support.

.. note:: The CC310 backend is limited to key-sizes of 128 bits.

The individual cipher modes can be enabled according to the following table:
+--------------+----------------+-----------------------------------+
| AEAD cipher  | Support        | Configurations                    |
+==============+================+===================================+
| CCM          | Glue           | CC310: CC310_MBEDTLS_CCM_C        |
| CCM*         |                | Vanilla: VANILLA_MBEDTLS_CCM_C    |
+--------------+----------------+-----------------------------------+
| GCM          | Vanilla only   | MBEDTLS_GCM_C                     |
+--------------+----------------+-----------------------------------+
| ChaCha-Poly  | Vanilla only   | MBEDTLS_CHACHA20_C                |
+--------------+----------------+-----------------------------------+

.. note:: AES CCM depends on :option:`MBEDTLS_AES_C` being set.

.. note:: AES GCM depends on :option:`VANILLA_MBEDTLS_AES_C` being set.


DHM configurations
------------------
The Diffie-Hellman-Merkel module can be configured by setting the
:option:`MBEDTLS_DHM_C` Kconfig variable. Additional configurations can
be set to enable backend support for DHM operations.

.. note:: The CC310 backend is limited to key-size <= 2048 bits.

The module can be enabled according to the following table:
+--------------+----------------+-----------------------------------+
| Module       | Support        | Configurations                    |
+==============+================+===================================+
| DHM          | Glue           | CC310: CC310_MBEDTLS_DHM_C        |
|              |                | Vanilla: VANILLA_MBEDTLS_DHM_C    |
+--------------+----------------+-----------------------------------+

ECDH configurations
-------------------

The Elliptic Curve Diffie-Hellman support can be configured by setting the
:option:`MBEDTLS_ECDH_C` Kconfig variable. If multiple backends are enabled
in the nrf_security module, additional configurations must be set to select the
backend to use for ECDH operations.

The module can be enabled according to the following table:
+--------------+----------------+-----------------------------------+
| Module       | Support        | Configurations                    |
+==============+================+===================================+
| ECDH         | Choice         | CC310: CC310_MBEDTLS_ECDH_C       |
|              |                | Vanilla: VANILLA_MBEDTLS_ECDH_C   |
+--------------+----------------+-----------------------------------+

.. note:: The `nrf_cc310_mbedcrypto`_ library does not integrate on ECP layer.
only the top-level APIs for ECDH is replaced.


ECDSA configurations
--------------------

The Elliptic Curve Digital Signature Algorithm support can be configured
by setting the :option:`MBEDTLS_ECDSA_C` Kconfig variable. If multiple backends
are enabled in the nrf_security module, additional configurations must be set
to select the backend to use for ECDSA operations.

The module can be enabled according to the following table:
+--------------+----------------+-----------------------------------+
| Module       | Support        | Configurations                    |
+==============+================+===================================+
| ECDSA        | Choice         | CC310: CC310_MBEDTLS_ECDSA_C      |
|              |                | Vanilla: VANILLA_MBEDTLS_ECDSA_C  |
+--------------+----------------+-----------------------------------+


.. note:: The `nrf_cc310_mbedcrypto`_ library does not integrate on ECP layer.
only the top-level APIs for ECDSA is replaced.


RSA configurations
------------------

The Rivest-Shamir-Adleman cryptosystem can be configured by setting the
:option:`MBEDTLS_RSA_C` Kconfig variable. If multiple backends are enabled
in the nrf_security module, additional configurations must be set to select the
backend to use for RSA operations.

.. note:: The CC310 backend is limited to key-sizes <= 2048 bits.

The module can be enabled according to the following table:
+--------------+----------------+-----------------------------------+
| Module       | Support        | Configurations                    |
+==============+================+===================================+
| RSA          | Choice         | CC310: CC310_MBEDTLS_RSA_C        |
|              |                | Vanilla: VANILLA_MBEDTLS_RSA_C    |
+--------------+----------------+-----------------------------------+

Secure hash configurations
--------------------------

The Secure Hash algorithms APIs in mbed TLS will either be using hardware
accelerated implementations or vanilla mbed TLS. To enable support for the
algorithm, set the :option:`HASH_FUNCTIONS_CONFIGURATION`Kconfig variable and
configure the backend support according to the following table:

+--------------+-------------------+-----------------------------------+
| Algorith     | Configuration     | Backend selection                 |
+==============+===================+===================================+
| SHA-1        | MBEDTLS_SHA1_C    | CC310: CC310_MBEDTLS_SHA1_C       |
|              |                   | Vanilla: VANILLA_MBEDTLS_SHA1_C   |
+--------------+-------------------+-----------------------------------+
| SHA-256      | MBEDTLS_SHA256_C  | CC310: CC310_MBEDTLS_SHA256_C     |
|              |                   | Vanilla: VANILLA_MBEDTLS_SHA256_C |
+--------------+-------------------+-----------------------------------+
| SHA-512      | MBEDTLS_SHA512_C  | Vanilla only                      |
+--------------+-------------------+-----------------------------------+

.. note:: Backend selection is not necessary in case only one backend is enabled.

Platform specific configurations
--------------------------------
The Kconfig integration of nrf_security module coupled with CMAKE will set some
default configurations to ensure the backend works as expected. This includes
integration with entropy data sampled from the ARM CC310 HW as well as a
way to abstract the memory management in an RTOS context. Please see the
`nrf_cc310_mbedcrypto`_ library documentation for details.