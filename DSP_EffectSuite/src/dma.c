/*
 * dma.c
 *
 *  Created on: Nov 16, 2024
 *      Author: felip
 */
#include <stdio.h>
#include "dma.h"
#include "define.h"
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"

#include <csl.h>
#include <csl_irq.h>
#include <csl_dma.h>

static Uint8 transferComplete = 0;
extern void VECSTART(void);
Uint8 teste = 0;

Uint16 xmtEventId, rcvEventId;
Uint32 old_intm;
//interrupt void dmaRcvIsr(void);

/* Define a DMA_Handle object */
DMA_Handle dmaReceive;
DMA_Handle dmaTransmit;

DMA_Config myconfig_receive = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST, // No burst for destination
        DMA_DMACSDP_DSTPACK_OFF,    // No packing for destination
        DMA_DMACSDP_DST_DARAMPORT1, // Destination: DARAM memory
        DMA_DMACSDP_SRCBEN_NOBURST, // No burst for source
        DMA_DMACSDP_SRCPACK_OFF,    // No packing for source
        DMA_DMACSDP_SRC_PERIPH,     // Source: peripheral (McBSP)
        DMA_DMACSDP_DATATYPE_16BIT  // Data type: 16 bits
    ),

    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_POSTINC, // Post-increment for destination address
        DMA_DMACCR_SRCAMODE_CONST,  // Fixed address for source (DRR1)
        DMA_DMACCR_ENDPROG_OFF,
        DMA_DMACCR_WP_DEFAULT,
        DMA_DMACCR_REPEAT_ALWAYS,   // Continuous repetition
        DMA_DMACCR_AUTOINIT_ON,     // Automatic reinitialization
        DMA_DMACCR_EN_STOP,
        DMA_DMACCR_PRIO_HI,         // High priority
        DMA_DMACCR_FS_ELEMENT,
        DMA_DMACCR_SYNC_REVT1       // Synchronization on RX event
    ),

    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,       // Enable interrupt for address error
        DMA_DMACICR_BLOCKIE_OFF,
        DMA_DMACICR_LASTIE_ON,
        DMA_DMACICR_FRAMEIE_OFF,      // Interrupt at the end of the frame
        DMA_DMACICR_FIRSTHALFIE_OFF,
        DMA_DMACICR_DROPIE_OFF,
        DMA_DMACICR_TIMEOUTIE_OFF
    ),

    (DMA_AdrPtr)0x5800, // Source: DRR1 (data received from McBSP1)
    0,
    (DMA_AdrPtr)0x5000, //  DMA source set to random location (Set to array later)
    0,
    BUFF_SIZE,          // Number of elements
    1,                  // 1 frame
    0,                  // DMACSFI - Source frame index
    2,                  // DMACSEI - Source element index
    0,                  // DMACDFI - Destination frame index
    0                   // DMACDEI - Destination element index
};


DMA_Config  myconfig_transmit = {
     DMA_DMACSDP_RMK(
         DMA_DMACSDP_DSTBEN_NOBURST , // Destination burst
         DMA_DMACSDP_DSTPACK_OFF,     // Destination packing
         DMA_DMACSDP_DST_PERIPH ,     // Source selection
         DMA_DMACSDP_SRCBEN_NOBURST , // Source burst
         DMA_DMACSDP_SRCPACK_OFF,     // Source packing
         DMA_DMACSDP_SRC_DARAMPORT1 , // Source selection
         DMA_DMACSDP_DATATYPE_16BIT   // Data type
     ), /* DMACSDP */

     DMA_DMACCR_RMK(
         DMA_DMACCR_DSTAMODE_CONST,   // Destination address mode
         DMA_DMACCR_SRCAMODE_POSTINC, // Source address mode
         DMA_DMACCR_ENDPROG_OFF,      // End of programmation bit
         DMA_DMACCR_WP_DEFAULT,
         DMA_DMACCR_REPEAT_ALWAYS,    // Repeat condition
         DMA_DMACCR_AUTOINIT_ON,      // Auto initialization bit
         DMA_DMACCR_EN_STOP,          // Channel enable
         DMA_DMACCR_PRIO_HI,          // Channel priority
         DMA_DMACCR_FS_ELEMENT,       // Frame\Element Sync
         DMA_DMACCR_SYNC_XEVT1        // Synchronization control
     ), /* DMACCR */

     DMA_DMACICR_RMK(
         DMA_DMACICR_AERRIE_OFF,
         DMA_DMACICR_BLOCKIE_OFF,
         DMA_DMACICR_LASTIE_OFF,
         DMA_DMACICR_FRAMEIE_OFF,
         DMA_DMACICR_FIRSTHALFIE_OFF,
         DMA_DMACICR_DROPIE_OFF,
         DMA_DMACICR_TIMEOUTIE_OFF
       ), /* DMACICR */

     (DMA_AdrPtr)0x5000, // DMACSSAL - DMA source set to random location (Set to array later)
     0,                  // DMACSSAU
     (DMA_AdrPtr)0x5804, // DMACDSAL - DMA destination is DXR1
     0,                  // DMACDSAU
     BUFF_SIZE,                 // DMACEN  - 96 elements (48 for each channel)
     1,                  // DMACFN  - Single frame
     0,                  // DMACSFI - Source frame index
     0,                  // DMACSEI - Source element index
     0,                  // DMACDFI - Destination frame index
     2                   // DMACDEI - Destination element index
};

interrupt void dmaRcvIsr(void) {
     //DMA_stop(dmaReception);        // Para a operação de recepção do DMA.
    transferComplete = 1;
    teste = 50;

}

void configAudioDma(Int16 *buffReceive, Int16 *buffTransmit){
    transferComplete = 0;
    CSL_init();
    IRQ_setVecs((Uint32)(&VECSTART));
    /* Open DMA Channel */
    dmaReceive = DMA_open(DMA_CHA0, 0);
    dmaTransmit = DMA_open(DMA_CHA1, 0);

    /* Configure lower address registers */
    myconfig_receive.dmacdsal = (DMA_AdrPtr)(((Uint32)buffReceive)<<1);

    /* Configure lower address registers */
    myconfig_transmit.dmacssal = (DMA_AdrPtr)(((Uint32)buffTransmit) << 1);

    /* Write configuration structure values to DMA control registers */
    DMA_config(dmaReceive, &myconfig_receive);
    DMA_config(dmaTransmit, &myconfig_transmit);

    /* Get interrupt event associated with DMA receive and transmit */
    xmtEventId = DMA_getEventId(dmaTransmit);
    rcvEventId = DMA_getEventId(dmaReceive);

    /* Temporarily disable interrupts and clear any pending */
    /* interrupts for MCBSP transmit */
    old_intm = IRQ_globalDisable();

    /* Clear any pending interrupts for DMA channels */
    IRQ_clear(xmtEventId);
    IRQ_clear(rcvEventId);

    /* Enable DMA interrupt in IER register */
    IRQ_enable(xmtEventId);
    IRQ_enable(rcvEventId);

    /* Place DMA interrupt service addresses at associate vector */
    //IRQ_plug(xmtEventId,&dmaXmtIsr);
    IRQ_plug(rcvEventId,&dmaRcvIsr);

    IRQ_globalEnable();
    teste = 10;
}

/*
 *  startAudioDma( )
 *
 *    Start DMA transfer for Audio
 */
void startAudioDma (void)
{
    /* Enable DMA channel to begin transfer */
    DMA_start(dmaReceive);
    DMA_start(dmaTransmit);
    IRQ_enable(rcvEventId);
}

/*
 *  stopAudioDma( )
 *
 *    Stop DMA transfer for Audio
 */
void stopAudioDma (void)
{
    DMA_stop(dmaReceive);
    DMA_stop(dmaTransmit);
}

void resetSinal(){
    transferComplete = 0;
}

Uint8 getSinal(){
    return transferComplete;
}

