function[R,TC] = MSSA_ISA(x,X,d,key)
R = 0;
TC = 0;
m =length(X);
lo = fix(x/key-d);
hi = fix(x/key+d);

if lo<1
    lo=1;
elseif lo>m
    lo = m;
end
if hi>m
    hi = m;
end

while (X(hi)~= X(lo) && x >= X(lo) && x <= X(hi))
    
    TC = TC +1 ;
    
    pos =  lo + fix( ( x - X(lo) ) * (hi-lo) / ( X(hi)-X(lo) ) );
    
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


