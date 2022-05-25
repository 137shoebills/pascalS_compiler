{数组嵌套}
program test(input, output);

type 
arr0=array[1..10] of integer; 
arr1=array[11..15,16..20] of arr0;
arr2=array[21..25,26..30] of arr1;

var 
x:arr0;
y:arr1;
z:arr2;

begin
  x[1]:=3;
  y[11][16][2]:=4;
  z[21][26][15][20][10]:=5
end.