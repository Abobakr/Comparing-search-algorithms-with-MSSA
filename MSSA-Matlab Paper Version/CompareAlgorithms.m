%% THIS CODING IS DONE FOR PROOFING THE RESULTS OF THE PUBLISHED PAPER: https://ieeexplore.ieee.org/document/8751856/ 

%4 Algorithms: Binary (BSA) | Interpolation (ISA) | MSSA using Binary (MSSA_BSA) | MSSA using Interpolation (MSSA_ISA)
n = 3;   % Comparision count for different array sizes each time x10
m = 1000000;  % The first array size; (when n=1)
% if array's size = 10^6 it would take up to 45 minutes
% rng('shuffle', 'v5normal');

%% Elements Count

TestSize = zeros(1,n);

%% Measure 1

BSA_MaxTC = zeros(1,n);
MSSA_BSA_MaxTC = zeros(1,n);
ISA_MaxTC = zeros(1,n);
MSSA_ISA_MaxTC = zeros(1,n);

%% Measure 2

BSA_AvgTC = zeros(1,n);
MSSA_BSA_AvgTC = zeros(1,n);
ISA_AvgTC = zeros(1,n);
MSSA_ISA_AvgTC = zeros(1,n);

%% Measure 3

BSA_MinTC = zeros(1,n);
MSSA_BSA_MinTC = zeros(1,n);
ISA_MinTC = zeros(1,n);
MSSA_ISA_MinTC = zeros(1,n);

%% Apply Test

for i= 1:n
    
    %% Generating Random Data
        
    TestSize(i) = m;
    Y1=sort(round(randn(1,m*2.5)*m*10));
    Y1 = Y1(Y1>0); Y1 = Y1(1:m); % normal increase
    Y2=sort(round(abs(randn(1,m)*m*10))); % exponential increase
    Y3=sort(randi(m*10,1,m)); % Approximate-Uniform increase
    Y4=2:2:m*2; % Ideal Uniform increase
    elements = 1:m;
    figure
    plot(elements,Y1,'g',elements,Y2,'r',elements,Y3,'b',elements,Y4,'y');
    title('Data increase Comparision');
    xlabel('Elements Count');
    ylabel('Average Test Count');
    legend('Normal','Exponential','Approximate-Uniform','Ideal Uniform');
    
    %% Maximum-Spead Search Algorithm's Precalculations
    % MSSCKA (Parallel Version) - Maximum-Spead Search Key Calculation Algorithms
    % [MS_key, MS_d ]=MSSCKA_Parallel(Y1);
    [MS_key, MS_d ]=Game_Changer_MSSCKA(Y1);
    
    %% Search for All compettive Algorithms
    
    BSA_TCs=zeros(1,m);
    MSSA_BSA_TCs=zeros(1,m);
    ISA_TCs=zeros(1,m);
    MSSA_ISA_TCs=zeros(1,m);
    
    for j=1:m
        [~,BSA_TCs(j)]=BSA(Y1(j),Y1);
        [~, MSSA_BSA_TCs(j)]=MSSA_BSA(Y1(j),Y1,MS_d,MS_d);
        [~, ISA_TCs(j)]=ISA(Y1(j),Y1);
        [~, MSSA_ISA_TCs(j)]=MSSA_ISA(Y1(j),Y1,MS_d,MS_d);
    end
    
    %% 1) Worst Case - max Test Count
    
    BSA_MaxTC(i)= max(BSA_TCs);
    MSSA_BSA_MaxTC(i) = max(MSSA_BSA_TCs);
    ISA_MaxTC(i) = max(ISA_TCs);
    MSSA_ISA_MaxTC(i) = max(MSSA_ISA_TCs);
    
    %% 2) Avarege Case - Mean Test Count
    
    BSA_AvgTC(i) = mean(BSA_TCs);
    MSSA_BSA_AvgTC(i) = mean(MSSA_BSA_TCs);
    ISA_AvgTC(i) = mean(ISA_TCs);
    MSSA_ISA_AvgTC(i) = mean(MSSA_ISA_TCs);
    
    %% 3) Total Best Case - Test Count = 1
    
    BSA_MinTC(i) = size(find(BSA_TCs == 1),2);
    MSSA_BSA_MinTC(i) = size(find(MSSA_BSA_TCs == 1),2);
    ISA_MinTC(i) = size(find(ISA_TCs == 1),2);
    MSSA_ISA_MinTC(i) = size(find(MSSA_ISA_TCs == 1),2);
    
    % the size fot the next iteration
    m = m*10;
end

%% Printing
disp(newline);
disp(['Max Test Count', newline ,'***********************']);
disp(['BSA_maxTC = ',num2str(BSA_MaxTC)]);
disp(['MSSA_BSA_maxTC = ',num2str(MSSA_BSA_MaxTC)]);
disp(['ISA_maxTC = ',num2str(ISA_MaxTC)]);
disp(['MSSA_ISA_maxTC = ',num2str(MSSA_ISA_MaxTC)]);

disp(newline);
disp(['Mean Test Count', newline ,'***********************']);
disp(['BSA_AvgTC = ',num2str(BSA_AvgTC)]);
disp(['MSSA_BSA_AvgTC = ',num2str(MSSA_BSA_AvgTC)]);
disp(['ISA_AvgTC = ',num2str(ISA_AvgTC)]);
disp(['MSSA_ISA_AvgTC = ',num2str(MSSA_ISA_AvgTC)]);

disp(newline);
disp(['Total Test Count = 1', newline ,'***********************']);
disp(['BSA_MinTC = ',num2str(BSA_MinTC)]);
disp(['MSSA_BSA_MinTC = ',num2str(MSSA_BSA_MinTC)]);
disp(['ISA_MinTC = ',num2str(ISA_MinTC)]);
disp(['MSSA_ISA_MinTC = ',num2str(MSSA_ISA_MinTC)]);

%% Figures

if(n==1)
    c = categorical({'BSA','MSSA\_BSA','ISA','MSSA\_ISA'});
    figure
    bar(c,[BSA_MaxTC,MSSA_BSA_MaxTC,ISA_MaxTC,MSSA_ISA_MaxTC]);
    title('Worst-case');
    
    c = categorical({'BSA','MSSA\_BSA','ISA','MSSA\_ISA'});
    figure
    bar(c,[BSA_AvgTC,MSSA_BSA_AvgTC,ISA_AvgTC,MSSA_ISA_AvgTC]);
    title('Average-case');
    
    c = categorical({'BSA','MSSA\_BSA','ISA','MSSA\_ISA'});
    figure
    bar(c,[BSA_MinTC,MSSA_BSA_MinTC,ISA_MinTC,MSSA_ISA_MinTC]);
    title('Best-case');
    
else
    
    figure
    plot(TestSize,BSA_MaxTC,'r',TestSize,MSSA_BSA_MaxTC,'m'...
        ,TestSize,ISA_MaxTC,'g',TestSize,MSSA_ISA_MaxTC,'b');
    title('Worst-case');
    xlabel('Elements Count');
    ylabel('maximum Test Count');
    legend('BSA','MSSA\_BSA','ISA','MSSA\_ISA');
    
    figure
    plot(TestSize,BSA_AvgTC,'r',TestSize,MSSA_BSA_AvgTC,'m'...
        ,TestSize,ISA_AvgTC,'g',TestSize,MSSA_ISA_AvgTC,'b');
    title('Average-case');
    xlabel('Elements Count');
    ylabel('Average Test Count');
    legend('BSA','MSSA\_BSA','ISA','MSSA\_ISA');
    
    figure
    plot(TestSize,BSA_MinTC ,'r',TestSize,MSSA_BSA_MinTC,'m'...
        ,TestSize,ISA_MinTC,'g',TestSize,MSSA_ISA_MinTC,'b');
    title('Best-case');
    xlabel('Elements Count');
    ylabel('Total Min Test Count');
    legend('BSA','MSSA\_BSA','ISA','MSSA\_ISA');
    
end

%% Note that here we are Comparing MSSA with other searching algorithms by testing the existence of all elements
% Execution Time is only for the array of the largest size elements
m = m/10;
ExecutionTime = zeros(1,4);

tic
for j=1:m
    BSA(Y1(j),Y1);
end
ExecutionTime(1) = toc;

tic
for j=1:m
    MSSA_BSA(Y1(j),Y1,MS_d,MS_d);
end
ExecutionTime(2) = toc;

tic
for j=1:m
    ISA(Y1(j),Y1);
end
ExecutionTime(3) = toc;

tic
for j=1:m
    MSSA_ISA(Y1(j),Y1,MS_d,MS_d);
end
ExecutionTime(4) = toc;

%%  Execution Time Results

AlgsNames = ['BSA       ','MSSA_BSA      ','ISA     ','MSSA_ISA    '];

disp(['AlgsNames = ',AlgsNames]);
disp(['ExecTime = ',num2str(ExecutionTime)]);

c = categorical({'BSA','MSSA\_BSA','ISA','MSSA\_ISA'});
figure
bar(c,ExecutionTime);
title(['ExecTime for searching ', num2str(m), 'items (all existed)']);
