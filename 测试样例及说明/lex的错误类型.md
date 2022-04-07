## lex的错误类型

* 非法字符错误
* 读取字符常量遇到文件尾
* 读取字符常量为空，错误处理：“\0”
* 读取字符常量长度大于1，错误处理：取第一个字符
* 读取字符常量单引号缺失，错误处理：“\0”或取第一个字符
* 单行注释时遇到换行符，缺失}
* 多行注释之间遇到文件尾，缺失*）

### 非法字符错误

```pascal
program test(input,output);
var a#:integer;
begin
    ?@
end&
```

运行结果

```
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
keyword: var
identifier: a
divop::
keyword: integer
divop:;
keyword: begin
keyword: end
end parsing
print lexicalerrror
begin error printing
[Invalid character!] 2:#
[Invalid character!] 4:?
[Invalid character!] 4:@
[Invalid character!] 5:&
```

### 读取字符常量遇到文件尾

```pascal
program test(input,output);
char a='b
```

运行结果

```pascal
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
keyword: char
identifier: a
relop:=
end parsing
print lexicalerrror
begin error printing
[Unexpected end of file when reading a char constant] 2:
```

### 读取字符常量为空

```pascal
program test(input,output);
char a=''
```

运行结果

```pascal
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
keyword: char
identifier: a
relop:=
letter: 
end parsing
print lexicalerrror
begin error printing
[Missing character!] 2:'
```

### 读取字符常量长度大于1

```pascal
program test(input,output);
char a='wsx'
begin
	writeln(a);
end.
```

运行结果

```pascal
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
keyword: char
identifier: a
relop:=
letter: w
keyword: begin
identifier: writeln
divop:(
identifier: a
divop:)
divop:;
keyword: end
divop:.
end parsing
print lexicalerrror
begin error printing
[Too many characters!] 2:'
```

### 读取字符常量单引号缺失

```pascal
program test(input,output);
char a='w
char b='
char c='wsx
begin
	
end.
```

运行结果

```pascal
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
keyword: char
identifier: a
relop:=
letter: w
keyword: char
identifier: b
relop:=
letter: 
keyword: char
identifier: c
relop:=
letter: w
keyword: begin
keyword: end
divop:.
end parsing
print lexicalerrror
begin error printing
[Right quote missing!] 2:
[Right quote missing!] 3:
[Right quote missing!] 4:
```

### 单行注释时遇到换行符

```pascal
program test(input,output);
{开始运行
char a='w'
begin
	writeln(a);
end.
```

运行结果

```pascal
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
keyword: char
identifier: a
relop:=
letter: w
keyword: begin
identifier: writeln
divop:(
identifier: a
divop:)
divop:;
keyword: end
divop:.
end parsing
print lexicalerrror
begin error printing
[Unexpected '\n' before a singleline comment ends] 2:
```

### 多行注释之间遇到文件尾

```pascal
program test(input,output);
(*统计错误
char a='w'
begin
	writeln(a);
end.
```

运行结果

```
begin parsing
keyword: program
identifier: test
divop:(
identifier: input
divop:,
identifier: output
divop:)
divop:;
end parsing
print lexicalerrror
begin error printing
[Unexpected EOF before a multiline comment ends] 6:
```

## 综合测试

```pascal
(*
    涉及到的库函数：
    readln(): 读入一行;
    read(): 读入一个数;
    write(): 打印.
*)

var
    n,i: integer;
    a: array [0..100000] of integer;

procedure quickSort(l,r: integer);
var
    i, j, mid: integer;

begin
    if l >= r then exit;
    i := l;
    j := r;
    mid := a[(l + r) div 2];
    repeat
        while a[i] < mid do i := i+1;
        while a[j] > mid do j := j-1;
        if i <= j then
        begin
            a[0] := a[i];
            a[i] := a[j];
            a[j] := a[0];
            i := i+1;
            j := j-1;
        end;
    until i > j;
    quickSort(l,j);
    quickSort(i,r);
end;

begin
    readln(n);
    for i := 1 to n do
    read(a[i]);
    quickSort(1,n);
    for i := 1 to n do
    write(a[i],' ');
end.
```

运行结果

```pascal
begin parsing
keyword: var
identifier: n
divop:,
identifier: i
divop::
keyword: integer
divop:;
identifier: a
divop::
keyword: array
divop:[
integer:0
divop:.
divop:.
integer:100000
divop:]
keyword: of
keyword: integer
divop:;
keyword: procedure
identifier: quickSort
divop:(
identifier: l
divop:,
identifier: r
divop::
keyword: integer
divop:)
divop:;
keyword: var
identifier: i
divop:,
identifier: j
divop:,
identifier: mid
divop::
keyword: integer
divop:;
keyword: begin
keyword: if
identifier: l
relop:>=
identifier: r
keyword: then
identifier: exit
divop:;
identifier: i
ASSIGNOP: :=
identifier: l
divop:;
identifier: j
ASSIGNOP: :=
identifier: r
divop:;
identifier: mid
ASSIGNOP: :=
identifier: a
divop:[
divop:(
identifier: l
addop:+
identifier: r
divop:)
mulop:div
integer:2
divop:]
divop:;
keyword: repeat
keyword: while
identifier: a
divop:[
identifier: i
divop:]
relop:<
identifier: mid
keyword: do
identifier: i
ASSIGNOP: :=
identifier: i
addop:+
integer:1
divop:;
keyword: while
identifier: a
divop:[
identifier: j
divop:]
relop:>
identifier: mid
keyword: do
identifier: j
ASSIGNOP: :=
identifier: j
addop:-
integer:1
divop:;
keyword: if
identifier: i
relop:<=
identifier: j
keyword: then
keyword: begin
identifier: a
divop:[
integer:0
divop:]
ASSIGNOP: :=
identifier: a
divop:[
identifier: i
divop:]
divop:;
identifier: a
divop:[
identifier: i
divop:]
ASSIGNOP: :=
identifier: a
divop:[
identifier: j
divop:]
divop:;
identifier: a
divop:[
identifier: j
divop:]
ASSIGNOP: :=
identifier: a
divop:[
integer:0
divop:]
divop:;
identifier: i
ASSIGNOP: :=
identifier: i
addop:+
integer:1
divop:;
identifier: j
ASSIGNOP: :=
identifier: j
addop:-
integer:1
divop:;
keyword: end
divop:;
keyword: until
identifier: i
relop:>
identifier: j
divop:;
identifier: quickSort
divop:(
identifier: l
divop:,
identifier: j
divop:)
divop:;
identifier: quickSort
divop:(
identifier: i
divop:,
identifier: r
divop:)
divop:;
keyword: end
divop:;
keyword: begin
identifier: readln
divop:(
identifier: n
divop:)
divop:;
keyword: for
identifier: i
ASSIGNOP: :=
integer:1
keyword: to
identifier: n
keyword: do
identifier: read
divop:(
identifier: a
divop:[
identifier: i
divop:]
divop:)
divop:;
identifier: quickSort
divop:(
integer:1
divop:,
identifier: n
divop:)
divop:;
keyword: for
identifier: i
ASSIGNOP: :=
integer:1
keyword: to
identifier: n
keyword: do
identifier: write
divop:(
identifier: a
divop:[
identifier: i
divop:]
divop:,
letter:  
divop:)
divop:;
keyword: end
divop:.
end parsing
print lexicalerrror
begin error printing
```

