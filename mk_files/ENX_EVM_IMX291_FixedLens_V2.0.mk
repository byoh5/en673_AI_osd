##################################################
# EN673 Configuration Script
##################################################

# Image Path settings
export EXTRA_CFLAGS := -DUSING_CONFIG_IMGPATH_SCRIPT
export EXTRA_CFLAGS += -DIMAGE_INPUT_MODE=0
export EXTRA_CFLAGS += -DUSE_DIGITAL_INPUT=0
export EXTRA_CFLAGS += -DDIGITAL_INPUT_FPS=0
export EXTRA_CFLAGS += -DDIGITAL_INPUT_RES=0

# ISP settings
export EXTRA_CFLAGS += -DUSING_CONFIG_ISP_SCRIPT
export EXTRA_CFLAGS += -Dmodel_Sens=15
export EXTRA_CFLAGS += -DSENS_CONTROL_MODE=1
export EXTRA_CFLAGS += -Dmodel_Key=0
export EXTRA_CFLAGS += -Dmodel_Lens=0
export EXTRA_CFLAGS += -Dmodel_MAF_Lens=0
export EXTRA_CFLAGS += -Dmodel_Iris=2
export EXTRA_CFLAGS += -Dmodel_Tdn=2
export EXTRA_CFLAGS += -Dmodel_Cds=0
export EXTRA_CFLAGS += -Dmodel_Led=1
export EXTRA_CFLAGS += -Dmodel_MotionOut=0
export EXTRA_CFLAGS += -Dmodel_Shade=1
export EXTRA_CFLAGS += -Dmodel_Save=1
export EXTRA_CFLAGS += -Dmodel_Par=1
export EXTRA_CFLAGS += -Dmodel_Par_Save=0

# GPIO assign settings
export EXTRA_CFLAGS += -DUSING_CONFIG_GPIO_SCRIPT
export EXTRA_CFLAGS += -DGPIO_SENS_RSTN=GPIO_00
export EXTRA_CFLAGS += -DGPIO_ISP_VI_LED=GPIO_01
export EXTRA_CFLAGS += -DGPIO_H264_ENC_LED=GPIO_02
export EXTRA_CFLAGS += -DGPIO_TDN_1=GPIO_03
export EXTRA_CFLAGS += -DGPIO_TDN_2=GPIO_04
#export EXTRA_CFLAGS += -DGPIO_MTR_FCS_AP=GPIO_05
#export EXTRA_CFLAGS += -DGPIO_MTR_FCS_AM=GPIO_06
#export EXTRA_CFLAGS += -DGPIO_MTR_FCS_BP=GPIO_07
#export EXTRA_CFLAGS += -DGPIO_MTR_FCS_BM=GPIO_08
#export EXTRA_CFLAGS += -DGPIO_MTR_ZOOM_AP=GPIO_09
#export EXTRA_CFLAGS += -DGPIO_MTR_ZOOM_AM=GPIO_10
export EXTRA_CFLAGS += -DGPIO_LED_CTL=GPIO_11
#export EXTRA_CFLAGS += -DGPIO_MTR_ZOOM_BP=GPIO_12
export EXTRA_CFLAGS += -DGPIO_MPWM=GPIO_13
export EXTRA_CFLAGS += -DGPIO_ETH_IRQ=GPIO_14
#export EXTRA_CFLAGS += -DGPIO_MOTION=GPIO_15
#export EXTRA_CFLAGS += -DGPIO_MTR_ZOOM_BM=GPIO_20
export EXTRA_CFLAGS += -DGPIO_SDIO1_RST=GPIO_21
export EXTRA_CFLAGS += -DGPIO_ETH_RST=GPIO_22
export EXTRA_CFLAGS += -DGPIO_AUD_RST=GPIO_23
export EXTRA_CFLAGS += -DADC_KEYr=ADC0_RDATr
export EXTRA_CFLAGS += -DADC_CDSr=ADC1_RDATr
#export EXTRA_CFLAGS += -DGPIO_KEY_C=GPIO_16
#export EXTRA_CFLAGS += -DGPIO_KEY_U=GPIO_16
#export EXTRA_CFLAGS += -DGPIO_KEY_D=GPIO_16
#export EXTRA_CFLAGS += -DGPIO_KEY_L=GPIO_16
#export EXTRA_CFLAGS += -DGPIO_KEY_R=GPIO_16