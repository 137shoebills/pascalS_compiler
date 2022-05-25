program test(input, output);
const
x1=true;
x2=true;
x3=false;
x4=false;
var
x:integer;
y:boolean;
begin
    
    y:= not x1;
    y:= not x3;
    y:= x1 and x2;
    y:= x1 and x3;
    y:= x1 or x3;
    y:=x3 or x4;
end.
