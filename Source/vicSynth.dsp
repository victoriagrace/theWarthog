import("stdfaust.lib");
import("filter.lib");

freq=nentry("freq",440,20,20000,0.01) :si.smoo;
gain = nentry("gain",1,0,1,0.01) :si.smoo;
onoff = button("gate"):si.smoo;

fc= hslider("cutoff",500,50,3000,.01) : si.smoo;

process = hgroup("saw",os.saw2(freq):lowpass(2,fc) * gain * onoff);
