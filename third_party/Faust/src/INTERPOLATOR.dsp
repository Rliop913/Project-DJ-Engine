import("stdfaust.lib");



selectNum = fvariable(int selectInterpolator, "Faust_interpolate.hpp");
p1 = fvariable(float v1, "");
p2 = fvariable(float v2, "");
p3 = fvariable(float v3, "");
p4 = fvariable(float v4, "");
p5 = fvariable(float v5, "");
p6 = fvariable(float v6, "");
p7 = fvariable(float v7, "");
p8 = fvariable(float v8, "");
pZero = fvariable(float vZero, "");

InterpolateFrames = fvariable(int frames, "");
activate_timer = fvariable(int timerActive, "");

TIMER = ba.countup(InterpolateFrames, activate_timer) / InterpolateFrames;
pointNumbers = 8;
WWwaveform(idx) = p1, p2, p3, p4, p5, p6, p7, p8 : ba.selectn(pointNumbers, idx);
idv = it.make_idv(TIMER * pointNumbers);

ITfirst = it.interpolator_linear(WWwaveform, idv);
ITsecond = it.interpolator_cosine(WWwaveform, idv);
ITthird = it.interpolator_cubic(WWwaveform, idv);


ITSW = ITfirst, ITsecond, ITthird, pZero : ba.selectn(4, selectNum);