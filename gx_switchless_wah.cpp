/*
 * Copyright (C) 2012 Hermann Meyer, Andreas Degert, Pete Shorthose, Steve Poskitt
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

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdint>

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
template <int32_t N> inline float faustpower(float x)
{
  return powf(x, N);
}
template <int32_t N> inline double faustpower(double x)
{
  return pow(x, N);
}
template <int32_t N> inline int32_t faustpower(int32_t x)
{
  return faustpower<N/2>(x) * faustpower<N-N/2>(x);
}
template <>      inline int32_t faustpower<0>(int32_t x)
{
  return 1;
}
template <>      inline int32_t faustpower<1>(int32_t x)
{
  return x;
}

#include "gx_switchless_wah.h"
#include "switchless_wah.cc"

// define run pointer typs
typedef void (switchless_wah::*run_switchless_wah)
(uint32_t count,float* input, float* output);

////////////////////////////// MONO ////////////////////////////////////

class GxSwitchLessWah
{
private:
  // internal stuff
  float*                       output;
  float*                       input;
  switchless_wah                      wah;
  run_switchless_wah                  _wah;
  //dunwahauto                   wah;
public:
  void activate_f();
  inline void run_dsp_mono(uint32_t n_samples);
  void connect_mono(uint32_t port,void* data);
  inline void init_dsp_mono(uint32_t rate, const LV2_Descriptor* descriptor);
  inline void connect_all_mono_ports(uint32_t port, void* data);
  // constructor
  GxSwitchLessWah() :
    output(NULL),
    input(NULL),
    wah(switchless_wah()),
    _wah(NULL)
  {
  };
  // destructor
  ~GxSwitchLessWah()
  {
  };
};

// plugin stuff

void GxSwitchLessWah::init_dsp_mono(uint32_t rate, const LV2_Descriptor* descriptor)
{
  if (strcmp("http://guitarix.sourceforge.net/plugins/gx_switchless_wah#wah",descriptor->URI)== 0)
    {
      _wah = &switchless_wah::run_d;
      wah.init_d(rate);
    }
}

void GxSwitchLessWah::connect_mono(uint32_t port,void* data)
{
  switch ((EffectPortIndex)port)
    {
    case AUTOWAH_OUTPUT:
      output = static_cast<float*>(data);
      break;
    case AUTOWAH_INPUT:
      input = static_cast<float*>(data);
      break;
    default:
      break;
    }
}

void GxSwitchLessWah::activate_f()
{
  wah.clear_state_fd();
}

void GxSwitchLessWah::run_dsp_mono(uint32_t n_samples)
{
  // run dsp
  (&wah->*_wah)(n_samples, input, output);
}

void GxSwitchLessWah::connect_all_mono_ports(uint32_t port, void* data)
{
  connect_mono(port,data);
  wah.connect_d(port,data);
}

///////////////////////////// LV2 defines //////////////////////////////

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{

  GxSwitchLessWah *self = new GxSwitchLessWah();
  if (!self)
    {
      return NULL;
    }

  self->init_dsp_mono((uint32_t)rate, descriptor);

  return (LV2_Handle)self;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
  GxSwitchLessWah* self = (GxSwitchLessWah*)instance;
  self->connect_all_mono_ports(port, data);
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
  GxSwitchLessWah* self = (GxSwitchLessWah*)instance;
  self->run_dsp_mono(n_samples);
}

static void
activate(LV2_Handle instance)
{
  GxSwitchLessWah* self = (GxSwitchLessWah*)instance;
  self->activate_f();
}

static void
cleanup(LV2_Handle instance)
{
  GxSwitchLessWah* self = (GxSwitchLessWah*)instance;
  delete self;
}

//////////////////////////////////////////////////////////////////

static const LV2_Descriptor descriptor1 =
{
  GXPLUGIN_URI "#wah",
  instantiate,
  connect_port,
  activate,
  run,
  NULL,
  cleanup,
  NULL
};

extern "C"
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index)
    {
    case 0:
      return &descriptor1;
    default:
      return NULL;
    }
}
