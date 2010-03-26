#ifndef AXUMMODULEPROCESSING_H
#define AXUMMODULEPROCESSING_H

#define NUMBEROFINPUTCHANNELS       64
#define NUMBEROFOUTPUTCHANNELS      64

#define NUMBEROFOBJECTS 32

#define NUMBEROFGAINOBJECTS         NUMBEROFOBJECTS
#define NUMBEROFMONOOBJECTS         NUMBEROFOBJECTS
#define NUMBEROFEQOBJECTS           NUMBEROFOBJECTS
    #define NUMBEROFEQBANDS         6
#define NUMBEROFDYNAMICSOBJECTS     NUMBEROFOBJECTS
#define NUMBEROFLEVELOBJECTS        NUMBEROFOBJECTS
#define NUMBEROFMETEROBJECTS        NUMBEROFOBJECTS

int RoutingFrom[8*NUMBEROFOBJECTS] =
{
//GainInput from
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,

//MonoInput from
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

//EQInput from
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,

//DynamicsInput from
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,

//McASPAInput from
    6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,

//McASPBInput from
    5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,

//MeterInput from
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

//LevelInput from
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
};


float InputSampleData[8*NUMBEROFOBJECTS];
float *GainInput            = &InputSampleData[0];
float *MonoInput          	= &InputSampleData[1*NUMBEROFOBJECTS];
float *EQInput              = &InputSampleData[2*NUMBEROFOBJECTS];
float *DynamicsInput        = &InputSampleData[3*NUMBEROFOBJECTS];
float *McASPAInput          = &InputSampleData[4*NUMBEROFOBJECTS];
float *McASPBInput          = &InputSampleData[5*NUMBEROFOBJECTS];
float *MeterInput           = &InputSampleData[6*NUMBEROFOBJECTS];
float *LevelInput           = &InputSampleData[7*NUMBEROFOBJECTS];

float OutputSampleData[7*NUMBEROFOBJECTS];
float *McASPAOutput     = &OutputSampleData[0];
float *McASPBOutput     = &OutputSampleData[1*NUMBEROFOBJECTS];
float *GainOutput       = &OutputSampleData[2*NUMBEROFOBJECTS];
float *MonoOutput       = &OutputSampleData[3*NUMBEROFOBJECTS];
float *EQBand6Output    = &OutputSampleData[4*NUMBEROFOBJECTS];
float *DynamicsOutput   = &OutputSampleData[5*NUMBEROFOBJECTS];
float *LevelOutput      = &OutputSampleData[6*NUMBEROFOBJECTS];

volatile float SmoothFactor = 1-0.002;
volatile float PPMReleaseFactor;
volatile float VUReleaseFactor;
volatile float RMSReleaseFactor;

//gain (and LC)
//float GainInput[NUMBEROFGAINOBJECTS];
float InputGainFactor[NUMBEROFGAINOBJECTS];
float Update_InputGainFactor[NUMBEROFGAINOBJECTS];
float FilterDelayBuffer[NUMBEROFGAINOBJECTS*2];
const float FilterCoefficients[NUMBEROFGAINOBJECTS*5];

//Mono
int MonoBInputFrom[NUMBEROFMONOOBJECTS];
float MonoInputAFactor[NUMBEROFMONOOBJECTS];
float MonoInputBFactor[NUMBEROFMONOOBJECTS];
float Update_MonoInputAFactor[NUMBEROFMONOOBJECTS];
float Update_MonoInputBFactor[NUMBEROFMONOOBJECTS];

// EQ
const float EQCoefficients[NUMBEROFEQOBJECTS*NUMBEROFEQBANDS*5];
float EQDelayBuffer[NUMBEROFEQOBJECTS*NUMBEROFEQBANDS*2];
float EQBand1Output[NUMBEROFEQOBJECTS];
float EQBand2Output[NUMBEROFEQOBJECTS];
float EQBand3Output[NUMBEROFEQOBJECTS];
float EQBand4Output[NUMBEROFEQOBJECTS];
float EQBand5Output[NUMBEROFEQOBJECTS];

//Dynamics
float DynamicsRMS[NUMBEROFDYNAMICSOBJECTS];
volatile float DownwardExpanderRMS[NUMBEROFDYNAMICSOBJECTS];
volatile double DownwardExpanderAverage[NUMBEROFDYNAMICSOBJECTS];
float CompressorReductionLevel[NUMBEROFDYNAMICSOBJECTS];
int DynamicsOn[NUMBEROFDYNAMICSOBJECTS];

float DynamicsOriginalFactor[NUMBEROFDYNAMICSOBJECTS];
float DynamicsProcessedFactor[NUMBEROFDYNAMICSOBJECTS];
float DownwardExpanderFactor[NUMBEROFDYNAMICSOBJECTS];
float CompressionData[NUMBEROFDYNAMICSOBJECTS];

float AttackFactor = 0.9978;
float ReleaseFactor = 1.0000217;
float AGCThreshold[NUMBEROFDYNAMICSOBJECTS];
float MakeupGain[NUMBEROFDYNAMICSOBJECTS];
float InverseMakeupGain[NUMBEROFDYNAMICSOBJECTS];
float DownwardExpanderThreshold[NUMBEROFDYNAMICSOBJECTS];
float DownwardExpanderLevel[NUMBEROFDYNAMICSOBJECTS];
float DownwardExpanderReleaseFactor = 0.9997;
float DownwardExpanderAttackFactor = 1.00099;//1.002

//Leveler
float LevelFactor[NUMBEROFLEVELOBJECTS];
float Update_LevelFactor[NUMBEROFLEVELOBJECTS];

float MeterPPM[NUMBEROFMETEROBJECTS];
float MeterVU[NUMBEROFMETEROBJECTS];
float PhaseRMS[NUMBEROFMETEROBJECTS/2];

float PhaseRelease = 0.0002;

#define RMS_LENGTH 256
struct delay_struct
{
	float Buffer[RMS_LENGTH];
	unsigned int Ptr;
} RMSDelay[NUMBEROFDYNAMICSOBJECTS];


void InitializeProcessing();
void Processing(signed int *InputBuffer, signed int *OutputBuffer);

#endif

