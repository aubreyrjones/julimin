#!/usr/bin/env python

# This script resamples the wave files found at <http://www.adventurekid.se/akrt/waveforms/> from 600 samples
# to a configurable number of samples. Set the number of output samples with the OUT_SAMPLES variable below.

# This script depends on `numpy`, which you may not have installed.

OUT_SAMPLES = 512
OUT_BITS = 12

import sys
import wave
import numpy
import struct

if len(sys.argv) < 3:
    print "USAGE: resample_akwf filename symbolname"
    print "Output printed to STDOUT, use pipe to redirect. (`>>` to append to existing file.)"
    exit()

w = wave.open(sys.argv[1], 'r')

nSamples = w.getnframes()
rawSamples = w.readframes(nSamples)

normFactor = 2.0**16

samples = map(lambda s: s / normFactor, struct.unpack_from("%dh" % nSamples, rawSamples))
sampleTimes = map(lambda x: x * (1.0 / (nSamples - 1)), range(0, nSamples))

resampledTimes = map(lambda x: x * (1.0 / (OUT_SAMPLES - 1)), range(0, OUT_SAMPLES))
outSamples = map(lambda t: numpy.interp(t, sampleTimes, samples), resampledTimes)

scaleFactor = 2**OUT_BITS

scaledSamples = map(lambda s: int(round((s + 0.5) * scaleFactor)), outSamples)

print "uint16_t const {}[] = {{".format(sys.argv[2]), ", ".join(str(s) for s in scaledSamples), "};\n"

