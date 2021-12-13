/*
 * Copyright (C) 2016 Hermann Meyer, Andreas Degert
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * --------------------------------------------------------------------------
 */

class switchless_wah {
private:
	uint32_t fSamplingFreq;
	float 	*fslider0_;
	float 	fslider0;
	int32_t 	iConst0;
	double 	fConst1;
	double 	fRec1[2];
	double 	fConst2;
	double 	fConst3;
	double 	fRec2[2];
	double 	fConst4;
	double 	fRec3[2];
	double 	fRec0[4];
	double 	fConst5;
	double 	fConst6;
	double 	fConst7;
	double 	fConst8;
	double 	fConst9;
	double 	fConst10;
	double 	fConst11;
	float 	*faslider0_;
	float 	faslider0;
	double 	faConst0;
	double 	faRec0[2];
	float 	*faslider1_;
	float 	faslider1;
	double 	faRec1;
	double 	faslider2_[2];
	int 	iSlow2;
	float 	*faslider3_;
	float 	faslider3;
	float 	*faslider4_;
	float 	faslider4;
public:
    void clear_state_fd();
	void init_d(uint32_t samplingFreq);
	void run_d(uint32_t count, float *input0, float *output0);
    void connect_d(uint32_t port,void* data);
	switchless_wah();
	~switchless_wah();
};

switchless_wah::switchless_wah() {
}

switchless_wah::~switchless_wah() {
}

inline void switchless_wah::clear_state_fd()
{
	for (int32_t i=0; i<2; i++) fRec1[i] = 0;
	for (int32_t i=0; i<2; i++) fRec2[i] = 0;
	for (int32_t i=0; i<2; i++) fRec3[i] = 0;
	for (int32_t i=0; i<4; i++) fRec0[i] = 0;
	for (int32_t i=0; i<2; i++) faRec0[i] = 0;
	faRec1 = 1;
	for (int32_t i=0; i<2; i++) faslider2_[i] = fslider0;
	iSlow2 = 0;
}

void switchless_wah::connect_d(uint32_t port,void* data)
{
  switch ((EffectPortIndex)port)
    {
    case WAH:
      fslider0_ = (float*)data;
      break;
    case ATTACK:
      faslider0_ = (float*)data;
      break;
    case RELEASE:
      faslider1_ = (float*)data;
      break;
    case MINLEVEL:
      faslider3_ = (float*)data;
      break;
    case MAXLEVEL:
      faslider4_ = (float*)data;
      break;
    default:
      break;
    }
}

inline void switchless_wah::init_d(uint32_t samplingFreq)
{
	fSamplingFreq = samplingFreq;
	iConst0 = min(192000, max(1, fSamplingFreq));
	faConst0 = (1e+04 / double(min(192000, max(1, fSamplingFreq))));
	fConst1 = (0.007000000000000006 * ((iConst0 * (1.73888e-06 - (8.38823e-12 * iConst0))) - 0.193457));
	fConst2 = (0.5 / iConst0);
	fConst3 = (1.0 / iConst0);
	fConst4 = exp((0 - (1236.9027460477864 / iConst0)));
	fConst5 = (1.77528e-06 - (8.52216e-12 * iConst0));
	fConst6 = (0.879905 + (iConst0 * fConst5));
	fConst7 = (1.54419e-05 - (6.43963e-11 * iConst0));
	fConst8 = ((iConst0 * fConst7) - 0.386688);
	fConst9 = (fConst8 * (0 - (1.00038 * fConst6)));
	fConst10 = ((fConst8 * fConst6) + (1.00038 * (fConst8 + fConst6)));
	fConst11 = (0 - ((iConst0 * (fConst7 + fConst5)) + 1.4935970000000003));
	clear_state_fd();
}

inline void switchless_wah::run_d(uint32_t count, float *input0, float *output0)
{
    fslider0 = (*fslider0_);
    faslider0 = (*faslider0_);
    faslider1 = (*faslider1_);
    faslider3 = (*faslider3_);
    faslider4 = (*faslider4_);
	double 	fSlow0 = fslider0;
	double 	fSlow1 = (fConst1 * (0 - ((1.0 / ((fSlow0 * (0.270546 + (fSlow0 * ((fSlow0 * (3.64419 + (fSlow0 * ((2.85511 * fSlow0) - 5.20364)))) - 0.86331)))) - 0.814203)) + 0.933975)));
	double 	fSlow2 = (1973.48 - (1000 / ((fSlow0 * (1.9841 + (fSlow0 * (5.76598 + (fSlow0 * ((fSlow0 * (49.9836 + (fSlow0 * ((12.499 * fSlow0) - 40.3658)))) - 28.3434)))))) - 1.6086)));
	double 	fSlow3 = (1 - (fConst2 * (fSlow2 / (21.9737 + (fSlow0 * ((fSlow0 * (42.2734 + (fSlow0 * ((fSlow0 * (115.375 - (52.3051 * fSlow0))) - 99.7712)))) - 24.555))))));
	double 	fSlow4 = (0.007000000000000006 * (cos((fConst3 * fSlow2)) * (0 - (2.0 * fSlow3))));
	double 	fSlow5 = (0.007000000000000006 * faustpower<2>(fSlow3));
	double 	faSlow0 = (7.000000000000006e-05 * (faConst0 / double(faslider0)));
	double 	faSlow1 = (7.000000000000006e-05 * (0 - (faConst0 / double(faslider1))));
	double 	faSlow2 = (7.000000000000006e-03 * fSlow0);
	
	for (uint32_t i=0; i<count; i++) {
		float fTemp1 = input0[i];
		iSlow2 = ((fabs(faslider2_[0] - fSlow0)>1.0e-6)? 1:0);
		faslider2_[0] = (faslider2_[1] * 0.993) + faSlow2;
		((iSlow2) || (faslider3 < fslider0 && !(faslider4 < fslider0)) ?
			faRec0[0] = min((double)1, ((faRec0[1]) + faSlow0)):
			faRec0[0] = max((double)0, ((faRec0[1]) + faSlow1))); //wet
		faRec1 = 1.0 - faRec0[0]; //dry

		fRec1[0] = (fSlow1 + (0.993 * fRec1[1]));
		fRec2[0] = (fSlow4 + (0.993 * fRec2[1]));
		fRec3[0] = (fSlow5 + (0.993 * fRec3[1]));
		fRec0[0] = (0 - (((fRec0[1] * (fRec2[0] - fConst4)) + ((fConst4 * ((0 - fRec3[0]) * fRec0[3])) + (fRec0[2] * (fRec3[0] - (fConst4 * fRec2[0]))))) - (fTemp1 * fRec1[0])));
		float fTemp2 = (float)((fConst11 * fRec0[1]) + ((fConst10 * fRec0[2]) + (fRec0[0] + (fConst9 * fRec0[3]))));
		output0[i] = (fTemp1 * faRec1) + (fTemp2 * faRec0[0]);
		// post processing
		for (int32_t i=3; i>0; i--) fRec0[i] = fRec0[i-1];
		fRec3[1] = fRec3[0];
		fRec2[1] = fRec2[0];
		fRec1[1] = fRec1[0];
		faRec0[1] = faRec0[0];
		faslider2_[1] = faslider2_[0];
	}
	
}
