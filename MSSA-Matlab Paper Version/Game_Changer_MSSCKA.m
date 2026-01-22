function [key ,d] = Game_Changer_MSSCKA(X)
m =length(X);

disp('                                                 ');
disp(['Game Changer MSSCKA, Number of elements = ',num2str(m)]);
disp('-------------------------------------------------');


tic;
 
% Initialize key and error
key = X(1) / 1;  % initial key (adjusted for 1-based indexing)
d = 0.0;                % d in ideal case
game_changer = [1];   % vector to track game changers
counter = 1;            % counter for while loop

while counter <= length(game_changer)
    i = game_changer(counter);
    gc = -1;
    
    for j = 1:m
        if i == j
            continue;
        end
        
        xi_over_i = X(i) / i;
        xj_over_j = X(j) / j;
        
        if xi_over_i < xj_over_j
            dd = (i * X(j) - j * X(i)) / (X(i) + X(j));
            if dd > d
                key = X(j) / (j + dd);
                d = dd;
                gc = j;
            end
        elseif xi_over_i > xj_over_j
            dd = (j * X(i) - i * X(j)) / (X(i) + X(j));
            if dd > d
                key = X(i) / (i + dd);  % denominator is always positive
                d = dd;
                gc = j;
            end
        end
    end
    
    if gc ~= -1  % if a game changer was found
        game_changer = [game_changer, gc];
    end
    
    counter = counter + 1;
end


disp(['Game Changer MSSCKA, Execution Time = ',num2str(toc)]);
disp(['Game Changer MSSCKA, Calculated d = ',num2str(d)]);
disp(['Game Changer MSSCKA, Calculated key = ',num2str(key)]);

end