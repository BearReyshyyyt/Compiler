$$ p2g1, remember to try variants of this with each different statement $$
data x := 1;
data y := 2;
main
begin
    outter x;
    begin
        data x := 1; 
        outter x;
    end
    void y;
    getter x ;
    proc y;
    outter (x + x) ;
    assign x := (1 + 1);
    loop[4 % 1]
        outter x;
    ;
end