function [magnitudes] = goetzel(signal, targetfreq, blocksize, samplingrate)
blocks = samplingrate/2 /blocksize;
k = floor(0.5 + blocksize * targetfreq / samplingrate);
w = k * 2 * pi / blocksize;
cosw = cos(w);
%sinw = sin(w);
coef = 2 * cosw;
magnitudes = zeros(blocks,1);
for i=1:blocks
magnitudes(i) = goertzelstep(signal, coef, blocksize, i);
end
end

function [magnitude] = goertzelstep(signal, coef, blocksize, i)
stepm1 = 0;
stepm2 = 0;
for j=1:blocksize
    stepm0 = coef * stepm1 - stepm2 + signal(blocksize*(i-1)+j,1);
    stepm2 = stepm1;
    stepm1 = stepm0;
end
magnitude = sqrt(stepm1^2 + stepm2^2 - stepm1 * stepm2 * coef);
end

