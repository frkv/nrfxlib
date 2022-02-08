.. _nrf_security_driver_config:

Driver configurations and supported features
############################################

.. contents::
   :local:
   :depth: 2

This section covers the configurations available when using PSA drivers.

.. _nrf_security_drivers_config_multiple:

Configuring multiple drivers
****************************

Multiple PSA drivers can be enabled at the same time, with added support for fine-grained control of which drivers implements support for cryptographic features.

To enable a PSA driver, set the configurations according to the following table:

+---------------+------------------------------------+------------------------------------------------+
| PSA driver    | Configurations                     | Notes                                          |
+================+===================================+================================================+
| nrf_cc3xx     | :kconfig:`PSA_CRYPTO_DRIVER_CC3XX` | Only on nRF52840, nRF9160, and nRF5340 devices |
+---------------+------------------------------------+------------------------------------------------+
| nrf_oberon    | :kconfig:`PSA_RYPTO_DRIVER_OBERON` |                                                |
+---------------+------------------------------------+------------------------------------------------+

If multiple drivers are enabled, the first ordered item in this table takes precedence for an enabled cryptographic feature, unless the driver does not enable or support it.

Enabling or disabling PSA driver specific configurations controls the support for a given algorithm, per driver.

If a specific cryptographic feature is not supported by a PSA driver but the algorithm is configured to be used, then builtin Mbed TLS will be enabled to ensure the feature is available.


AES cipher configuration
************************

AES cipher modes can be enabled by setting one or more of the following Kconfig variables:

+----------------+-----------------------------------------------+
| Cipher mode    | Configurations                                |
+================+===============================================+
| ECB_NO_PADDING | :kconfig:`CONFIG_PSA_WANT_ALG_ECB_NO_PADDING` |
+----------------+-----------------------------------------------+
| CBC_NO_PADDING | :kconfig:`CONFIG_PSA_WANT_ALG_CTR`            |
+----------------+-----------------------------------------------+
| CBC_PKCS7      | :kconfig:`CONFIG_PSA_WANT_ALG_CBC_PKCS7`      |
+----------------+-----------------------------------------------+
| CFB            | :kconfig:`CONFIG_PSA_WANT_ALG_CFB`            |
+----------------+-----------------------------------------------+
| CTR            | :kconfig:`CONFIG_PSA_WANT_ALG_CTR`            |
+----------------+-----------------------------------------------+
| OFB            | :kconfig:`CONFIG_PSA_WANT_ALG_OFB`            |
+----------------+-----------------------------------------------+
| XTS            | :kconfig:`CONFIG_PSA_WANT_ALG_XTS`            |
+----------------+-----------------------------------------------+


AES cipher driver configuration
===============================

The Kconfig variables in the following table can be used for fine-grained control over which drivers provides AES cipher support:

+----------------+-------------------------------------------------------+--------------------------------------------------------+
| Cipher mode    | nrf_cc3xx driver support                              | nrf_oberon driver support                              |
+================+=======================================================+========================================================+
| ECB_NO_PADDING | :kconfig:`PSA_CRYPTO_DRIVER_ALG_ECB_NO_PADDING_CC3XX` | :kconfig:`PSA_CRYPTO_DRIVER_ALG_ECB_NO_PADDING_CC3XX`  |
+----------------+-------------------------------------------------------+--------------------------------------------------------+
| CBC_NO_PADDING | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CBC_NO_PADDING_CC3XX` | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CBC_NO_PADDING_OBERON` |
+----------------+-------------------------------------------------------+--------------------------------------------------------+
| CBC_PKCS7      | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CBC_PKCS7_CC3XX`      | Not supported                                          |
+----------------+-------------------------------------------------------+--------------------------------------------------------+
| CFB            | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CFB_CC3XX`            | Not supported                                          |
+----------------+-------------------------------------------------------+--------------------------------------------------------+
| CTR            | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CTR_CC3XX`            | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CTR_OBERON`            |
+----------------+-------------------------------------------------------+--------------------------------------------------------+
| OFB            | Not supported                                         | Not supported                                          |
+----------------+-------------------------------------------------------+--------------------------------------------------------+
| XTS            | Not supported                                         | Not supported                                          |
+----------------+-------------------------------------------------------+--------------------------------------------------------+

.. note::
   * If an AES cipher mode is enabled and no PSA driver enables or supports it, then builtin Mbed TLS support is enabled and used.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to AES key sizes of 128 bits on devices with Arm CryptoCell cc310.


MAC configuration
*****************

MAC support can be enabled by setting one or more Kconfig variables in the following table:

+----------------+------------------------------+
| MAC cipher     | Configurations               |
+================+==============================+
| ECB_NO_PADDING | :kconfig:`PSA_WANT_ALG_CMAC` |
+----------------+-------------------------------
| CBC_NO_PADDING | :kconfig:`PSA_WANT_ALG_HMAC` |
+----------------+-------------------------------


MAC driver configurations
=========================

The Kconfig variables in the following table can be used for fine-grained control over which drivers provides AEAD support:

+----------------+---------------------------------------------+----------------------------+
| MAC cipher     | nrf_cc3xx driver support                    | nrf_oberon driver support  |
+================+=============================================+============================+
| ECB_NO_PADDING | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CMAC_CC3XX` | Not supported              |
+----------------+----------------+----------------------------+----------------------------+
| CBC_NO_PADDING | :kconfig:`PSA_CRYPTO_DRIVER_ALG_HMAC_CC3XX` | Not supported              |
+----------------+----------------+----------------------------+----------------------------+

.. note::
   * If a MAC algorithm is enabled and no PSA driver enables or supports it, then builtin Mbed TLS support is enabled and used.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to AES CMAC key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to HMAC using SHA-1, SHA-224, and SHA-256 on devices with Arm CryptoCell cc310.


AEAD configurations
*******************

Authenticated Encryption with Associated Data (AEAD) can be enabled by setting one or more Kconfig variables in the following table:

+----------------+-------------------------------------------+
| AEAD cipher    | Configurations                            |
+================+===========================================+
| AES CCM        | :kconfig:`PSA_WANT_ALG_CCM`               |
+----------------+-------------------------------------------+
| AES GCM        | :kconfig:`PSA_WANT_ALG_GCM`               |
+----------------+-------------------------------------------+
| ChaCha/Poly    | :kconfig:`PSA_WANT_ALG_CHACHA20_POLY1305` |
+----------------+-------------------------------------------+


AEAD driver configurations
==========================

The Kconfig variables in the following table can be used for fine-grained control over which drivers provides AEAD support:

+----------------+----------------------------------------------------------+-----------------------------------------------------------+
| AEAD cipher    | nrf_cc3xx driver support                                 | nrf_oberon driver support                                 |
+================+==========================================================+===========================================================+
| AES CCM        | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CCM_CC3XX`               | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CCM_OBERON`               |
+----------------+----------------------------------------------------------+-----------------------------------------------------------+
| AES GCM        | :kconfig:`PSA_CRYPTO_DRIVER_ALG_GCM_CC3XX`               | Not Supported                                             |
+----------------+----------------------------------------------------------+-------------------------------------------------------- --+
| ChaCha/Poly    | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CHACHA20_POLY1305_CC3XX` | :kconfig:`PSA_CRYPTO_DRIVER_ALG_CHACHA20_POLY1305_OBERON` |
+----------------+----------------------------------------------------------+-----------------------------------------------------------+

.. note:
   * If an AEAD algorithm is enabled and no PSA driver enables or supports it, then builtin Mbed TLS support is enabled and used.
   * The :ref:`nrf_security_drivers_cc3xx` is limited to AES key sizes of 128 bits on devices with Arm CryptoCell cc310.
   * The :ref:`nrf_security_drivers_cc3xx` does not provide hardware support for AES GCM on devices with Arm CryptoCell cc310.


ECC configurations
******************

Elliptic Curve Cryptography (ECC) can be enabled by setting one or more Kconfig variables in the following table:

+-----------------------+---------------------------------------------+
| ECC algorithm         | Configurations                              |
+=======================+=============================================+
| ECDH                  | :kconfig:`PSA_WANT_ALG_ECDH`                |
+----------------+----------------------------------------------------+
| ECDSA                 | :kconfig:`PSA_WANT_ALG_ECDSA`               |
+-----------------------+---------------------------------------------+
| ECDSA (deterministic) | :kconfig:`PSA_WANT_ALG_DETERMINISTIC_ECDSA` |
+-----------------------+---------------------------------------------+

The ECC algorithm support is dependent on one or more Kconfig variables enabling curve support according to :ref:`ECC curve configurations`.


ECC driver configurations
=========================

The Kconfig variables in the following table can be used for fine-grained control over which drivers provides ECC support:

+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| ECC algorithm         | nrf_cc3xx driver support                                   | nrf_oberon driver support                                   |
+=======================+============================================================+=============================================================+
| ECDH                  | :kconfig:`PSA_CRYPTO_DRIVER_ALG_ECDH_CC3XX`                | :kconfig:`PSA_CRYPTO_DRIVER_ALG_ECDSA`                      |
+----------------+-------------------------------------------------------------------+-------------------------------------------------------------+
| ECDSA                 | :kconfig:`PSA_CRYPTO_DRIVER_ALG_ECDSA_CC3XX`               | :kconfig:`PSA_CRYPTO_DRIVER_ALG_ECDSA_OBERON`               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| ECDSA (deterministic) | :kconfig:`PSA_CRYPTO_DRIVER_ALG_DETERMINISTIC_ECDSA_CC3XX` | :kconfig:`PSA_CRYPTO_DRIVER_ALG_DETERMINISTIC_ECDSA_OBERON` |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+

.. note::
   * If an ECC algorithm is enabled and no PSA driver enables or supports it, then builtin Mbed TLS support is enabled and used.
   * The :ref:`nrf_security_drivers_oberon` is currently limited to curve types secp224r1 and secp256r1 for ECDH and ECDSA.


ECC curve configurations
************************

Elliptic curve support can be configured by setting one or more Kconfig variables in the following table:

+-----------------------+---------------------------------------------+
| ECC curve type        | Configurations                              |
+=======================+=============================================+
| Brainpool256r1        | :kconfig:`PSA_WANT_ECC_BRAINPOOL_P_R1_256`  |
+-----------------------+---------------------------------------------+
| Brainpool384r1        | :kconfig:`PSA_WANT_ECC_BRAINPOOL_P_R1_384`  |
+-----------------------+---------------------------------------------+
| Brainpool512r1        | :kconfig:`PSA_WANT_ECC_BRAINPOOL_P_R1_512`  |
+-----------------------+---------------------------------------------+
| Curve25519            | :kconfig:`PSA_WANT_ECC_MONTGOMERY_255`      |
+-----------------------+---------------------------------------------+
| Curve448              | :kconfig:`PSA_WANT_ECC_MONTGOMERY_448`      |
+-----------------------+---------------------------------------------+
| secp192k1             | :kconfig:`PSA_WANT_ECC_SECP_K1_192`         |
+-----------------------+---------------------------------------------+
| secp256k1             | :kconfig:`PSA_WANT_ECC_SECP_K1_256`         |
+-----------------------+---------------------------------------------+
| secp192r1             | :kconfig:`PSA_WANT_ECC_SECP_R1_192`         |
+-----------------------+---------------------------------------------+
| secp224r1             | :kconfig:`PSA_WANT_ECC_SECP_R1_224`         |
+-----------------------+---------------------------------------------+
| secp256r1             | :kconfig:`PSA_WANT_ECC_SECP_R1_256`         |
+-----------------------+---------------------------------------------+
| secp384r1             | :kconfig:`PSA_WANT_ECC_SECP_R1_384`         |
+-----------------------+---------------------------------------------+
| secp521r1             | :kconfig:`PSA_WANT_ECC_SECP_R1_521`         |
+-----------------------+---------------------------------------------+


ECC curve driver configurations
===============================

The Kconfig variables in the following table can be used for fine-grained control over which drivers provides elliptic curve support:

+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| ECC curve type        | nrf_cc3xx driver support                                   | nrf_oberon driver support                                   |
+=======================+============================================================+=============================================================+
| Brainpool256r1        | :kconfig:`PSA_CRYPTO_DRIVER_ECC_BRAINPOOL_P_R1_256_CC3XX`  | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| Brainpool384r1        | Not supported                                              | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| Brainpool512r1        | Not supported                                              | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| Curve25519            | :kconfig:`PSA_CRYPTO_DRIVER_ECC_MONTGOMERY_255_CC3XX`      | :kconfig:`PSA_CRYPTO_DRIVER_ECC_MONTGOMERY_255_OBERON`      |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| Curve448              | Not supported                                              | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp192k1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_K1_192_CC3XX`         | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp256k1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_K1_256_CC3XX`         | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp192r1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_192_CC3XX`         | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp224r1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_224_CC3XX`         | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_224_OBERON`         |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp256r1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_256_CC3XX`         | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_256_OBERON`         |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp384r1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_384_CC3XX`         | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+
| secp521r1             | :kconfig:`PSA_CRYPTO_DRIVER_ECC_SECP_R1_521_CC3XX`         | Not supported                                               |
+-----------------------+------------------------------------------------------------+-------------------------------------------------------------+


RSA configurations
******************

Rivest-Shamir-Adleman (RSA) support can be enabled by setting one or more Kconfig variables in the following table:

+-----------------------+---------------------------------------------+
| RSA algorithms        | Configurations                              |
+=======================+=============================================+
| RSA OAEP              | :kconfig:`PSA_WANT_ALG_RSA_OAEP`            |
+-----------------------+---------------------------------------------+
| RSA PKCS#1 v1.5 crypt | :kconfig:`PSA_WANT_ALG_RSA_PKCS1V15_CRYPT`  |
+-----------------------+---------------------------------------------+
| RSA PKCS#1 v1.5 sign  | :kconfig:`PSA_WANT_ALG_RSA_PKCS1V15_SIGN`   |
+-----------------------+---------------------------------------------+
| RSA PSS               | :kconfig:`PSA_WANT_ALG_RSA_PSS`             |
+-----------------------+---------------------------------------------+


RSA driver configurations
=========================

The Kconfig variables in the following table can be used for fine-grained control over which drivers provides RSA support:

+-----------------------+------------------------------------------------------------+----------------------------+
| RSA algorithms        | nrf_cc3xx driver support                                   | nrf_oberon driver support  |
+=======================+============================================================+============================+
| RSA OAEP              | :kconfig:`PSA_CRYPTO_DRIVER_ALG_RSA_OAEP_CC3XX`            | Not supported              |
+-----------------------+------------------------------------------------------------+----------------------------+
| RSA PKCS#1 v1.5 crypt | :kconfig:`PSA_CRYPTO_DRIVER_ALG_RSA_PKCS1V15_CRYPT_CC3XX`  | Not supported              |
+-----------------------+------------------------------------------------------------+----------------------------+
| RSA PKCS#1 v1.5 sign  | :kconfig:`PSA_CRYPTO_DRIVER_ALG_RSA_PKCS1V15_SIGN_CC3XX`   | Not supported              |
+-----------------------+------------------------------------------------------------+----------------------------+
| RSA PSS               | :kconfig:`PSA_CRYPTO_DRIVER_ALG_RSA_PSS_CC3XX`             | Not supported              |
+-----------------------+------------------------------------------------------------+----------------------------+

.. note:
   * If an RSA algorithm is enabled and no PSA driver enables or supports it, then builtin Mbed TLS support is enabled and used.
   * :ref:`nrf_security_drivers_cc3xx`  is limited to key sizes of <= 2048 bits.


Secure Hash configurations
**************************

The Secure Hash algorithms can be configured by setting one or more Kconfig variables according in the following table:

+-----------------------+---------------------------------+
| Hash algorithm        | Configurations                  |
+=======================+=================================+
| SHA-1                 | :kconfig:`PSA_WANT_ALG_SHA_1`   |
+-----------------------+---------------------------------+
| SHA-224               | :kconfig:`PSA_WANT_ALG_SHA_224` |
+-----------------------+---------------------------------+
| SHA-256               | :kconfig:`PSA_WANT_ALG_SHA_256` |
+-----------------------+---------------------------------+
| SHA-384               | :kconfig:`PSA_WANT_ALG_SHA_384` |
+-----------------------+---------------------------------+
| SHA-512               | :kconfig:`PSA_WANT_ALG_SHA_512` |
+-----------------------+---------------------------------+


Secure Hash driver configurations
=================================

PSA driver specific configurations given in this table can be used for fine-grained control over which drivers provides the Secure Hash algorithm.

+-----------------------+-------------------------------------------------+-------------------------------------------------+
| Hash algorithm        |  nrf_cc3xx driver support                       | nrf_oberon driver support                       |
+=======================+=================================================+=================================================+
| SHA-1                 |  :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_1_CC3XX`   | :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_1_OBERON`   |
+-----------------------+-------------------------------------------------+-------------------------------------------------+
| SHA-224               |  :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_224_CC3XX` | :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_224_OBERON` |
+-----------------------+-------------------------------------------------+-------------------------------------------------+
| SHA-256               |  :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_256_CC3XX` | :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_256_OBERON` |
+-----------------------+-------------------------------------------------+-------------------------------------------------+
| SHA-384               |  Not supported                                  | :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_384_OBERON` |
+-----------------------+-------------------------------------------------+-------------------------------------------------+
| SHA-512               |  Not supported                                  | :kconfig:`PSA_CRYPTO_DRIVER_ALG_SHA_512_OBERON` |
+-----------------------+-------------------------------------------------+-------------------------------------------------+

.. note:
   * If Secure Hash algorithm is enabled and no PSA driver enables or supports it, then builtin Mbed TLS support is enabled and used.
