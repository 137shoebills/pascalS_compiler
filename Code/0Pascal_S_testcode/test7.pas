{record及数组嵌套}
program test(input, output);

type 
arr0=array[1..10] of integer; 
arr1=array[11..15,16..20] of arr0;
rec0=record a:arr0;b:integer;c:real end;
rec1=record a:arr1;b:arr0;c:rec0 end;
rec2=record a:rec0;b:arr1 end;

var 
b:arr1;
x:rec0;
y:rec1;
z:rec2;

begin
  b[11][16][10]:=99;
  x.a[9]:=98;
  x.b:=97;
  x.c:=96.0;
  y.a[12][17][8]:=88;
  y.b[7]:=77;
  y.c.a[6]:=66;
  y.c.b:=55;
  z.a.a[5]:=15;
  z.b[13][18][4]:=14;
end.