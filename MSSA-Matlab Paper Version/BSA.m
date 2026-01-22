function [R,TC] = BSA(x,X)

lo=1;
hi=length(X);

R=0;
TC = 0;

while lo<=hi 
    
    TC = TC +1;
    
    i= fix((lo+hi)/2);
    
    if x==X(i)
        R=i;
        return
    end   
    
    if x<X(i)
        hi=i-1;
    else
        lo=i+1;
    end
    
end