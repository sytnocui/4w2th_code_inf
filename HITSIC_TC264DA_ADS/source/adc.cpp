#include "adc.hpp"

uint32 LV_Temp[ChannelTimes][SampleTimes];
const uint32 channels[ChannelTimes] = {16, 23, 17, 18, 10, 11, 12, 13};
float LV[ChannelTimes];
float AD[ChannelTimes];
float em_error;
void swap(uint32 *x,uint32 *y);

void LV_Sample()                             // ad�ɼ�����
{
    for(uint8 i=0;i<SampleTimes;i++)
    {
        /*��ȡ������ֵ*/
        LV_Temp[0][i]=SCADC_Sample(ADC0,0,channels[0]);
        LV_Temp[2][i]=SCADC_Sample(ADC0,0,channels[2]);
    }
}

void LV_Get_Val()//Լ0.3mS                  //�Բɼ���ֵ�˲�
{
  //��ʱ����0-65535(255)������
  for(uint8 i=0;i<=8;i++)
  {
    for(uint8 j=0;j<=SampleTimes-1;j++)
    {
         if(LV_Temp[i][j]>500)//�޳�ë���ź�
         {
             LV_Temp[i][j]=500;
         }
    }
  }

  //����
  for(uint8 k=0;k<8;k++)
  {
    for(uint8 i=0;i<=SampleTimes-1;i++)
    {
      for(uint8 j=i+1;j<SampleTimes;j++)
      {
          if(LV_Temp[k][i]>LV_Temp[k][j])
              swap(&LV_Temp[k][i],&LV_Temp[k][j]);//������swap�����Լ�д
      }
    }
  }

  for(uint8 k=0;k<8;k++)
  {
    LV[k]=0;
    for(uint8 i=3;i<=SampleTimes-4;i++)
    {
         LV[k]+=(float)LV_Temp[k][i];
    }
    LV[k]=LV[k]/(SampleTimes-6);
    if( LV[k] < 1 )
    {
       LV[k] = 1;
    }
  }

  AD[0] = LV[0];    //��ʹ��
//  AD[1] = LV[1];
  AD[2] = LV[2];    //��ʹ��
//  AD[3] = LV[3];
//  AD[4] = LV[4];
//  AD[5] = LV[5];
//  AD[6] = LV[6];
//  AD[7] = LV[7];
}

void swap(uint32 *x,uint32 *y)
{
    uint32 temp = 0;
    temp = *x;
    *x = *y;
    *y = temp;
}

void ADC_Upgrade(void)
{
    LV_Sample();
    //LV_Get_Val();//�˲�
    em_error = (float)((AD[2]-AD[0])/(AD[0]*AD[2]));
}
