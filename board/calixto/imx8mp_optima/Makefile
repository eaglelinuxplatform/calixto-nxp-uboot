#
# Copyright 2019 NXP
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8mp_optima.o

ifdef CONFIG_XPL_BUILD
obj-y += spl.o
#ifdef CONFIG_IMX8M_LPDDR4_FREQ0_3200MTS
#obj-y += lpddr4_timing_ndm.o
#else
obj-$(CONFIG_IMX8MP_LPDDR4_2GB) += lpddr4_calixto_optima_H2AB16G32D6C_timing.o
obj-$(CONFIG_IMX8MP_LPDDR4_4GB) += lpddr4_calixto_optima_MT53E1G32D2FW_timing.o

endif
