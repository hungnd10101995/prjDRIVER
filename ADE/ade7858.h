/*
 * ade7858.h
 *
 * Created: 06/05/2016
 *  Author: TUANTM
 */ 


#ifndef ADE7858_H_
#define ADE7858_H_

// Header prjEMETERS
#include "hardware.h"

//#define ADE78xx_NEUTRAL_SUPPORT
// Calibration Data

#define CAL_CVRMS 41951
#define CAL_CIRMS 17687307
#define CAL_CPOW 18628

//#define ADE78XX_ANGLE_SWITCH_MODE_SUPPORT
#define CAL_POW 8388608 //2^23
#define DIGITAL_FILTER_CLK  256000
#define F_LINE     50 //Hz
//#define THR1    0x000003
//#define THR0    0xA08EAC
//analog device sample
#if defined(ADE7858_SUPPORT)
#define THR1_C    0x15//0x0223
#define THR0_C    0x4F193C//0x2B1900
#elif defined(ADE7880_SUPPORT)
#define THR_C    3 //default //16 //tinh lai so nay
#endif
#ifdef METER_3WIRE
    #define COMPMODE_C  0x016D //disable phase B, clear bit 1, 4, 7
#else
    #define COMPMODE_C  0x01FF
#endif
// Register addresses
#if defined(ADE7858_SUPPORT)
#define AIGAIN		0x4380 //Phase A current gain adjust. <display>
#define AVGAIN		0x4381 //Phase A voltage gain adjust. <display>
#define BIGAIN		0x4382
#define BVGAIN		0x4383
#define CIGAIN		0x4384
#define CVGAIN		0x4385
#define NIGAIN		0x4386
#define AIRMSOS		0x4387 //Phase A current rms offset 
#define AVRMSOS		0x4388 //Phase A voltage rms offset.
#define BIRMSOS		0x4389
#define BVRMSOS		0x438A
#define CIRMSOS		0x438B
#define CVRMSOS		0x438C
#define NIRMSOS		0x438D
#define AVAGAIN		0x438E //Phase A apparent power gain adjust.
#define BVAGAIN		0x438F
#define CVAGAIN		0x4390
#define AWGAIN		0x4391 //Phase A total active power gain adjust. <pulse - write to ADE register>
#define AWATTOS		0x4392 //Phase A total active power offset adjust.
#define BWGAIN		0x4393
#define BWATTOS		0x4394
#define CWGAIN		0x4395
#define CWATTOS		0x4396
#define AVARGAIN	0x4397 //Phase A total reactive power gain adjust <pulse - write to ADE register>
#define AVAROS		0x4398 //Phase A total reactive power offset adjust
#define BVARGAIN	0x4399
#define BVAROS		0x439A
#define CVARGAIN	0x439B
#define CVAROS		0x439C
#define AFWGAIN		0x439D //Phase A fundamental active power gain adjust
#define AFWATTOS	0x439E //Phase A fundamental active power offset adjust
#define BFWGAIN		0x439F
#define BFWATTOS	0x43A0
#define CFWGAIN		0x43A1
#define CFWATTOS	0x43A2 //0x43A3
#define AFVARGAIN	0x43A3 //0x439E //Phase A fundamental reactive power gain adjust (ADE7878 only).

#define APHCAL		0xE614 //Phase calibration of Phase A <phase shift - write to ADE register>
#define BPHCAL		0xE615
#define CPHCAL		0xE616
//
#define AFVAROS		0x43A4
#define BFVARGAIN	0x43A5
#define BFVAROS		0x43A6
#define CFVARGAIN	0x43A7
#define CFVAROS		0x43A8
#define VATHR1		0x43A9
#define VATHR0		0x43AA
#define WTHR1		0x43AB
#define WTHR0		0x43AC
#define VARTHR1		0x43AD
#define VARTHR0		0x43AE
#define VANOLOAD	0x43B0
#define APNOLOAD	0x43B1
#define VARNOLOAD	0x43B2
#define VLEVEL		0x43B3
#define DICOEFF		0x43B5
#define HPFDIS		0x43B6
#define ISUM		0x43BF
#define AIRMS		0x43C0
#define AVRMS		0x43C1
#define BIRMS		0x43C2
#define BVRMS		0x43C3
#define CIRMS		0x43C4
#define CVRMS		0x43C5
#define NIRMS		0x43C6
#define RUN			0xE228
#define AWATTHR		0xE400
#define BWATTHR		0xE401
#define CWATTHR		0xE402
#define AFWATTHR	0xE403
#define BFWATTHR	0xE404
#define CFWATTHR	0xE405
#define AVARHR		0xE406
#define BVARHR		0xE407
#define CVARHR		0xE408
#define AFVARHR		0xE409
#define BFVARHR		0xE40A
#define CFVARHR		0xE40B
#define AVAHR		0xE40C
#define BVAHR		0xE40D
#define CVAHR		0xE40E
#define IPEAK		0xE500
#define VPEAK		0xE501
#define STATUS0		0xE502
#define STATUS1		0xE503
#define AIMAV		0xE504
#define BIMAV		0xE505
#define CIMAV		0xE506
#define OILVL		0xE507
#define OVLVL		0xE508
#define SAGLVL		0xE509
#define MASK0		0xE50A
#define MASK1		0xE50B
#define IAWV		0xE50C
#define IBWV		0xE50D
#define ICWV		0xE50E
#define INWV		0xE50F
#define VAWV		0xE510
#define VBWV		0xE511
#define VCWV		0xE512
#define AWATT		0xE513
#define BWATT		0xE514
#define CWATT		0xE515
#define AVAR		0xE516
#define BVAR		0xE517
#define CVAR		0xE518
#define AVA			0xE519
#define BVA			0xE51A
#define CVA			0xE51B
#define CHECKSUM	0xE51F
#define VNOM		0xE520
#define PHSTATUS	0xE600
#define ANGLE0		0xE601
#define ANGLE1		0xE602
#define ANGLE2		0xE603
#define PERIOD		0xE607
#define PHNOLOAD	0xE608
#define LINECYC		0xE60C
#define ZXTOUT		0xE60D
#define COMPMODE	0xE60E
#define GAIN		0xE60F
#define CFMODE		0xE610
#define CF1DEN		0xE611
#define CF2DEN		0xE612
#define CF3DEN		0xE613
//
#define PHSIGN		0xE617
#define CONFIG		0xE618
#define MMODE		0xE700
#define ACCMODE		0xE701
#define LCYCMODE	0xE702
#define PEAKCYC		0xE703
#define SAGCYC		0xE704
#define CFCYC		0xE705
#define HSDC_CFG	0xE706
#define VERSION_ADE	0xE707
#define LPOILVL		0xEC00
#define CONFIG2		0xEC01

#define ADE7858_DUMMY_ADDR     0xEBFF

#elif defined(ADE7880_SUPPORT)

#define AIGAIN 0x4380
#define AVGAIN 0x4381
#define BIGAIN 0x4382
#define BVGAIN 0x4383
#define CIGAIN 0x4384
#define CVGAIN 0x4385
#define NIGAIN 0x4386
#define DICOEFF 0x4388
#define APGAIN 0x4389
#define AWATTOS 0x438A
#define BPGAIN 0x438B
#define BWATTOS 0x438C
#define CPGAIN 0x438D
#define CWATTOS 0x438E
#define AIRMSOS 0x438F
#define AVRMSOS 0x4390
#define BIRMSOS 0x4391
#define BVRMSOS 0x4392
#define CIRMSOS 0x4393
#define CVRMSOS 0x4394
#define NIRMSOS 0x4395
#define HPGAIN 0x4398
#define ISUMLVL 0x4399
#define VLEVEL 0x439F
#define AFWATTOS 0x43A2
#define BFWATTOS 0x43A3
#define CFWATTOS 0x43A4
#define AFVAROS 0x43A5
#define BFVAROS 0x43A6
#define CFVAROS 0x43A7
#define AFIRMSOS 0x43A8
#define BFIRMSOS 0x43A9
#define CFIRMSOS 0x43AA
#define AFVRMSOS 0x43AB
#define BFVRMSOS 0x43AC
#define CFVRMSOS 0x43AD
#define HXWATTOS 0x43AE
#define HYWATTOS 0x43AF
#define HZWATTOS 0x43B0
#define HXVAROS 0x43B1
#define HYVAROS 0x43B2
#define HZVAROS 0x43B3
#define HXIRMSOS 0x43B4
#define HYIRMSOS 0x43B5
#define HZIRMSOS 0x43B6
#define HXVRMSOS 0x43B7
#define HYVRMSOS 0x43B8
#define HZVRMSOS 0x43B9
#define AIRMS 0x43C0
#define AVRMS 0x43C1
#define BIRMS 0x43C2
#define BVRMS 0x43C3
#define CIRMS 0x43C4
#define CVRMS 0x43C5
#define NIRMS 0x43C6
#define ISUM 0x43C7
#define RUN 0xE228
#define AWATTHR 0xE400
#define BWATTHR 0xE401
#define CWATTHR 0xE402
#define AFWATTHR 0xE403
#define BFWATTHR 0xE404
#define CFWATTHR 0xE405
#define AFVARHR 0xE409
#define BFVARHR 0xE40A
#define CFVARHR 0xE40B
#define AVAHR 0xE40C
#define BVAHR 0xE40D
#define CVAHR 0xE40E
#define IPEAK 0xE500
#define VPEAK 0xE501
#define STATUS0 0xE502
#define STATUS1 0xE503
#define AIMAV 0xE504
#define BIMAV 0xE505
#define CIMAV 0xE506
#define OILVL 0xE507
#define OVLVL 0xE508
#define SAGLVL 0xE509
#define MASK0 0xE50A
#define MASK1 0xE50B
#define IAWV 0xE50C
#define IBWV 0xE50D
#define ICWV 0xE50E
#define INWV 0xE50F
#define VAWV 0xE510
#define VBWV 0xE511
#define VCWV 0xE512
#define AWATT 0xE513
#define BWATT 0xE514
#define CWATT 0xE515
#define AFVAR 0xE516
#define BFVAR 0xE517
#define CFVAR 0xE518
#define AVA 0xE519
#define BVA 0xE51A
#define CVA 0xE51B
#define CHECKSUM 0xE51F
#define VNOM 0xE520
#define LAST_RWDATA_24bit 0xE5FF
#define PHSTATUS 0xE600
#define ANGLE0 0xE601
#define ANGLE1 0xE602
#define ANGLE2 0xE603
#define PHNOLOAD 0xE608
#define LINECYC 0xE60C
#define ZXTOUT 0xE60D
#define COMPMODE 0xE60E
#define Gain 0xE60F
#define CFMODE 0xE610
#define CF1DEN 0xE611
#define CF2DEN 0xE612
#define CF3DEN 0xE613
#define APHCAL 0xE614
#define BPHCAL 0xE615
#define CPHCAL 0xE616
#define PHSIGN 0xE617
#define CONFIG 0xE618
#define MMODE 0xE700
#define ACCMODE 0xE701
#define LCYCMODE 0xE702
#define PEAKCYC 0xE703
#define SAGCYC 0xE704
#define CFCYC 0xE705
#define HSDC_CFG 0xE706
#define Version 0xE707
#define LAST_RWDATA_8bit 0xE7FD
#define FVRMS 0xE880
#define FIRMS 0xE881
#define FWATT 0xE882
#define FVAR 0xE883
#define FVA 0xE884
#define FPF 0xE885
#define VTHDN 0xE886
#define ITHDN 0xE887
#define HXVRMS 0xE888
#define HXIRMS 0xE889
#define HXWATT 0xE88A
#define HXVAR 0xE88B
#define HXVA 0xE88C
#define HXPF 0xE88D
#define HXVHD 0xE88E
#define HXIHD 0xE88F
#define HYVRMS 0xE890
#define HYIRMS 0xE891
#define HYWATT 0xE892
#define HYVAR 0xE893
#define HYVA 0xE894
#define HYPF 0xE895
#define HYVHD 0xE896
#define HYIHD 0xE897
#define HZVRMS 0xE898
#define HZIRMS 0xE899
#define HZWATT 0xE89A
#define HZVAR 0xE89B
#define HZVA 0xE89C
#define HZPF 0xE89D
#define HZVHD 0xE89E
#define HZIHD 0xE89F
#define HCONFIG 0xE900
#define APF 0xE902
#define BPF 0xE903
#define CPF 0xE904
#define APERIOD 0xE905
#define BPERIOD 0xE906
#define CPERIOD 0xE907
#define APNOLOAD 0xE908
#define VARNOLOAD 0xE909
#define VANOLOAD 0xE90A
#define LAST_ADD 0xE9FE
#define LAST_RWDATA_16bit 0xE9FF
#define CONFIG3 0xEA00
#define LAST_OP 0xEA01
#define WTHR 0xEA02
#define VARTHR 0xEA03
#define VATHR 0xEA04
#define HX_reg 0xEA08
#define HY_reg 0xEA09
#define HZ_reg 0xEA0A
#define LPOILVL 0xEC00
#define CONFIG2 0xEC01

#define ADE7858_DUMMY_ADDR     0xEBFF
#endif

//======================================================================
// Register bit masks

// IPEAK
#define IPEAKVAL	0xFFFFFF
#define IPPHASE0	0x1000000
#define IPPHASE1	0x2000000
#define IPPHASE2	0x4000000

// VPEAK
#define VPEAKVAL	0xFFFFFF
#define VPPHASE0	0x1000000
#define VPPHASE1	0x2000000
#define VPPHASE2	0x4000000

// STATUS0, MASK0
#define AEHF		0x1
#define FAEHF		0x2
#define REHF		0x4
#define FREHF		0x8
#define VAEHF		0x10
#define LENERGY		0x20
#define REVAPA		0x40
#define REVAPB		0x80
#define REVAPC		0x100
#define REVPSUM1	0x200
#define REVRPA		0x400
#define REVRPB		0x800
#define REVRPC		0x1000
#define REVPSUM2	0x2000
#define CF1			0x4000
#define CF2			0x8000
#define CF3			0x10000
#define DREADY		0x20000
#define REVPSUM3	0x40000

// STATUS1, MASK1
#define NLOAD		0x1
#define FNLOAD		0x2
#define VANLOAD		0x4
#define ZXTOVA		0x8
#define ZXTOVB		0x10
#define ZXTOVC		0x20
#define ZXTOIA		0x40
#define ZXTOIB		0x80
#define ZXTOIC		0x100
#define ZXVA		0x200
#define ZXVB		0x400
#define ZXVC		0x800
#define ZXIA		0x1000
#define ZXIB		0x2000
#define ZXIC		0x4000
#define RSTDONE		0x8000
#define SAG			0x10000
#define OI			0x20000
#define OV			0x40000
#define SEQERR		0x80000
#define MISMTCH		0x100000
#define PKI			0x800000
#define PKV			0x1000000

// PHSTATUS
#define OIPHASE0	0x4
#define OIPHASE1	0x8
#define OIPHASE2	0x10
#define OVPHASE0	0x200
#define OVPHASE1	0x400
#define OVPHASE2	0x800
#define VSPHASE0	0x1000
#define VSPHASE1	0x2000
#define VSPHASE2	0x4000

// PHNOLOAD
#define NLPHASE0	0x1
#define NLPHASE1	0x2
#define NLPHASE2	0x4
#define FNLPHASE0	0x8
#define FNLPHASE1	0x10
#define FNLPHASE2	0x20
#define VANLPHASE0	0x40
#define VANLPHASE1	0x80
#define VANLPHASE2	0x100

// COMPMODE
#define TERMSEL1_A	0x1
#define TERMSEL1_B	0x2
#define TERMSEL1_C	0x4
#define TERMSEL2_A	0x8
#define TERMSEL2_B	0x10
#define TERMSEL2_C	0x20
#define TERMSEL3_A	0x40
#define TERMSEL3_B	0x80
#define TERMSEL3_C	0x100
//#define ANGLE_UI	0x0
//#define ANGLE_UU    0x200//BIT_9
//#define ANGLE_II    0x400//BIT_10
#define ANGLE_DIS   0x600
#define VNOMAEN		0x800
#define VNOMBEN		0x1000
#define VNOMCEN		0x2000
#define SELFREQ		0x4000
#define CF1_TOTAL   (TERMSEL1_A | TERMSEL1_B | TERMSEL1_C)
#define CF2_TOTAL   (TERMSEL2_A | TERMSEL2_B | TERMSEL2_C)
#define CF3_TOTAL   (TERMSEL3_A | TERMSEL3_B | TERMSEL3_C)  

// GAIN
#define PGA1		0x7
#define PGA2		0x38
#define PGA3		0x1C0
// GAIN set values
#define PGA1_1		0x0
#define PGA1_2		0x1
#define PGA1_4		0x2
#define PGA1_8		0x3
#define PGA1_16		0x4
#define PGA2_1		0x0
#define PGA2_2		0x8
#define PGA2_4		0x10
#define PGA2_8		0x18
#define PGA2_16		0x20
#define PGA3_1		0x0
#define PGA3_2		0x40
#define PGA3_4		0x80
#define PGA3_8		0xC0
#define PGA3_16		0x100

// CFMODE
#define CF1SEL		0x7
#define CF2SEL		0x38
#define CF3SEL		0x1C0
#define CF1DIS		0x200
#define CF2DIS		0x400
#define CF3DIS		0x800
#define CF1LATCH	0x1000
#define CF2LATCH	0x2000
#define CF3LATCH	0x4000
#define RSTDONE		0x8000
#if defined(METER_THAILAN) || defined(_DT03MRF_PCB_PIC_AS8_S_V10_) || defined(_DT03MRF_PCB_PIC_AMS_S_V10_)
#if defined(ADE7858_SUPPORT)
    #define CF1_REACTIVE    0x0001
    #define CF2_ACTIVE      0x0000
    #define CF3_APPARENT    0x0080
#elif defined(ADE7880_SUPPORT)
    #define CF1_REACTIVE    0x0004 //4 la fundamental reactive
    #define CF2_ACTIVE      0x0000
    #define CF3_APPARENT    0x0080
#endif
#else
    #define CF1_ACTIVE      0x0000
    #define CF2_REACTIVE    0x0008
    #define CF3_APPARENT    0x0080
#endif
// APHCAL, BPHCAL, CPHCAL
#define PHCALVAL	0x3FF

// PHSIGN
#define AWSIGN		0x1
#define BWSIGN		0x2
#define CWSIGN		0x4
#define SUM1SIGN	0x8
#define AVARSIGN	0x10
#define BVARSIGN	0x20
#define CVARSIGN	0x40
#define SUM2SIGN	0x80
#define SUM3SIGN	0x100

// CONFIG
#define INTEN		0x1
#define SWAP		0x8
#define MOD1SHORT	0x10
#define MOD2SHORT	0x20
#define HSDCEN		0x40
#define SWRST		0x80
#define VTOIA		0x300
#define VTOIB		0xC00
#define VTOIC		0x3000
// VTOI set values
#define VTOIA_A		0x0
#define VTOIA_B		0x100
#define VTOIA_C		0x200
#define VTOIB_B		0x0
#define VTOIB_C		0x400
#define VTOIB_A		0x800
#define VTOIC_C		0x0
#define VTOIC_A		0x1000
#define VTOIC_B		0x2000

// MMODE
#define PERSEL		0x3
#define PEAKSEL0	0x4
#define PEAKSEL1	0x8
#define PEAKSEL2	0x10

// ACCMODE
#define WATTACC		0x3
#define VARACC		0xC
#define CONSEL		0x30
#define REVAPSEL	0x40
#define REVRPSEL	0x80

// LCYCMODE
#define LWATT		0x1
#define LVAR		0x2
#define LVA			0x4
#define ZXSEL0		0x8
#define ZXSEL1		0x10
#define ZXSEL2		0x20
#define RSTREAD		0x40

// HSDC_CFG
#define HCLK		0x1
#define HSIZE		0x2
#define HGAP		0x4
#define HXFER		0x18
#define HSAPOL		0x20

// LPOILVL
#define LPOIL		0x7
#define LPLINE		0xF8

// CONFIG2
#define EXTREFEN	0x1
#define I2C_LOCK	0x2

#if defined(ADE7858_SUPPORT)
#define   METER_CONST_5000    		25   
#define   METER_CONST_1000    		125   
#define   METER_CONST_25000    		25//5
#elif defined(ADE7880_SUPPORT)
#define   METER_CONST_5000    		4500//4489 //200 
#define   METER_CONST_1000    		125   
#define   METER_CONST_25000    		25//5
#endif

// <default calibration value>
#if defined(ADE7858_SUPPORT)
//phase A
#define     AIGAIN_C		15233333
#define     AVGAIN_C		30166666
#define     AIRMSOS_C		0
#define     AVRMSOS_C		0
#define     AWGAIN_C        5088888
#define     AVARGAIN_C      5088888
#define     AVAGAIN_C       5088888
#define     APHASE_C        52
#define     A_P_SCALE_C     10320
//phase B
#define     BIGAIN_C		15155555
#define     BVGAIN_C		30222222
#define     BIRMSOS_C		0
#define     BVRMSOS_C		0
#define     BWGAIN_C        5056666
#define     BVARGAIN_C      5056666
#define     BVAGAIN_C       5056666
#define     BPHASE_C        52
#define     B_P_SCALE_C     10320
//phase C
#define     CIGAIN_C		15200000
#define     CVGAIN_C		30188888
#define     CIRMSOS_C		0
#define     CVRMSOS_C		0
#define     CWGAIN_C        5055555
#define     CVARGAIN_C      5055555
#define     CVAGAIN_C       5055555
#define     CPHASE_C        52
#define     C_P_SCALE_C     10320
//Neutral
#define   NIGAIN_C      0
#define   NIRMSOS_C       0
#elif defined(ADE7880_SUPPORT)
//phase A
#define     AIGAIN_C		15970803 //15233333
#define     AVGAIN_C		31130729 //30166666
#define     AIRMSOS_C		0
#define     AVRMSOS_C		0
#define     AWGAIN_C        14193276
#define     AVARGAIN_C      14193276
#define     AVAGAIN_C       14193276
#define     APHASE_C        52
#define     A_P_SCALE_C     6530//10320 so nay la tinh toan theo gia tri thuc te doc ra
//phase B
#define     BIGAIN_C		15970803
#define     BVGAIN_C		31130729
#define     BIRMSOS_C		0
#define     BVRMSOS_C		0
#define     BWGAIN_C        14193276
#define     BVARGAIN_C      14193276
#define     BVAGAIN_C       14193276
#define     BPHASE_C        52
#define     B_P_SCALE_C     6530//10320 so nay la tinh toan theo gia tri thuc te doc ra
//phase C
#define     CIGAIN_C		16015091
#define     CVGAIN_C		31453719
#define     CIRMSOS_C		0
#define     CVRMSOS_C		0
#define     CWGAIN_C        14193276
#define     CVARGAIN_C      14193276
#define     CVAGAIN_C       14193276
#define     CPHASE_C        49
#define     C_P_SCALE_C     6530//10320 so nay la tinh toan theo gia tri thuc te doc ra
//Neutral
#define   NIGAIN_C      0
#define   NIRMSOS_C       0
#endif

// <auto offset>
#define     AVOL_HIGH   20000
#define     BVOL_HIGH   20000
#define     CVOL_HIGH   20000
#define     AVOL_LOW    8000
#define     BVOL_LOW    8000
#define     CVOL_LOW    8000
#define     AVOL_NONE   1500
#define     BVOL_NONE   1500
#define     CVOL_NONE   1500

//da dinh nghia o proccess.h
//#define RESOLUTION_P            10000 //dinh nghia do phan giai he so cong suat
//#define LIMIT_RESOLUTION_P      1000

//cac thanh ghi cau hinh
#if defined(ADE7858_SUPPORT)
#ifdef METER_3WIRE
#define   ACCMODE_C     0x10
#else
#define   ACCMODE_C     0
#endif
#elif defined(ADE7880_SUPPORT)
#ifdef METER_3WIRE
#define   ACCMODE_C     0x90 //BIT7 default = 1
#else
#define   ACCMODE_C     0x80 //BIT7 default = 1
#endif
#endif

#define   VANOLOAD_C	25
#define   APNOLOAD_C	25
#define   VARNOLOAD_C	25
#define   VATHR1_C		0x01
#define   VATHR0_C		0xF3E709
#define   WTHR1_C		0x01
#define   WTHR0_C		0xF3E709
#define   VARTHR1_C		0x01
#define   VARTHR0_C		0xF3E709
#define   VLEVEL_C		31034483//402885
#define   VNOM_C        1996737



#define   EXTERREFEN            0x01
#define   INTERREFEN            0xFE
#define   PORT_LOCK             0x02
#define   WATT2CF             	0xFFF8
#define   VAR2CF             	0xFFF9
#define   FWATT2CF             	0xFFFB
#define   CF1EN             	0xFDFF
#define   CF2EN             	0xFBFF
#define   CF3EN             	0xF7FF
#define   Angle_Phase           0xF3FF
#define   Angle_Voltage         0xF7FF
#define   Angle_Current         0xFBFF
#define   Period_A           	0xFC
#define   Period_B           	0xFD
#define   Period_C           	0xFE

#define PHASE_ABC       0x00
#define PHASE_BAC       0x01
#define PHASE_BCA       0x02
#define PHASE_ACB       0x03
#define PHASE_CAB       0x04
#define PHASE_CBA       0x05



//cs
#define ade7858_cs_dir_output()				_ade7858_cs_dir_output()
#define ade7858_cs_low()					_ade7858_cs_low()
#define ade7858_cs_high()                   _ade7858_cs_high()
#define ade7858_cs_tgl()                    _ade7858_cs_tgl()
//sdi
#define ade7858_sdi_dir_output()            _ade7858_sdi_dir_output()
#define ade7858_sdi_output_high()           _ade7858_sdi_output_high()
#define ade7858_sdi_output_low()            _ade7858_sdi_output_low()
//clk
#define ade7858_sclk_dir_output()           _ade7858_sclk_dir_output()
#define ade7858_sclk_output_high()          _ade7858_sclk_output_high()
#define ade7858_sclk_output_low()           _ade7858_sclk_output_low()
//sdo
#define ade7858_sdo_dir_input()             _ade7858_sdo_dir_input()
#define ade7858_sdo_input()                 _ade7858_sdo_input()
//reset
#define ade_reset_dir_output()              _ade_reset_dir_output()//TRISBbits.TRISB10 = 0
#define ade_reset_output_low()              _ade_reset_output_low()//LATBbits.LATB10 = 0
#define ade_reset_output_high()             _ade_reset_output_high()//LATBbits.LATB10 = 1

#ifdef ADE7858_POWER_CONTROL_SUPPORT
//pm0
#define ade7858_pm0_dir_output()            _ade7858_pm0_dir_output()
#define ade7858_pm0_output_high()           _ade7858_pm0_output_high()
#define ade7858_pm0_output_low()            _ade7858_pm0_output_low()
//pm1
#define ade7858_pm1_dir_output()            _ade7858_pm1_dir_output()
#define ade7858_pm1_output_high()           _ade7858_pm1_output_high()
#define ade7858_pm1_output_low()            _ade7858_pm1_output_low()
#endif


typedef enum {ANGLE_UI = 0x01, ANGLE_UU = 0x02, ANGLE_II = 0x03} ANGLE_MODE;
typedef struct {
    struct{
        unsigned long vrmsA;
        unsigned long vrmsB;
        unsigned long vrmsC;
        unsigned long irmsA;
        unsigned long irmsB;
        unsigned long irmsC;
        long active_powerA; //active power A
        long active_powerB; //active power B
        long active_powerC; //active power C  
        long active_power; //total  
        long reactive_powerA; //reactive power A
        long reactive_powerB; //reactive power B
        long reactive_powerC; //reactive power C    
        long reactive_power; // total
        long apparent_powerA; //active power A
        long apparent_powerB; //active power B
        long apparent_powerC; //active power C   
        int power_factorA; //active power A
        int power_factorB; //active power B
        int power_factorC; //active power C  
        float phase_angleA;
        float phase_angleB;
        float phase_angleC;
        #if defined(ADE7858_SUPPORT)
        long freq;
        #elif defined(ADE7880_SUPPORT)
        long freqA;
        long freqB;
        long freqC;
        #endif
        unsigned long line_cycles;
        long temperature;
    } measure;
    struct {
        struct{
            int32_t i_gain;
            int32_t v_gain;
            int32_t irms_gain;
            int32_t vrms_gain;   
            #if defined(ADE7858_SUPPORT)
            int32_t watt_gain;
            int32_t var_gain;
            int32_t va_gain;
            #elif defined(ADE7880_SUPPORT)
            int32_t p_gain;
            int32_t var_gain; //ko dung
            int32_t va_gain; //ko dung
            #endif
            int16_t phase;
            int32_t dips_scale; //he so hien thi active power
        }pA, pB, pC;
        struct{
            int32_t i_gain;          
            int32_t irms_gain;
        }pN;
    }calib;
    struct {
        uint8_t register_error;
        uint8_t spi_error;
        uint8_t pA_set_offset;
        uint8_t pB_set_offset;
        uint8_t pC_set_offset;
        uint8_t angle_UU_mode; //==1 neu dang do goc U^U, ==0 neu dang do dem U^I
    }flags;
    ANGLE_MODE angle_mode;
    uint16_t addr_reg;
    uint8_t  reg_uint8;
    uint16_t reg_uint16;
    uint32_t reg_uint32;     
    uint32_t reg_value;    // Kiem soat lai va xoa 
} ADE_STRUCT;

extern ADE_STRUCT ade7858;
extern unsigned char dread1,dread2,dread3;

void ade7858_write_SUBCMD_RW_ADE_REGISTER(uint8_t module);
//void ade7858_write_SUBCMD_RW_ADE_REGISTER(uint8_t *rx_buffer, uint8_t module);
void ade7858_read_SUBCMD_RW_ADE_REGISTER(uint8_t module);
#if defined(_DT03M_RF_)
BOOL ade7858_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase, uint8_t length);
BOOL ade7858_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase);
#elif defined(_ROUTER3P_)
BOOL ade7858_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module);
BOOL ade7858_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module);
#endif
extern BOOL ade7858_load_calib_value(void);
extern void spi_init();
extern void ade7858_init(void);
extern BOOL ade7858_config(void);

extern BOOL ade7858_write_1byte(uint16_t address,uint8_t dwrite);
extern BOOL ade7858_write_2byte(uint16_t address,uint16_t dwrite);
extern BOOL ade7858_write_3byte(uint16_t address,uint32_t dwrite);
extern BOOL ade7858_write_4byte(uint16_t address,uint32_t dwrite);

extern uint8_t ade7858_read_1byte(uint16_t address);
extern uint16_t ade7858_read_2byte(uint16_t address);
extern uint32_t ade7858_read_3byte(uint16_t address);
extern uint32_t ade7858_read_4byte(uint16_t address);

extern void ade7858_read(uint16_t address,uint8_t bytecount);
extern void measure_task(void);
extern void measure_vrms(void);
extern void measure_irms(void);
extern void measure_activepower(void);
extern void measure_apparentpower(void);
extern void measure_reactivepower(void);
extern void measure_freq(void);
extern void measure_temperature(void);
extern void measure_line_cycles(void);
extern void check_reverse_active_power(void);
extern void check_reverse_reactive_power(void);
unsigned char ade7858_spi_get_byte(void);
void ade7858_angle_measure_select(uint8_t type);

extern BOOL ade7858_set_calib_value(void);
extern BOOL ade7858_set_calib_value_phaseA(BOOL wr);
extern BOOL ade7858_set_calib_value_phaseB(BOOL wr);
extern BOOL ade7858_set_calib_value_phaseC(BOOL wr);
extern BOOL ade7858_set_calib_value_phaseN(BOOL wr);

extern void ade7858_get_calib_value_phaseA(void);
extern void ade7858_get_calib_value_phaseB(void);
extern void ade7858_get_calib_value_phaseC(void);
extern void ade7858_get_calib_value_phaseN(void);

void ade7858_spi_send_byte(uint8_t value);
void ade7858_spi_send_3byte(uint16_t value);
void ade7878_go_normal_mode();
void ade7878_go_reduce_power_mode();
void ade7878_go_low_power_mode();
void ade7878_go_sleep_mode();
extern void ade7858_reset();
// <get dtp buffer, se chuyen sang dtp.c>
//uint8_t _dtp_get_8bit(uint8_t index, uint8_t module);
//uint16_t _dtp_get_16bit(uint8_t index, uint8_t module);
//uint32_t _dtp_get_32bit(uint8_t index, uint8_t module);
//void _dtp_get_buffer(uint8_t *source, uint8_t index, uint16_t length,uint8_t module);
#endif /* ADE7858_H_ */
