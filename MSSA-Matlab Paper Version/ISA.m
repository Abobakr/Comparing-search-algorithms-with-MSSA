function [R,TC] = ISA(x,X) 
lo=1;
hi=length(X);
R=0;
TC = 0;
while (X(hi)~= X(lo) && x >= X(lo) && x <= X(hi))
     TC = TC +1;
     
     pos =  lo + round( ( x - X(lo) ) * (hi-lo) / ( X(hi)-X(lo) ) );
    
    if (X(pos) < x)
        lo = pos + 1;
    elseif (X(pos) > x)
        hi = pos - 1;
    else
        R=pos;
        return;
    end
end

if x==X(lo)
    R=lo;
    return;
end
end
