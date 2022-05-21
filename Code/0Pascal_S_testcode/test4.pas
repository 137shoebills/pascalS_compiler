program test(input, output);

type 
arr=array[1..10,2..7] of integer; 
rec=record a:arr;b:integer;c:real end;
arr1=array[1..5,5..7] of rec;
rec1=record a:arr1;b:arr end;

var 
s,r:arr;
u,v,w:rec;
x,y:integer;
s1,r1:arr1;
u1,v1:rec1;
z:real;

begin
  u1.a[2][6].a[1][3]:=1;
  v1.a[3][5].b:=u1.a[2][6].a[1][3];
  s1[2][7].a[1][3]:=3;
  r1[1][5].b:= s1[2][7].a[1][3];
end.