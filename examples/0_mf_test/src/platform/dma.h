/**
 * @file    dma.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_DMA_H_
#define PLAT_DMA_H_

/* GPDMA LLLI (Link List Item)
   GPDMA Channels 0 to 11: GPDMA_CxTR1, GPDMA_CxTR2, GPDMA_CxBR1, GPDMA_CxSAR, GPDMA_CxDAR GPDMA_CxLLR
*/
typedef struct {
    uint32_t CTR1;  /* Control Register 1 (Data widths, bursts, increments) */
    uint32_t CTR2;  /* Control Register 2 (Triggers, transfer type) */
    uint32_t CBR1;  /* Block Register 1 (Number of bytes to transfer) */
    uint32_t SAR;   /* Source Address Register (ADC1->DR) */
    uint32_t DAR;   /* Destination Address Register (SRAM Buffer address) */
    uint32_t CLLR;  /* Channel Link List Register (Address of the next descriptor) */
} GPDMA_LLI_t;

#endif /* PLAT_DMA_H_ */
