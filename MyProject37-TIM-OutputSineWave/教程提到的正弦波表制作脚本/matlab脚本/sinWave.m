% Generate sine wave DAC values for STM32 (12-bit DAC, 3.3V reference)
n = 2*pi/32 : 2*pi/32 : 2*pi;      % Divide 2¦Ð into 32 equal parts

a = sin(n) + 1;                    % Shift sine wave up (range 0-2)
a = a * 3.3/2;                     % Scale to 0-3.3V
r = a * (2^12)/3.3;                % Convert to 12-bit DAC values (0-4095)
r = uint16(r);                     % Convert to unsigned 16-bit integer
r(r > 4095) = 4095;                % Clamp to maximum DAC value

% Write to C file with horizontal array formatting
fid = fopen('dac_sinWave.c', 'w');
fprintf(fid, 'const uint16_t sineWaveTable[32] = { ');
fprintf(fid, '%d, ', r(1:end-1));  % Print all but last value with commas
fprintf(fid, '%d };\n', r(end));   % Print last value without comma
fclose(fid);

% Plot the results (optional)
figure;
plot(n, r, 'o-');
title('12-bit DAC Values for Sine Wave');
xlabel('Angle (radians)');
ylabel('DAC Value');
grid on;