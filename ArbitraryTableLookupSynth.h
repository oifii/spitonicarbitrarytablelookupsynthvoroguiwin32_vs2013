/*
 * Copyright (c) 2015-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  ArbitraryTableLookupSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 5/21/13.
//

#ifndef ArbitraryTableLookupSynth_h
#define ArbitraryTableLookupSynth_h

#include "Tonic.h"

using namespace Tonic;

class ArbitraryTableLookupSynth : public Synth
{
  
public:

  ArbitraryTableLookupSynth(){
    
    // Build a table of arbitrary length. TableLookupOsc requires power-of-two+1 length tables, but they will be resized if necessary.
    // Try changing this to 2049 and compare the sound. Should be perceptually almost identical.
    const unsigned int tablesize = 2500;
    
    // You can register this in a collection if you want to use it in a few oscillators
    SampleTable lookupTable = SampleTable(tablesize, 1);
    
    TonicFloat norm = 1.0f / tablesize;
    TonicFloat * tableData = lookupTable.dataPointer();
    for (unsigned int i=0; i<tablesize; i++){
      
      // sum a few sine waves
      TonicFloat phase = TWO_PI * i * norm;
      *tableData++ = 0.75f * sinf(phase) + 0.5f * sinf( phase * 2 ) + 0.25f * sinf(phase * 5);
      
    }
    
	//TableLookupOsc osc = TableLookupOsc().setLookupTable(lookupTable).freq(100 + 40 * SineWave().freq(0.1)); //original
	TableLookupOsc osc = TableLookupOsc().setLookupTable(lookupTable).freq(addParameter("base_freq", 100.0).min(50.0).max(1000.0) + addParameter("lfo_amp", 40.0).min(10.0).max(100.0) * SineWave().freq(addParameter("lfo_freq", 5.0).min(0.1).max(10.0))); //spi
	//TableLookupOsc osc = TableLookupOsc().setLookupTable(lookupTable).freq(90 + 30 * SineWave().freq(10)); //spi

	StereoDelay delay = StereoDelay(3.0f, 3.0f, 3.0f, 3.0f)
		//.delayTimeLeft(0.5 + SineWave().freq(0.2) * 0.01)
		.delayTimeLeft(addParameter("delay_left", 0.5).min(0.0).max(3.0) + SineWave().freq(0.2) * 0.01)
		//.delayTimeRight(0.55 + SineWave().freq(0.23) * 0.01)
		.delayTimeRight(addParameter("delay_right", 0.55).min(0.0).max(3.0) + SineWave().freq(0.23) * 0.01)
		//.feedback(0.3)
		.feedback(addParameter("delay_feedback", 0.4).min(0.0).max(1.0))
		.dryLevel(addParameter("delay_drylevel", 0.5).min(0.0).max(1.0))
		//.wetLevel(0.2);
		.wetLevel(addParameter("delay_wetlevel", 0.5).min(0.0).max(1.0));

	//setOutputGen(osc * 0.25); //original
	setOutputGen(delay.input(osc * addParameter("base_amp", 0.25).min(0.0).max(1.0))); //spi
    
  }
  
};

//TONIC_REGISTER_SYNTH(ArbitraryTableLookupSynth);

#endif

