/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// This file defines board specific functions.

#ifndef MICROPY_INCLUDED_MIMXRT10XX_IMX_RT_FLASH_CONFIG_FLASH_CONFIG_H
#define MICROPY_INCLUDED_MIMXRT10XX_IMX_RT_FLASH_CONFIG_FLASH_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "mpconfigboard.h"  // For flash size settings

#include "fsl_common.h"
#include "fsl_flexspi_nor_config.h"


// From: fsl_flexspi_nor_boot.h
/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! @name Driver version */
/*@{*/
/*! @brief XIP_DEVICE driver version 2.0.0. */
#define FSL_XIP_DEVICE_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*************************************
 *  IVT Data
 *************************************/
typedef struct _ivt_
{
    /** @ref hdr with tag #HAB_TAG_IVT, length and HAB version fields
     *  (see @ref data)
     */
    uint32_t hdr;
    /** Absolute address of the first instruction to execute from the
     *  image
     */
    uint32_t entry;
    /** Reserved in this version of HAB: should be NULL. */
    uint32_t reserved1;
    /** Absolute address of the image DCD: may be NULL. */
    uint32_t dcd;
    /** Absolute address of the Boot Data: may be NULL, but not interpreted
     *  any further by HAB
     */
    uint32_t boot_data;
    /** Absolute address of the IVT.*/
    uint32_t self;
    /** Absolute address of the image CSF.*/
    uint32_t csf;
    /** Reserved in this version of HAB: should be zero. */
    uint32_t reserved2;
} ivt;

#define IVT_MAJOR_VERSION 0x4
#define IVT_MAJOR_VERSION_SHIFT 0x4
#define IVT_MAJOR_VERSION_MASK 0xF
#define IVT_MINOR_VERSION 0x1
#define IVT_MINOR_VERSION_SHIFT 0x0
#define IVT_MINOR_VERSION_MASK 0xF

#define IVT_VERSION(major, minor)                                    \
    ((((major)&IVT_MAJOR_VERSION_MASK) << IVT_MAJOR_VERSION_SHIFT) | \
     (((minor)&IVT_MINOR_VERSION_MASK) << IVT_MINOR_VERSION_SHIFT))

/* IVT header */
#define IVT_TAG_HEADER 0xD1 /**< Image Vector Table */
#define IVT_SIZE 0x2000
#define IVT_PAR IVT_VERSION(IVT_MAJOR_VERSION, IVT_MINOR_VERSION)
#define IVT_HEADER (IVT_TAG_HEADER | (IVT_SIZE << 8) | (IVT_PAR << 24))

/* Set resume entry */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__Vectors)
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__Vectors)
#elif defined(__ICCARM__)
extern uint32_t __VECTOR_TABLE[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__VECTOR_TABLE)
#elif defined(__GNUC__)
extern uint32_t __VECTOR_TABLE[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__VECTOR_TABLE)
#endif

#if defined(XIP_BOOT_HEADER_DCD_ENABLE) && (1 == XIP_BOOT_HEADER_DCD_ENABLE)
#define DCD_ADDRESS dcd_data
#else
#define DCD_ADDRESS 0
#endif

#define BOOT_DATA_ADDRESS &boot_data
#define CSF_ADDRESS 0
#define IVT_RSVD (uint32_t)(0x00000000)

/*************************************
 *  Boot Data
 *************************************/
typedef struct _boot_data_
{
    uint32_t start;       /* boot start location */
    uint32_t size;        /* size */
    uint32_t plugin;      /* plugin flag - 1 if downloaded application is plugin */
    uint32_t placeholder; /* placehoder to make even 0x10 size */
} BOOT_DATA_T;

#define FLASH_BASE FlexSPI_AMBA_BASE
#define PLUGIN_FLAG (uint32_t)0

/* External Variables */
extern const BOOT_DATA_T boot_data;
#if defined(XIP_BOOT_HEADER_DCD_ENABLE) && (1 == XIP_BOOT_HEADER_DCD_ENABLE)
extern const uint8_t dcd_data[];
#endif

// End from fsl_flexspi_nor_boot.h

#define SEQUENCE(first, second, third, fourth) first, second, third, fourth
#define TWO_EMPTY_STEPS 0x00000000
#define EMPTY_SEQUENCE SEQUENCE(TWO_EMPTY_STEPS, TWO_EMPTY_STEPS, TWO_EMPTY_STEPS, TWO_EMPTY_STEPS)

// FlexSPI configuration that stores command info.
extern const flexspi_nor_config_t qspiflash_config;

uint32_t imx_rt_flash_size(void);

#endif  // MICROPY_INCLUDED_MIMXRT10XX_IMX_RT_FLASH_CONFIG_FLASH_CONFIG_H
