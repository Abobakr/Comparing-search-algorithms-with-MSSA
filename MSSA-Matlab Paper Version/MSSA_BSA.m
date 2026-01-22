function [R,TC]= MSSA_BSA(x,X,d,key)
R=0;
TC = 0;
m = length(X);

lo = fix(x/key)-d;
hi = fix(x/key)+d;

if  lo<1
    lo = 1;
elseif  hi>m
    hi = m;
end

while lo<=hi
    
    TC = TC +1 ;
    
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