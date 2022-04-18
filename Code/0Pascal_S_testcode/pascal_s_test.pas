(*
    涉及到的库函数：
    read(): 读入一个数;
    write(): 打印.
*)

program test(input, output);
var x,y: integer;
function gtcomdiv(a,b: integer):integer;
    begin
        if b=0 then gtcomdiv:=a     {这里末尾没有分号？}
        else gtcomdiv:=gtcomdiv(b,a mod b)
    end;
begin
    read(x,y);
    write(gtcomdiv(x,y));
end.
