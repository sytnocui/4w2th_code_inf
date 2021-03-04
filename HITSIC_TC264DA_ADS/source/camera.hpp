/*
 * camera.hpp
 *
 *  Created on: 2021年3月3日
 *      Author: 10798
 */

#ifndef SOURCE_CAMERA_HPP_
#define SOURCE_CAMERA_HPP_

#include "everything.hpp"

extern cam_zf9v034_configPacket_t cameraCfg;
extern dmadvp_config_t dmadvpCfg;
extern dmadvp_handle_t dmadvpHandle;
extern disp_ssd1306_frameBuffer_t dispBuffer;
extern uint8_t *fullBuffer;

void Image_Upload_Gray(void);

inline void CAM_ZF9V034_UnitMyTestDmaCallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    static dmadvp_handle_t *dmadvpHandle = (dmadvp_handle_t*)userData;

    DMADVP_EdmaCallbackService(dmadvpHandle, transferDone);
    //PRINTF("new full buffer: 0x%-8.8x = 0x%-8.8x\n", handle->fullBuffer.front(), handle->xferCfg.destAddr);
    if(kStatus_DMADVP_NoEmptyBuffer == DMADVP_TransferStart(dmadvpHandle->base, dmadvpHandle))
    {
        DMADVP_TransferSubmitEmptyBuffer(DMADVP0, dmadvpHandle, fullBuffer);
        /*DMADVP_TransferStop(dmadvpHandle->base, dmadvpHandle);
         PRINTF("transfer stop! insufficent buffer\n");*/
    }
    if(true==transferDone)
    {
        DMADVP_TransferGetFullBuffer(DMADVP0, dmadvpHandle, &fullBuffer);
        image_main();
//        Image_Upload_Gray();//采图代码
        DMADVP_TransferSubmitEmptyBuffer(DMADVP0, dmadvpHandle, fullBuffer);
        State_Update();
        Ctrl_Update();
    }
}

inline void CAM_ZF9V034_UnitMyTest(void)
{
    CAM_ZF9V034_GetDefaultConfig(&cameraCfg);//设置摄像头配置
    cameraCfg.imageGain = 16;
    CAM_ZF9V034_CfgWrite(&cameraCfg);//写入配置
    //CAM_ZF9V034_CfgRead(&cameraCfg);

    CAM_ZF9V034_GetReceiverConfig(&dmadvpCfg, &cameraCfg);//生成对应接收器的配置数据，使用此数据初始化接受器并接收图像数据。

    DMADVP_Init(DMADVP0, &dmadvpCfg);

    DMADVP_TransferCreateHandle(&dmadvpHandle, DMADVP0, CAM_ZF9V034_UnitMyTestDmaCallback);

    //FIXME: do cache operation HERE!
    uint8_t *imageBuffer0 = new uint8_t[DMADVP0->imgSize];
    uint8_t *imageBuffer1 = new uint8_t[DMADVP0->imgSize];
    uint8_t *imageBuffer2 = new uint8_t[DMADVP0->imgSize];

    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer0);
    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer1);
    DMADVP_TransferSubmitEmptyBuffer(DMADVP0, &dmadvpHandle, imageBuffer2);
    DMADVP_TransferStart(DMADVP0, &dmadvpHandle);
}



#endif /* SOURCE_CAMERA_HPP_ */
