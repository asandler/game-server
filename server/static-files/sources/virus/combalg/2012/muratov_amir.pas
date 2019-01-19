program bot;

{$MODE DELPHI}
//{$APPTYPE CONSOLE}

uses
  SysUtils;


var i,j,x,x1,x2,x3,y1,y2,y3,ss,fx,fy,tx,ty:integer;
    a,b,ceat,cmove,use,dfe:array[-1..10,-1..10]of integer;
    s:string;
    mx,my:array[1..8]of integer;
    fb:boolean;

procedure dfs(x,y:integer);
var i:integer;
begin
use[x,y]:=1;
for i:=1 to 8 do begin
  if (a[x+mx[i],y+my[i]]=2) then
    ceat[x+mx[i],y+my[i]]:=1;
  if ((a[x+mx[i],y+my[i]]=1) or (a[x+mx[i],y+my[i]]=3)) and (use[x+mx[i],y+my[i]]=0) then
    dfs(x+mx[i],y+my[i]);
end;
end;


procedure dfs2(x,y:integer);
var i:integer;
begin
use[x,y]:=1;
for i:=1 to 8 do begin
  if (a[x+mx[i],y+my[i]]=0) then
    cmove[x+mx[i],y+my[i]]:=1;
  if ((a[x+mx[i],y+my[i]]=1)) and (use[x+mx[i],y+my[i]]=0) then
    dfs2(x+mx[i],y+my[i]);
end;
end;

procedure usedfs2();
var i,j:integer;
begin
fillchar(use,sizeof(use),0);
fillchar(cmove,sizeof(cmove),0);
for i:=0 to 9 do
  for j:=0 to 9 do
    if (a[i,j]=1) and (use[i,j]=0) then
      dfs2(i,j);
end;

procedure countdfe;
var i,l,r,cx,cy,j:integer;
    ochx,ochy:array[1..100]of integer;
begin
fillchar(dfe,sizeof(dfe),0);
fillchar(use,sizeof(use),0);
l:=1;
r:=1;
for i:=0 to 9 do
  for j:=0 to 9 do
    if a[i,j]=2 then begin
      ochx[r]:=i;
      ochy[r]:=j;
      inc(r);
      use[i,j]:=1;
    end;
if l=r then begin
for i:=0 to 9 do
  for j:=0 to 9 do
    dfe[i,j]:=4;
end;
while l<>r do begin
  cx:=ochx[l];
  cy:=ochy[l];
  inc(l);
  for i:=1 to 8 do begin
    if (use[cx+mx[i],cy+my[i]]=0) and (a[cx+mx[i],cy+my[i]]=0) then begin
      dfe[cx+mx[i],cy+my[i]]:=dfe[cx,cy]+1;
      use[cx+mx[i],cy+my[i]]:=1;
      ochx[r]:=cx+mx[i];
      ochy[r]:=cy+my[i];
      inc(r);
    end;
  end;
end;
end;

procedure usedfs();
var i,j:integer;
begin
countdfe;
usedfs2;
fillchar(use,sizeof(use),0);
fillchar(ceat,sizeof(ceat),0);
for i:=0 to 9 do
  for j:=0 to 9 do
    if (a[i,j]=1) and (use[i,j]=0) then
      dfs(i,j);
end;


procedure findtarget;
var i,j,k:integer;
begin
tx:=fx;
ty:=fy;
  for i:=0 to 9 do
    for j:=0 to 9 do
      if a[i,j]=4 then
        for k:=1 to 8 do
          if a[i+mx[k],j+my[k]]=2  then begin
            tx:=i+mx[k];
            ty:=j+my[k];
          end;

end;

procedure funnymove(var x,y:integer);
var i,j,ii,jj:integer;
begin
  if (fx=0) and not fb then
  if  (x=-1) then begin
    for i:=0 to 9 do begin
      ii:=i;
      jj:=0;
      while (ii>=0) and (jj<=9) do begin
        if (cmove[ii,jj]=1) and
        ((ii=9) or (ii=0) or (jj=9) or (jj=0)) and (x=-1) and (dfe[ii,jj]>2) then begin
          x:=ii;
          y:=jj;
        end;
        dec(ii);
        inc(jj);
      end;
    end;
    for i:=1 to 9 do begin
      ii:=9;
      jj:=i;
      while (ii>=0) and (jj<=9) do begin
        if (cmove[ii,jj]=1) and
        ((ii=9) or (ii=0) or (jj=9) or (jj=0)) and (x=-1) and (dfe[ii,jj]>2) then begin
          x:=ii;
          y:=jj;
        end;
        dec(ii);
        inc(jj);
      end;
    end;
  end else begin end
  else
  if (not fb) and (x=-1) then begin
    for i:=9 downto 1 do begin
      ii:=9;
      jj:=i;
      while (ii>=0) and (jj<=9) do begin
        if (cmove[ii,jj]=1) and
        ((ii=9) or (ii=0) or (jj=9) or (jj=0)) and (x=-1) and (dfe[ii,jj]>2) then begin
          x:=ii;
          y:=jj;
        end;
        dec(ii);
        inc(jj);
      end;
    end;

    for i:=9 downto 0 do begin
      ii:=i;
      jj:=0;
      while (ii>=0) and (jj<=9) do begin
        if (cmove[ii,jj]=1) and
        ((ii=9) or (ii=0) or (jj=9) or (jj=0)) and (x=-1) and (dfe[ii,jj]>2) then begin
          x:=ii;
          y:=jj;
        end;
        dec(ii);
        inc(jj);
      end;
    end;
  end;
end;


procedure onemove(var x,y:integer);
var i,j:integer;
    min:extended;
begin
  findtarget;
  usedfs;
  min:=maxint;
  if x=-1 then
  for i:=0 to 9 do
    for j:=0 to 9 do
      if (ceat[i,j]=1) and  (min>sqrt((i-tx)*(i-tx)+(j-ty)*(j-ty))) then begin
        x:=i;
        y:=j;
        min:=sqrt((i-tx)*(i-tx)+(j-ty)*(j-ty));
      end;
  funnymove(x,y);
  if a[fx,fy]=0 then begin
    x:=fx;
    y:=fy;
  end else begin
    if (x=-1) and (not fb) then begin
      usedfs;
      min:=maxint;
      for i:=0 to 9 do
        for j:=0 to 9 do
          if (dfe[i,j]>3) and (cmove[i,j]=1) and  (min>sqrt((i-tx)*(i-tx)+(j-ty)*(j-ty))) then begin
            x:=i;
            y:=j;
            min:=sqrt((i-tx)*(i-tx)+(j-ty)*(j-ty));
          end;
    end;
    if x=-1 then begin
      usedfs;
      min:=maxint;
      for i:=0 to 9 do
        for j:=0 to 9 do
          if (cmove[i,j]=1) and  (min>sqrt((i-tx)*(i-tx)+(j-ty)*(j-ty))) then begin
            x:=i;
            y:=j;
            min:=sqrt((i-tx)*(i-tx)+(j-ty)*(j-ty));
          end;
    end;
  end;
end;


procedure move();
begin
{if fx=0 then begin
  tx:=9;
  ty:=9;
end else begin
  tx:=0;
  ty:=0;
end;}

tx:=fx;
ty:=fy;


x1:=-1;
y1:=-1;
x2:=-1;
y2:=-1;
x3:=-1;
y3:=-1;
onemove(x1,y1);
if x1<>-1 then
  if a[x1,y1]=0 then
    a[x1,y1]:=1
  else begin
    a[x1,y1]:=3;
    fb:=true;
  end;

onemove(x2,y2);
if x2<>-1 then
  if a[x2,y2]=0 then
    a[x2,y2]:=1
  else begin
    a[x2,y2]:=3;
    fb:=true;
  end;
onemove(x3,y3);
if x3<>-1 then
  if a[x3,y3]=0 then
    a[x3,y3]:=1
  else begin
    a[x3,y3]:=3;
    fb:=true;
  end;
if (x1=-1) then halt;
if (x2=-1) then
  write(x1,y1)
else
  if (x3=-1) then
    write(x1,y1,x2,y2)
  else
    write(x1,y1,x2,y2,x3,y3);
if x3=-1 then halt;
writeln;
flush(output);

{writeln;
for i:=0 to 9 do begin
  for j:=0 to 9 do begin
    if a[i,j]=0 then
      write('.');
    if a[i,j]=1 then
      write('x');
    if a[i,j]=2 then
      write('o');
    if a[i,j]=3 then
      write('X');
    if a[i,j]=4 then
      write('O');
  end;
  writeln;
end;}
end;


begin
  mx[1]:=1; my[1]:=0;
  mx[2]:=0; my[2]:=1;
  mx[3]:=-1; my[3]:=0;
  mx[4]:=0; my[4]:=-1;
  mx[5]:=1; my[5]:=1;
  mx[6]:=1; my[6]:=-1;
  mx[7]:=-1; my[7]:=1;
  mx[8]:=-1; my[8]:=-1;
  fillchar(a,sizeof(a),0);
  fb:=false;
  for i:=-1 to 10 do begin
    a[i,-1]:=-1;
    a[-1,i]:=-1;
    a[10,i]:=-1;
    a[i,10]:=-1;
  end;               //1=svoi,2=chuzhie,3=chuzhoi trup,4=svoi trup,0=pusto
  readln(x);
  if x=1 then begin
    s:='1';
    fx:=0;
    fy:=0;
    move;
  end else begin
    fx:=9;
    fy:=9;
    readln(s);
    x1:=ord(s[1])-ord('0');
    y1:=ord(s[2])-ord('0');
    x2:=ord(s[3])-ord('0');
    y2:=ord(s[4])-ord('0');
    x3:=ord(s[5])-ord('0');
    y3:=ord(s[6])-ord('0');
    if a[x1,y1]<>1 then
      a[x1,y1]:=2
    else begin
      a[x1,y1]:=4;
      fb:=true;
    end;
    if a[x2,y2]<>1 then
      a[x2,y2]:=2
    else begin
      a[x2,y2]:=4;
      fb:=true;
    end;
    if a[x3,y3]<>1 then
      a[x3,y3]:=2
    else begin
      a[x3,y3]:=4;
      fb:=true;
    end;
    move;
  end;
  while true do begin
    readln(s);
    x1:=ord(s[1])-ord('0');
    y1:=ord(s[2])-ord('0');
    x2:=ord(s[3])-ord('0');
    y2:=ord(s[4])-ord('0');
    x3:=ord(s[5])-ord('0');
    y3:=ord(s[6])-ord('0');
    if a[x1,y1]<>1 then
      a[x1,y1]:=2
    else begin
      a[x1,y1]:=4;
      fb:=true;
    end;
    if a[x2,y2]<>1 then
      a[x2,y2]:=2
    else begin
      a[x2,y2]:=4;
      fb:=true;
    end;
    if a[x3,y3]<>1 then
      a[x3,y3]:=2
    else begin
      a[x3,y3]:=4;
      fb:=true;
    end;
    move;
  end;
end.
