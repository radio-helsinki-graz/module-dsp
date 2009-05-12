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

int RoutingFrom[7*NUMBEROFOBJECTS] =
{
//GainInput from
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

//MonoInput from
//  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,

//EQInput from
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,

//DynamicsInput from
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,

//McASPAInput from
    5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,

//McASPBInput from
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,

//MeterInput from
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

//LevelInput from
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
};


float InputSampleData[7*NUMBEROFOBJECTS];
float *GainInput            = &InputSampleData[0];
//float *MonoInput          = &InputSampleData[1*NUMBEROFOBJECTS];
float *EQInput              = &InputSampleData[1*NUMBEROFOBJECTS];
float *DynamicsInput        = &InputSampleData[2*NUMBEROFOBJECTS];
float *McASPAInput          = &InputSampleData[3*NUMBEROFOBJECTS];
float *McASPBInput          = &InputSampleData[4*NUMBEROFOBJECTS];
float *MeterInput           = &InputSampleData[5*NUMBEROFOBJECTS];
float *LevelInput           = &InputSampleData[6*NUMBEROFOBJECTS];

float OutputSampleData[6*NUMBEROFOBJECTS];
float *McASPAOutput     = &OutputSampleData[0];
float *McASPBOutput     = &OutputSampleData[1*NUMBEROFOBJECTS];
float *GainOutput       = &OutputSampleData[2*NUMBEROFOBJECTS];
//float *MonoOutput         = &OutputSampleData[3*NUMBEROFOBJECTS];
float *EQBand6Output    = &OutputSampleData[3*NUMBEROFOBJECTS];
float *DynamicsOutput   = &OutputSampleData[4*NUMBEROFOBJECTS];
float *LevelOutput      = &OutputSampleData[5*NUMBEROFOBJECTS];

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
//int MonoBInputFrom[NUMBEROFMONOOBJECTS];
//float MonoInputAFactor[NUMBEROFMONOOBJECTS];
//float MonoInputBFactor[NUMBEROFMONOOBJECTS];
//float Update_MonoInputAFactor[NUMBEROFMONOOBJECTS];
//float Update_MonoInputBFactor[NUMBEROFMONOOBJECTS];

// EQ
const float EQCoefficients[NUMBEROFEQOBJECTS*NUMBEROFEQBANDS*5];
float EQDelayBuffer[NUMBEROFEQOBJECTS*NUMBEROFEQBANDS*2];
float EQBand1Output[NUMBEROFEQOBJECTS];
float EQBand2Output[NUMBEROFEQOBJECTS];
float EQBand3Output[NUMBEROFEQOBJECTS];
float EQBand4Output[NUMBEROFEQOBJECTS];
float EQBand5Output[NUMBEROFEQOBJECTS];

//Dynamics
int DynamicsStereoInputFrom[NUMBEROFDYNAMICSOBJECTS];

float DynamicsRMS[NUMBEROFDYNAMICSOBJECTS];
float CompressorReductionLevel[NUMBEROFDYNAMICSOBJECTS];

float DynamicsOriginalFactor[NUMBEROFDYNAMICSOBJECTS];
float DynamicsProcessedFactor[NUMBEROFDYNAMICSOBJECTS];

float AttackFactor = 0.9978;
float ReleaseFactor = 1.0000217;
float Threshold[NUMBEROFDYNAMICSOBJECTS];

float MakeUpGain = 10;

//volatile float DuckReleaseFactor = 0.9999;
//float DuckLevelFactor[NUMBEROFDYNAMICSOBJECTS];
//float Update_DuckLevelFactor[NUMBEROFDYNAMICSOBJECTS];

//Leveler
float LevelFactor[NUMBEROFLEVELOBJECTS];
float Update_LevelFactor[NUMBEROFLEVELOBJECTS];

float MeterPPM[NUMBEROFMETEROBJECTS];
float MeterVU[NUMBEROFMETEROBJECTS];
float PhaseRMS[NUMBEROFMETEROBJECTS/2];

float PhaseRelease = 0.0002;

void InitializeProcessing();
void Processing(signed int *InputBuffer, signed int *OutputBuffer);

#endif

