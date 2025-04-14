% 呼吸灯PWM波形生成器 - 最终版
clear; clc;

% 基础参数
TOTAL_POINTS = 256;     % 总点数（必须偶数）
MAX_VALUE = 1024;       % PWM最大值
GAMMA = 2.3;            % 伽马校正系数（2.2-2.5）

%% ===== 核心波形生成 =====
% 上升阶段（伽马曲线）
t_rise = linspace(0, 1, TOTAL_POINTS/2);
rise = round(MAX_VALUE * (t_rise.^GAMMA));

% 下降阶段（对称处理）
t_fall = linspace(1, 0, TOTAL_POINTS/2);
fall = round(MAX_VALUE * (t_fall.^GAMMA));

% 合并波形（移除中心重复点）
pwm = [rise(1:end-1), fall];

% 低区强化平滑（前15点线性过渡）
pwm(1:15) = round(linspace(0, pwm(16), 15));

%% ===== 可视化验证 =====
figure('Position', [100,100,900,400]);

% 时域波形
subplot(1,2,1);
plot(pwm, '-b', 'LineWidth', 1.5);
hold on;
stem(pwm, 'filled', 'MarkerSize', 4);
title('PWM时域波形');
xlabel('时间点'); ylabel('PWM值');
grid on; axis tight;

% 亮度感知曲线
subplot(1,2,2);
perceived = (pwm/MAX_VALUE).^(1/2.2);  % 反伽马变换
plot(perceived*100, '-r', 'LineWidth', 2);
title('人眼感知亮度 (%)');
xlabel('时间点'); ylabel('亮度百分比');
grid on;

%% ===== Arduino输出 =====
fid = fopen('pwm_table.h', 'w');
fprintf(fid, 'const uint16_t pwmTable[%d] PROGMEM = {\n', TOTAL_POINTS);
for i = 1:TOTAL_POINTS
    fprintf(fid, '%4d', pwm(i));
    if i ~= TOTAL_POINTS, fprintf(fid, ','); end
    if mod(i,16) == 0, fprintf(fid, '\n'); end
end
fprintf(fid, '};\n');
fclose(fid);

disp(['生成完成！点数：', num2str(TOTAL_POINTS)]);