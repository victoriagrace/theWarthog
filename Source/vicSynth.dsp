import("stdfaust.lib");
import("oscillator.lib");
import("filter.lib");

ba = library("basic.lib");
dyn= library("misceffect.lib");

freq=nentry("freq",24,12, 48,0.01) :si.smoo;
gain = hslider("gain",1,0,1,0.01) :si.smoo;
onoff = button("gate"):si.smoo;

//fc= hslider("cutoff",500,50,3000,.01) : si.smoo;

fc=50+gain*2950;
pos= hslider("formant",0,0,1,.01) : si.smoo;

f1_freq_oo = 400;
f1_bw_oo = 10;
f1_amp_oo = 1;

f2_freq_oo = 750;
f2_bw_oo = 10;
f2_amp_oo = 0.7;

f3_freq_oo = 2400;
f3_bw_oo = 5;
f3_amp_oo = 0.5;

f1_freq_ee = 400;
f1_bw_ee = 10;
f1_amp_ee = 1;

f2_freq_ee = 1620;
f2_bw_ee = 10;
f2_amp_ee = 0.65;

f3_freq_ee = 2400;
f3_bw_ee = 5;
f3_amp_ee = 0.9;

f1_freq = pos * f1_freq_oo + (1 - pos) * f1_freq_ee;
f1_bw = pos * f1_bw_oo + (1 - pos) * f1_bw_ee;
f1_amp = pos * f1_amp_oo + (1 - pos) * f1_amp_ee;

f2_freq = pos * f2_freq_oo + (1 - pos) * f2_freq_ee;
f2_bw = pos * f2_bw_oo + (1 - pos) * f2_bw_ee;
f2_amp = pos * f2_amp_oo + (1 - pos) * f2_amp_ee;

f3_freq = pos * f3_freq_oo + (1 - pos) * f3_freq_ee;
f3_bw = pos * f3_bw_oo + (1 - pos) * f3_bw_ee;
f3_amp = pos * f3_amp_oo + (1 - pos) * f3_amp_ee;


//vib = osc(0.5 + 5 * gain) * 0.1 * gain; // for vibrato

process = hgroup("saw",
pulsetrain(ba.midikey2hz(freq), 0.1) 
+ pulsetrain(ba.midikey2hz(freq + 12), 0.1)

//:lowpass(2,fc):dyn.cubicnl(0.8,0.5):
:resonlp(fc, 8, 1):dyn.cubicnl(0.3,0):

resonbp(f1_freq,f1_bw,f1_amp):
resonbp(f2_freq,f2_bw,f2_amp):
resonbp(f3_freq,f3_bw,f3_amp)
* gain);

