#include "epos.h"

u8 eposInitOK=0;

EPOSmotorTypedef EPOSmotor[4];



/*
 * @description: set EPOS mode of operation
 * @param: ID:      node-ID
 *         mode:    operation mode
 *                  1  ->  Proofile Position Mode (PPM)
 *                  3  ->  Profile Velocity Mode (PVM)
 *                  6  ->  Homing Mode (HMM)
 *                  8  ->  Cyclic Synchronous Position Mode (CSP)
 *                  9  ->  Cyclic Synchoronous Velocity Mode (CSV)
 *                  10 ->  Cyclic Synchronous Torque Mode (CST)
 *          InConGrpFlag: put the CAN message in the control group
 */
void EPOS_SetMode(u8 ID, s32 mode, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=mode;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;
}

/*
 * @descrription: torque on motor
 * @param: ID:  node-ID
 *         InConGrpFlag: put the CAN message in the control group
 */
void EPOS_StartMotor(u8 ID, u8 InConGrpFlag)
{
    //Ready to switch on
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x06;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;
    //Switched on
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x07;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;
    //Operation enabled
    EPOS_EnableOperation(ID, InConGrpFlag);
}

/*
 * @descrription: Diable power section
 * @param: ID:  node-ID
 *         InConGrpFlag: put the CAN message in the control group
 */
void EPOS_RelieveMotor(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;
}

/*
 * @descrription: set target velocity (PVM)
 * @param: ID:  node-ID
 *         speed: target velocity
 *         InConGrpFlag: put the CAN message in the control group
 */
void EPOS_SetPVMspeed(u8 ID, s32 speed, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0xFF;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
        EncodeS32Data(&speed,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;
}

/*
 * @descrription: Enable drive function (enable current controller and, if needed, positon or velocity controller)
 * @param: ID:  node-ID
 *         InConGrpFlag: put the CAN message in the control group
 */
void EPOS_EnableOperation(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x0F;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;
}

/*
 * @descrription: Quick stop movement
 * @param: ID:  node-ID
 *         InConGrpFlag: put the CAN message in the control group
 */
void EPOS_QuickStop(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x20;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;   
}

/*
 * @descrription: Stop axis
 * @param: ID:  node-ID
 *         InConGrpFlag: put the CAN message in the control group
 */
void EPOS_Halt(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x0F;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x01;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;   
}

//不清楚效果，待验证
void EPOS_ClearFault(u8 ID, u8 InConGrpFlag)
{
  EPOS_RelieveMotor(ID, InConGrpFlag);
  
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x22;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x80;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2;   
}

void EPOS_ReadStatusword(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x41;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2; 
}

void EPOS_ReadPosition(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x64;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2; 
}

void EPOS_ReadVelocity(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0xD3;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x30;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x01;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2; 
}

void EPOS_ReadTorque(u8 ID, u8 InConGrpFlag)
{
    if(Rear2==Can2_Sendqueue.Front)
    {
        flag.Can2SendqueueFULL++;
        return;
    }
    else
    {
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x600+ID;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]=0x40;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]=0x77;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0x60;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0x00;
        Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=InConGrpFlag;
    }
    Can2_Sendqueue.Rear=Rear2; 
}
