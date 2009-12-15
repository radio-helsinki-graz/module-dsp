#include "AxumModuleProcessing.h"

void InitializeProcessing()
{
	int cntChannel;
	int cntBand;
	int cntSample;

	for (cntChannel=0; cntChannel<NUMBEROFGAINOBJECTS; cntChannel++)
	{
		float *Temp = (float *)FilterCoefficients;
		Temp[(cntChannel*5)+0] = 0;
		Temp[(cntChannel*5)+1] = 0;
		Temp[(cntChannel*5)+2] = 1;
		Temp[(cntChannel*5)+3] = 0;
		Temp[(cntChannel*5)+4] = 0;

		FilterDelayBuffer[(cntChannel*2)+0] = 0;
		FilterDelayBuffer[(cntChannel*2)+1] = 0;

		InputGainFactor[cntChannel] = 0;
		Update_InputGainFactor[cntChannel] = 1;
	}

/*	for (cntChannel=0; cntChannel<NUMBEROFMONOOBJECTS; cntChannel++)
	{
		MonoBInputFrom[cntChannel] = cntChannel^1;

		MonoInputAFactor[cntChannel] = 0;
		MonoInputBFactor[cntChannel] = 0;

		Update_MonoInputAFactor[cntChannel] = 1;
		Update_MonoInputBFactor[cntChannel] = 1;
	}*/

	for (cntChannel=0; cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float *Temp = (float *)EQCoefficients;
		for (cntBand=0; cntBand<NUMBEROFEQBANDS; cntBand++)
		{
			Temp[(cntChannel*NUMBEROFEQBANDS*5)+(cntBand*5)+0] = 0;
			Temp[(cntChannel*NUMBEROFEQBANDS*5)+(cntBand*5)+1] = 0;
			Temp[(cntChannel*NUMBEROFEQBANDS*5)+(cntBand*5)+2] = 1;
			Temp[(cntChannel*NUMBEROFEQBANDS*5)+(cntBand*5)+3] = 0;
			Temp[(cntChannel*NUMBEROFEQBANDS*5)+(cntBand*5)+4] = 0;

			EQDelayBuffer[(cntChannel*NUMBEROFEQBANDS*2)+(cntBand*2)+0] = 0;
			EQDelayBuffer[(cntChannel*NUMBEROFEQBANDS*2)+(cntBand*2)+1] = 0;
		}

		EQBand1Output[cntChannel] = 0;
		EQBand2Output[cntChannel] = 0;
		EQBand3Output[cntChannel] = 0;
		EQBand4Output[cntChannel] = 0;
		EQBand5Output[cntChannel] = 0;
		EQBand6Output[cntChannel] = 0;
	}
	for (cntChannel=0; cntChannel<NUMBEROFEQOBJECTS*NUMBEROFEQBANDS*2; cntChannel++)
	{
		EQDelayBuffer[cntChannel] = 0;
	}

	for (cntChannel=0;cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
	{
		CompressorReductionLevel[cntChannel] = 1;
		AGCThreshold[cntChannel] = 21474836.48;
		DownwardExpanderThreshold[cntChannel] = 21474836.48;

		DynamicsOriginalFactor[cntChannel] = 1;
		DynamicsProcessedFactor[cntChannel] = 0;
		CompressionData[cntChannel] = 0.1;
		DynamicsOn[cntChannel] = 0;

		DownwardExpanderFactor[cntChannel] = 1;
		DownwardExpanderAverage[cntChannel] = 0;
	}

	for (cntChannel=0;cntChannel<NUMBEROFLEVELOBJECTS; cntChannel++)
	{
		LevelFactor[cntChannel] = 0;
		Update_LevelFactor[cntChannel] = 1;
	}

	for (cntChannel=0;cntChannel<NUMBEROFMETEROBJECTS/2; cntChannel++)
	{
		PhaseRMS[cntChannel] = 0.01;
	}

	PPMReleaseFactor = 0.99980814;
	VUReleaseFactor = 0.9;
	RMSReleaseFactor = 0.99956109;

	for (cntChannel=0;cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
	{
		RMSDelay[cntChannel].Ptr = 0;
		for (cntSample=0; cntSample<RMS_LENGTH; cntSample++)
		{
			RMSDelay[cntChannel].Buffer[cntSample] = 0;
		}
	}
}

void Processing(signed int *InputBuffer, signed int *OutputBuffer)
{
	int cntChannel;
	int cntSample;
	int cntDelaySample;
	int cntInputObject;

	const float RMSFactor = RMSReleaseFactor;

	for (cntChannel=0;cntChannel<NUMBEROFOUTPUTCHANNELS; cntChannel++)
	{
		McASPAOutput[cntChannel] = InputBuffer[cntChannel];
	}

	for (cntInputObject=0;cntInputObject<6;cntInputObject++)
	{
	 	for (cntChannel=0;cntChannel<NUMBEROFOBJECTS; cntChannel++)
		{
			int OutputObject = RoutingFrom[cntInputObject*NUMBEROFOBJECTS+cntChannel];
			InputSampleData[cntInputObject*NUMBEROFOBJECTS+cntChannel] = OutputSampleData[OutputObject*NUMBEROFOBJECTS+cntChannel];
		}
	}

	//Filter and Gain
	{
		cntSample = 0;
		cntDelaySample = 0;
	 	for (cntChannel=0;cntChannel<NUMBEROFGAINOBJECTS; cntChannel++)
		{
			const float FactorX = SmoothFactor;
			const float FactorY = 1-SmoothFactor;
			float d0 = FilterDelayBuffer[cntDelaySample];
			float d1 = FilterDelayBuffer[cntDelaySample+1];
			InputGainFactor[cntChannel] = (FactorX*InputGainFactor[cntChannel])+(Update_InputGainFactor[cntChannel]*FactorY);

			FilterDelayBuffer[cntDelaySample+1] = FilterDelayBuffer[cntDelaySample];
			FilterDelayBuffer[cntDelaySample] =	(GainInput[cntChannel]) +
												(d0*FilterCoefficients[cntSample]) +
												(d1*FilterCoefficients[cntSample+1]);

			GainOutput[cntChannel] = (FilterDelayBuffer[cntDelaySample]*FilterCoefficients[cntSample+2]) +
										(d0*FilterCoefficients[cntSample+3]) +
										(d1*FilterCoefficients[cntSample+4]);

			GainOutput[cntChannel] *= InputGainFactor[cntChannel];

			cntDelaySample += 2;
			cntSample += 5;
		}
	}

	//Mono
/*	{
		const float FactorX = SmoothFactor;
		const float FactorY = 1-SmoothFactor;
		for (cntChannel=0; cntChannel<NUMBEROFMONOOBJECTS; cntChannel++)
		{
			MonoInputAFactor[cntChannel] = (MonoInputAFactor[cntChannel]*FactorX)+(FactorY*Update_MonoInputAFactor[cntChannel]);
			MonoInputBFactor[cntChannel] = (MonoInputBFactor[cntChannel]*FactorX)+(FactorY*Update_MonoInputBFactor[cntChannel]);
			MonoOutput[cntChannel] = MonoInput[cntChannel]*MonoInputAFactor[cntChannel]+ MonoInput[MonoBInputFrom[cntChannel]]*MonoInputBFactor[cntChannel];
		}
	}*/

	// EQ
	//a0-a2 = zero / b0-b2 = pole
	//Coefficient[0] = -b1/b0; <- backward
	//Coefficient[1] = -b2/b0; <- backward
	//Coefficient[2] = a0/b0;  <- forward
	//Coefficient[3] = a1/b0;  <- forward
	//Coefficient[4] = a2/b0;  <- forward

	//Band 1
#if NUMBEROFEQBANDS >= 1
	cntSample = 0;
	cntDelaySample = 0;
 	for (cntChannel=0;cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float d0 = EQDelayBuffer[cntDelaySample];
		float d1 = EQDelayBuffer[cntDelaySample+1];

		EQDelayBuffer[cntDelaySample+1] = EQDelayBuffer[cntDelaySample];
		EQDelayBuffer[cntDelaySample] = EQInput[cntChannel] +
										(d0*EQCoefficients[cntSample]) +
										(d1*EQCoefficients[cntSample+1]);

		EQBand1Output[cntChannel] = (EQDelayBuffer[cntDelaySample]*EQCoefficients[cntSample+2]) +
									(d0*EQCoefficients[cntSample+3]) +
									(d1*EQCoefficients[cntSample+4]);
		cntDelaySample += 2;
		cntSample += 5;
	}
#endif

	//Band 2
#if NUMBEROFEQBANDS >= 2
 	for (cntChannel=0;cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float d0 = EQDelayBuffer[cntDelaySample];
		float d1 = EQDelayBuffer[cntDelaySample+1];

		EQDelayBuffer[cntDelaySample+1] = EQDelayBuffer[cntDelaySample];
		EQDelayBuffer[cntDelaySample] = EQBand1Output[cntChannel] +
										(d0*EQCoefficients[cntSample]) +
										(d1*EQCoefficients[cntSample+1]);

		EQBand2Output[cntChannel] = (EQDelayBuffer[cntDelaySample]*EQCoefficients[cntSample+2]) +
									(d0*EQCoefficients[cntSample+3]) +
									(d1*EQCoefficients[cntSample+4]);
		cntDelaySample += 2;
		cntSample += 5;
	}
#endif

	//Band 3
#if NUMBEROFEQBANDS >= 3
 	for (cntChannel=0;cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float d0 = EQDelayBuffer[cntDelaySample];
		float d1 = EQDelayBuffer[cntDelaySample+1];

		EQDelayBuffer[cntDelaySample+1] = EQDelayBuffer[cntDelaySample];
		EQDelayBuffer[cntDelaySample] = 	EQBand2Output[cntChannel] +
									(d0*EQCoefficients[cntSample]) +
									(d1*EQCoefficients[cntSample+1]);

		EQBand3Output[cntChannel] = (EQDelayBuffer[cntDelaySample]*EQCoefficients[cntSample+2]) +
									(d0*EQCoefficients[cntSample+3]) +
									(d1*EQCoefficients[cntSample+4]);
		cntDelaySample += 2;
		cntSample += 5;
	}
#endif

	//Band 4
#if NUMBEROFEQBANDS >= 4
 	for (cntChannel=0;cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float d0 = EQDelayBuffer[cntDelaySample];
		float d1 = EQDelayBuffer[cntDelaySample+1];

		EQDelayBuffer[cntDelaySample+1] = EQDelayBuffer[cntDelaySample];
		EQDelayBuffer[cntDelaySample] = EQBand3Output[cntChannel] +
										(d0*EQCoefficients[cntSample]) +
										(d1*EQCoefficients[cntSample+1]);

		EQBand4Output[cntChannel] = (EQDelayBuffer[cntDelaySample]*EQCoefficients[cntSample+2]) +
									(d0*EQCoefficients[cntSample+3]) +
									(d1*EQCoefficients[cntSample+4]);
		cntDelaySample += 2;
		cntSample += 5;
	}
#endif

	//Band 5
#if NUMBEROFEQBANDS >= 5
 	for (cntChannel=0;cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float d0 = EQDelayBuffer[cntDelaySample];
		float d1 = EQDelayBuffer[cntDelaySample+1];

		EQDelayBuffer[cntDelaySample+1] = EQDelayBuffer[cntDelaySample];
		EQDelayBuffer[cntDelaySample] = EQBand4Output[cntChannel] +
										(d0*EQCoefficients[cntSample]) +
										(d1*EQCoefficients[cntSample+1]);

		EQBand5Output[cntChannel] = (EQDelayBuffer[cntDelaySample]*EQCoefficients[cntSample+2]) +
									(d0*EQCoefficients[cntSample+3]) +
									(d1*EQCoefficients[cntSample+4]);
		cntDelaySample += 2;
		cntSample += 5;
	}
#endif

	//Band 6
#if NUMBEROFEQBANDS >= 6
 	for (cntChannel=0;cntChannel<NUMBEROFEQOBJECTS; cntChannel++)
	{
		float d0 = EQDelayBuffer[cntDelaySample];
		float d1 = EQDelayBuffer[cntDelaySample+1];

		EQDelayBuffer[cntDelaySample+1] = EQDelayBuffer[cntDelaySample];
		EQDelayBuffer[cntDelaySample] = EQBand5Output[cntChannel] +
										(d0*EQCoefficients[cntSample]) +
										(d1*EQCoefficients[cntSample+1]);

		EQBand6Output[cntChannel] = (EQDelayBuffer[cntDelaySample]*EQCoefficients[cntSample+2]) +
									(d0*EQCoefficients[cntSample+3]) +
									(d1*EQCoefficients[cntSample+4]);
		cntDelaySample += 2;
		cntSample += 5;
	}
#endif

	//Dynamics
	{
		float rms_sample;
		float ProcessedSample[NUMBEROFDYNAMICSOBJECTS];
		float MaxSample[NUMBEROFDYNAMICSOBJECTS];

/**********************/
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
		{
			float PairedSample = _fabsf(DynamicsInput[cntChannel^0x01]);
			DynamicsOutput[cntChannel] = DynamicsInput[cntChannel];
			MaxSample[cntChannel] = _fabsf(DynamicsInput[cntChannel]);
			if (PairedSample > MaxSample[cntChannel])
				MaxSample[cntChannel] = PairedSample;
		}

		//Expander
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel+=2)
		{
			struct delay_struct *delay_line = &RMSDelay[cntChannel];
			rms_sample = MaxSample[cntChannel];
			rms_sample = (rms_sample*rms_sample)/(RMS_LENGTH*4);
			DownwardExpanderAverage[cntChannel]	+= (rms_sample-delay_line->Buffer[delay_line->Ptr]);
			delay_line->Buffer[delay_line->Ptr] = rms_sample;
			delay_line->Ptr++;
			if (delay_line->Ptr>(RMS_LENGTH-1))
			{
				delay_line->Ptr = 0;
			}

			if (DownwardExpanderAverage[cntChannel] >= 0)
			{
				DownwardExpanderRMS[cntChannel] = _rcpsp(_rsqrsp(DownwardExpanderAverage[cntChannel]));
			}
		}
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel+=2)
		{
			DownwardExpanderLevel[cntChannel] = 1;
			if (DownwardExpanderRMS[cntChannel]<DownwardExpanderThreshold[cntChannel])
			{
				float Diff = 1-(DownwardExpanderRMS[cntChannel]*_rcpsp(DownwardExpanderThreshold[cntChannel]));
				DownwardExpanderLevel[cntChannel] = 1-(Diff*1.05);
				if (DownwardExpanderLevel[cntChannel]<0)
				{
					DownwardExpanderLevel[cntChannel] = 0;
				}
			}
		}
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel+=2)
		{
			float Offset = DownwardExpanderLevel[cntChannel]-DownwardExpanderFactor[cntChannel];
			if (Offset<0)
			{
				DownwardExpanderFactor[cntChannel] *= DownwardExpanderReleaseFactor;
				if (DownwardExpanderFactor[cntChannel]<0.0125)
				{
					DownwardExpanderFactor[cntChannel] = 0.0125;
				}
	        }
			else
			{
				if (Offset > 0.00020)//0.00005
				{
					Offset = 0.00020;
				}
				DownwardExpanderFactor[cntChannel] += Offset;
	        }
		}
		//Now do all channels again
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
		{
			DynamicsOutput[cntChannel] *= DownwardExpanderFactor[cntChannel&0xFE];
		}

		// limiter for Compression thing....
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
		{
			float AbsoluteAudio = _fabsf(DynamicsOutput[cntChannel]);
			DynamicsRMS[cntChannel] *= RMSFactor;
			if (AbsoluteAudio>DynamicsRMS[cntChannel])
			 	DynamicsRMS[cntChannel] = AbsoluteAudio;

			AbsoluteAudio = _fabsf(DynamicsOutput[cntChannel^0x01]);
			if (AbsoluteAudio>DynamicsRMS[cntChannel])
			 	DynamicsRMS[cntChannel] = AbsoluteAudio;
		}

		//Calculate Compression factor
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel+=2)
		{
			float AbsoluteThreshold = AGCThreshold[cntChannel];
			//Check Threshold
			if (DynamicsRMS[cntChannel]<AbsoluteThreshold)
			{	//Release
				//CompressionData[cntChannel] = CompressorReductionLevel[cntChannel];
				if (CompressionData[cntChannel] > InverseMakeupGain[cntChannel])//was 0.1
				{//0.1 is unity gain because makeup of 10x is used
					CompressionData[cntChannel] *= 0.99999;
				}
			}
			else
			{
//				CompressionData[cntChannel] = _divf(AbsoluteThreshold, DynamicsRMS[cntChannel]);
				CompressionData[cntChannel] = AbsoluteThreshold*_rcpsp(DynamicsRMS[cntChannel]);
			}
		}

		//Make Attack/Release
		{
			float FactorX = 1;
			for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel+=2)
			{
				FactorX = 1;
				if (CompressionData[cntChannel]<CompressorReductionLevel[cntChannel])
				{
				    FactorX = AttackFactor;
		        }

				if (CompressionData[cntChannel]>CompressorReductionLevel[cntChannel])
				{
				    FactorX = ReleaseFactor;
		        }
				CompressorReductionLevel[cntChannel] *= FactorX;
			}
		}

		//Check Release maximum
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel+=2)
		{
			if (CompressorReductionLevel[cntChannel]>1)
				CompressorReductionLevel[cntChannel]=1;
		}

		//Create result
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
		{
			ProcessedSample[cntChannel] = DynamicsOutput[cntChannel]*CompressorReductionLevel[cntChannel&0xFE];
		}

		//Create crossfade & MakeUpGain
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
		{
			DynamicsOutput[cntChannel] *= DynamicsOriginalFactor[cntChannel];
			DynamicsOutput[cntChannel] += ProcessedSample[cntChannel]*DynamicsProcessedFactor[cntChannel]*MakeupGain[cntChannel];
		}
		for (cntChannel=0; cntChannel<NUMBEROFDYNAMICSOBJECTS; cntChannel++)
		{
			if (!DynamicsOn[cntChannel])
			{
				DynamicsOutput[cntChannel] = DynamicsInput[cntChannel];
			}
		}
/***/

	}

	{
		const float FactorX = SmoothFactor;
		const float FactorY = 1-SmoothFactor;
		for (cntChannel=0;cntChannel<NUMBEROFLEVELOBJECTS; cntChannel++)
		{
			LevelOutput[cntChannel] = LevelInput[cntChannel]*LevelFactor[cntChannel];
			LevelFactor[cntChannel] = (LevelFactor[cntChannel]*FactorX)+(FactorY*Update_LevelFactor[cntChannel]);
		}
	}

	{
		const float FactorX = PPMReleaseFactor;
		const float FactorY = VUReleaseFactor;
		for (cntChannel=0;cntChannel<NUMBEROFMETEROBJECTS; cntChannel++)
		{
			float AbsoluteAudio = _fabsf(MeterInput[cntChannel]);
			if (AbsoluteAudio>MeterPPM[cntChannel])
				MeterPPM[cntChannel] = AbsoluteAudio;
			MeterVU[cntChannel] = (MeterVU[cntChannel]*FactorY)+((1-FactorY)*AbsoluteAudio);
		}
	}

	{
		float ReleaseFactor;
		for (cntChannel=0;cntChannel<NUMBEROFMETEROBJECTS/2; cntChannel++)
		{
			float Mono = MeterInput[(cntChannel<<1)+0]+MeterInput[(cntChannel<<1)+1];
			float Phase = 0;
			if (Mono != 0)
			{
				float Difference = MeterInput[(cntChannel<<1)+0]-MeterInput[(cntChannel<<1)+1];
				float X0 = _rcpsp(Mono);
				float X1 = X0 * (2.0 - Mono * X0);
				float X2 = X1 * (2.0 - Mono * X1);
				Phase = _fabsf(Difference * X2);
			}
			if (Phase<PhaseRMS[cntChannel])
			{
				ReleaseFactor = 1-PhaseRelease;
			}
			else if (Phase>PhaseRMS[cntChannel])
			{
				ReleaseFactor = 1+PhaseRelease;
			}

			PhaseRMS[cntChannel] *= ReleaseFactor;
			if (PhaseRMS[cntChannel] < 0.01)
					PhaseRMS[cntChannel] = 0.01;
		}
	}

	for (cntChannel=0;cntChannel<NUMBEROFOUTPUTCHANNELS; cntChannel++)
	{
		OutputBuffer[cntChannel] = _spint(McASPAInput[cntChannel]);
	}

//	OutputBuffer[0] = InputBuffer[0];
//	OutputBuffer[1] = InputBuffer[1];
}

