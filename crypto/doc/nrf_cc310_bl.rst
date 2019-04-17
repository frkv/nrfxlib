.. _nrf_cc310_bl_readme:

nrf_cc310_bl crypto library
###########################

The nrf_cc310_bl library is a software library to interface with ARM CryptoCell
CC310 hardware accelerator that is available on nRF52840 and nRF9160 SoCs. The
library adds hardware support for cryptographic algorithms to be used in a
bootloader-specific use cases.

The nrf_cc310_bl library support the following cryptographic algorithms:
* ECDSA verify using NIST curve secp256r1
* SHA-256

Initializing the library
========================
The library must be initialized before the APIs can be used

.. code-block:: c
        :caption: Initializing the nrf_cc310_bl library

        if (nrf_cc310_bl_init() != 0) {
                /** nrf_cc310_bl failed to initialize. */
                return -1;
        }


Enabling/Disabling the CryptoCell the hardware
==============================================
The CryptocCell CC310 hardware needs to be manually enabled/disabled prior to
the API calls in the nrf_cc310_bl library.

.. note:: The CryptoCell consumes power when the hardware is enabled even if
there is no ongoing operation.

Enabling the CryptoCell hardware
--------------------------------

The hardware is enabled by writing to a specific register.

.. code-block:: c
        :caption: Enabling the CryptoCell hardware

        NRF_CRYPTOCELL->ENABLE=1;

Disabling the CryptoCell hardware
---------------------------------

The hardware is disabled by writing to a specific register.

.. code-block:: c
        :caption : Disabling the CryptoCell hardware
        NRF_CRYPTOCELL->ENABLE=0;

*Note that structure type for the CryptoCell HW register is called
``NRF_CRYPTOCELL_S`` in nRF9160 due to HW only being accessible from the Secure
Processing Environment in the Cortex-M33 architecture.*

API documentation
*****************

.. doxygengroup:: nrf_cc310_bl
   :project: nrfxlib
   :members:
