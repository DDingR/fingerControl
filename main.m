clear
clc

delete(instrfindall)
ard_controller = serial('/dev/cu.usbmodem14101'); % USB address
ard_reporter = serial('/dev/cu.usbmodem14102'); % USB address

fopen(ard_controller);pause(3);
foped(ard_reporter);pause(3);
 
count = 0;
tic
startTimer = toc;
min = -0.2;
max = 1;

for i = 1:8
    out = fscanf(ard);
    fprintf(out);
end

time = [];
DIR_ = [];
cycle = [];
force = [];

while (toc < startTimer+60)
    out = fscanf(ard);
    %     fprintf(out);

    tmp_array = str2double(strsplit(out));
    % cycle_GO_ DIR CNT cycle displacment force END_flag

    fprintf("cycle_GO_: %d\t", tmp_array(1));
    fprintf("DIR: %d\t", tmp_array(2));
    fprintf("cycle: %d\t", tmp_array(4));
    fprintf("FORCE: %0.3f\t", tmp_array(6));
    fprintf("END_flag: %d\n", tmp_array(7));

    cycle_GO_ = tmp_array(1);
    END_flag = tmp_array(7);

    if (cycle_GO_)
        count = count+1;
        DIR_(count) = tmp_array(2);
        cycle(count) = tmp_array(4);
        force(count) = tmp_array(6);
        if (isempty(time))
            time(1) = 0;
            start_time = toc;
        else
            time(count) = toc - start_time;
            plot(time, force, '-r');
            axis( [time(1) time(end) min max]);
            drawnow;

        end
    end


    if (END_flag)
        fprintf("END_flag is HIGH!\n")
        break;
    end
    % end
end

fprintf("Record finished!\n")
endTimer = toc;

fclose(ard);
delete(ard)
clear ard


