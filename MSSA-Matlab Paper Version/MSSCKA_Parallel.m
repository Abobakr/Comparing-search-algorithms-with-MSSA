function [key ,d] = MSSCKA_Parallel(X)
m =length(X);

disp('                                                 ');
disp(['Paralell MSSCKA, Number of elements = ',num2str(m)]);
disp('-------------------------------------------------');


D = zeros(m,1);
J = zeros(m,1);
key = X(1);
tic;
parfor i=1:m-1
    if(mod(i,m/10)==0)
        disp(['Paralell MSSCKA , ',num2str(i/m*100) ,'% left']);
    end
    temp = zeros(1,m); % to avoid variable classification in parfor loop
    for j=i+1:m
        if X(i)/i< X(j)/j
            temp(1,j)=(  i*X(j)-j*X(i) )/( X(i)+X(j) );
        elseif X(i)/i> X(j)/j
            temp(1,j)=(  j*X(i)-i*X(j) )/( X(i)+X(j) );
        end
    end
[Max,Col]= max(temp);
D(i) = Max;
J(i) = Col;
end

[d,i] = max(D); j = J(i);

if X(i)/i< X(j)/j
key=X(i)/(i-d);
elseif X(i)/i> X(j)/j
key=X(i)/(i+d);
end

disp(['Parallel MSSCKA, Execution Time = ',num2str(toc)]);
disp(['Parallel MSSCKA, Calculated d = ',num2str(d)]);
disp(['Parallel MSSCKA, Calculated key = ',num2str(key)]);

end


