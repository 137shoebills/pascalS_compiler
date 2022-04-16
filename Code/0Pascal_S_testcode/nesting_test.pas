program main(x);	    {Pascal_S语法：program IDENTIFIER(IDENTIFIER,...);}
var a, b, ret: integer;     {全局变量}

procedure test;			{无参过程：省略括号}
    procedure inner;
	    procedure inner1(a,b:integer;var ret:integer);
		    function func(a,b:integer):integer;
		    begin
			    func:=a+b;
		    end;    { end of function func }  
		
	    {inner1}
	    begin
			a:=1;
			b:=2;
			a:=func(a,b);
			ret:=a+b;
	    end;    { end of procedure inner1 }  
	
    	    (*不支持跨过程调用
	    procedure inner2(x,y:integer;var ret:integer);
	    begin
		    writeln(func(a,b));
	    end;
	    *)
	
	    procedure noPara;
	    begin
		    writeln('n');
	    end;    { end of procedure noPara }  

    {inner}
    begin
		inner1(a,b,ret);
		(*writeln(func(a,b));*) {不支持隔层调用：func属于inner1，inner不能调用}
		noPara;
    end;    { end of procedure inner }  
	
{test}
begin
	inner;
end;    { end of procedure test }  

{main}
begin
	a:=10;
	b:=20;
        test;
end.
