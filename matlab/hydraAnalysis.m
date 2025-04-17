% Ask user to choose video file
[filename, pathname] = uigetfile('*.mp4', 'Select Hydra Video File');
if isequal(filename, 0)
    error('No video selected.');
end

videoFile = fullfile(pathname, filename);
vidObj = VideoReader(videoFile);

% Read first frame
firstFrame = readFrame(vidObj);
grayFirst = rgb2gray(firstFrame);

% STEP 1: CALIBRATION - get pixel-to-mm scale
figure; imshow(firstFrame);
title('Click two points a known distance apart (for calibration)');
[xCalib, yCalib] = ginput(2); close;

pixelDist = norm([xCalib(1) - xCalib(2), yCalib(1) - yCalib(2)]);
realDistMM = input('Enter the real-world distance between those points (in mm): ');
pixelsPerMM = pixelDist / realDistMM;

% STEP 2: HEAD and TAIL selection
figure; imshow(firstFrame);
title('Click on the HEAD first, then the TAIL of the Hydra');
[clickedX, clickedY] = ginput(2); close;

% Define points to track
points = [clickedX clickedY];

% Create point tracker
pointTracker = vision.PointTracker('MaxBidirectionalError', 2);
initialize(pointTracker, points, grayFirst);

% Reset video
vidObj.CurrentTime = 0;
frameRate = vidObj.FrameRate;
k = 1;

lengthsMM = [];
timePoints = [];

% Store last known tracked points
lastTrackedPoints = points;

figure; set(gcf, 'Position', [100 100 1200 600]);

while hasFrame(vidObj)
    frame = readFrame(vidObj);
    grayFrame = rgb2gray(frame);

    % Track points
    [trackedPoints, isFound] = step(pointTracker, grayFrame);

    if all(isFound)
        % Compute pixel distance and convert to mm
        pixelLength = norm(trackedPoints(1,:) - trackedPoints(2,:));
        hydraLengthMM = pixelLength / pixelsPerMM;
        lengthsMM(end+1) = hydraLengthMM;
        timePoints(end+1) = k / frameRate;

        % Store the last known tracked points
        lastTrackedPoints = trackedPoints;

        % Display
        subplot(1, 2, 1);
        imshow(frame); hold on;
        plot(trackedPoints(:,1), trackedPoints(:,2), 'ro', 'MarkerSize', 10, 'LineWidth', 2);
        line(trackedPoints(:,1), trackedPoints(:,2), 'Color', 'r', 'LineWidth', 2);
        title(sprintf('Frame %d: Hydra Length = %.2f mm', k, hydraLengthMM));
        hold off;

        subplot(1, 2, 2);
        smoothed = movmean(lengthsMM, 5);
        plot(timePoints, smoothed, 'b-', 'LineWidth', 2);
        xlabel('Time (s)');
        ylabel('Hydra Length (mm)');
        title('Hydra Contraction Over Time');
        grid on;
    else
        disp('Tracking lost.');

        % Ask the user whether to reselect or skip ahead
        choice = questdlg('Tracking lost. Would you like to reselect the Hydra position or continue from last known position?', ...
            'Tracking Lost', 'Reselect', 'Use Last Known Position + Skip Ahead', 'Reselect');

        switch choice
            case 'Reselect'
                % Release the tracker
                release(pointTracker);

                % Let user reselect head and tail
                figure; imshow(frame);
                title('Click to select HEAD and TAIL again');
                [clickedX, clickedY] = ginput(2); close;

                points = [clickedX clickedY];
                pointTracker = vision.PointTracker('MaxBidirectionalError', 2);
                initialize(pointTracker, points, grayFrame);

            case 'Use Last Known Position + Skip Ahead'
                % Ask user how many frames to skip
                framesToSkip = inputdlg('Enter number of frames to skip ahead:', ...
                                        'Skip Frames', [1 50], {'0'});
                if isempty(framesToSkip)
                    framesToSkip = 0;
                else
                    framesToSkip = str2double(framesToSkip{1});
                    if isnan(framesToSkip) || framesToSkip < 0
                        framesToSkip = 0;
                    end
                end

                % Skip frames
                for skip = 1:framesToSkip
                    if hasFrame(vidObj)
                        readFrame(vidObj);
                        k = k + 1;
                    else
                        break;
                    end
                end

                % Reinitialize the tracker with last known points
                if hasFrame(vidObj)
                    frame = readFrame(vidObj);
                    grayFrame = rgb2gray(frame);
                    k = k + 1;

                    release(pointTracker);
                    pointTracker = vision.PointTracker('MaxBidirectionalError', 2);
                    initialize(pointTracker, lastTrackedPoints, grayFrame);

                    % Try to track on this new frame
                    [trackedPoints, isFound] = step(pointTracker, grayFrame);

                    if all(isFound)
                        pixelLength = norm(trackedPoints(1,:) - trackedPoints(2,:));
                        hydraLengthMM = pixelLength / pixelsPerMM;
                        lengthsMM(end+1) = hydraLengthMM;
                        timePoints(end+1) = k / frameRate;

                        lastTrackedPoints = trackedPoints;

                        subplot(1, 2, 1);
                        imshow(frame); hold on;
                        plot(trackedPoints(:,1), trackedPoints(:,2), 'ro', 'MarkerSize', 10, 'LineWidth', 2);
                        line(trackedPoints(:,1), trackedPoints(:,2), 'Color', 'r', 'LineWidth', 2);
                        title(sprintf('Frame %d: Hydra Length = %.2f mm', k, hydraLengthMM));
                        hold off;

                        subplot(1, 2, 2);
                        smoothed = movmean(lengthsMM, 5);
                        plot(timePoints, smoothed, 'b-', 'LineWidth', 2);
                        xlabel('Time (s)');
                        ylabel('Hydra Length (mm)');
                        title('Hydra Contraction Over Time');
                        grid on;
                    else
                        disp('Tracking failed again after skip.');
                    end
                end

                % Continue to next iteration of loop
                continue;
        end
    end

    k = k + 1;
    pause(0.05);
end


