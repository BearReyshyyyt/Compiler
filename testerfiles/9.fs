$$ p4g5 $$
data y := 2;
data x := 1;
main
begin
    data z := 1 ;
    data sum := 0 ;
    begin
        data y := 5 ;
        data x := 10;        
        outter x;
    end
    if[y =< x]
    then
    begin
            assign z := 10000;
            assign y := 8008;
    end
    ;
    getter y ;
    loop [ y => z ]
        begin
        assign sum := sum + z ;
        assign x := z + 1 ;
        end 
    ;
    outter x ;
end