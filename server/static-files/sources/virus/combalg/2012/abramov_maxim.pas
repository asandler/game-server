program v3;

{$MODE DELPHI}

uses
  SysUtils, Math;

const
  dx: array[1..8] of Integer = (1, 1, 0, -1, -1, -1, 0, 1);
  dy: array[1..8] of Integer = (0, 1, 1, 1, 0, -1, -1, -1);

type
  Point = record
    x, y: integer;
  end;
  PointArray = array[1..100] of Point;
  IntegerArray = array[1..100] of Integer;

var
  size, es, ms, des, dms: integer;
  list, ml, el, del, dml: PointArray;
  used: array[0..9, 0..9] of boolean;
  a: array[0..9, 0..9] of integer;
  delvl, dmlvl: IntegerArray;
  queue: array[1..50] of Point;
  
procedure dfs(u: Point); forward;

procedure eaten(x, y: integer);
var
  i: integer;
begin
  for i := 1 to size do
    if (list[i].x = x) and (list[i].y = y) then begin
      list[i] := list[size];
      dec(size);
      a[x, y] := -1;
      break;
    end;
end;

procedure move();
var
  i, l, r: integer;
  t: Point;
begin
  for i := 1 to 100 do begin
    l := Random(ms)+1;
    r := Random(ms)+1;
    t := ml[l];
    ml[l] := ml[r];
    ml[r] := t;
  end;
  i := random(ms)+1;
  inc(size);
  list[size] := ml[i];
  a[ml[i].x, ml[i].y] := 1;
  used[ml[i].x, ml[i].y] := false;
  write(ml[i].x, ml[i].y);
  ml[i] := ml[ms];
  Dec(ms);
end;

procedure eat();
var
  i, l, r: integer;
  t: Point;
begin
  for i := 1 to 100 do begin
    l := Random(es)+1;
    r := Random(es)+1;
    t := el[l];
    el[l] := el[r];
    el[r] := t;
  end;
  i := random(es)+1;
  a[el[i].x, el[i].y] := -2;
  used[el[i].x, el[i].y] := false;
  dfs(el[i]);
  write(el[i].x, el[i].y);
  el[i] := el[es];
  dec(es);
end;

procedure sort(var size: Integer; var list: PointArray; var lvl: IntegerArray);
var
  i, j, p: Integer;
  t: Point;
begin
  for i := 1 to size-1 do
    for j := 1 to size-i do
      if lvl[j] < lvl[j+1] then begin
        t := list[j];
        list[j] := list[j+1];
        list[j+1] := t;
        p := lvl[j];
        lvl[j] := lvl[j+1];
        lvl[j+1] := p;
      end;
end;

procedure shuffle(var size: Integer; var list: PointArray; var lvl: IntegerArray);
var
  i, l, r, p: Integer;
  t: Point;
begin
  for i := 1 to 100 do begin
    l := Random(size)+1;
    r := Random(size)+1;
    t := list[l];
    list[l] := list[r];
    list[r] := t;
    p := lvl[l];
    lvl[l] := lvl[r];
    lvl[r] := p;
  end;
end;

procedure dangereat();
begin
  shuffle(des, del, delvl);
  sort(des, del, delvl);
  a[del[1].x, del[1].y] := -2;
  used[del[1].x, del[1].y] := false;
  dfs(del[1]);
  write(del[1].x, del[1].y);
  del[1] := del[des];
  delvl[1] := delvl[des];
  dec(des);
end;

procedure dangermove();
begin
  a[dml[1].x, dml[1].y] := 1;
  used[dml[1].x, dml[1].y] := false;
  inc(size);
  list[size] := dml[1];
  write(dml[1].x, dml[1].y);
  dml[1] := dml[dms];
  dmlvl[1] := dmlvl[dms];
  dec(dms);
end;

function dangerous(u: Point): Integer;
var
  i: Integer;
  v: Point;
begin
  Result := 0;
  for i := 1 to 8 do begin
    v.x := u.x + dx[i];
    v.y := u.y + dy[i];
    if not (v.x in [0..9]) or not (v.y in [0..9]) then
      Continue;
    if a[v.x, v.y] = 1 then begin
      Result := MaxInt;
      Exit;
    end;
    if (a[v.x, v.y] = 0) then
      inc(Result)
    else if (a[v.x, v.y] = -1) then
      Result := Result + 10;
  end;
end;

procedure push(x: Point; var tail: Integer);
begin
  queue[tail] := x;
  Inc(tail);
end;

function pop(var head: Integer): Point;
begin
  result := queue[head];
  inc(head);
end;

function check(s: Point): Integer;
var
  d: array[0..9, 0..9] of Integer;
  qt, qh, i: Integer;
  u, v: Point;
begin
  FillChar(d, SizeOf(d), -1);
  d[s.x, s.y] := 0;
  qt := 1;
  qh := 1;
  push(s, qt);
  Result := 3;
  while not (qt = qh) do begin
    u := pop(qh);
    for i := 1 to 8 do begin
      v.x := u.x + dx[i];
      v.y := u.y + dy[i];
      if not (v.x in [0..9]) or not (v.y in [0..9]) then
        continue;
      if (a[v.x, v.y] <> 0) and (abs(a[v.x, v.y]) <> 2) then
        Continue;
      if d[v.x, v.y] = -1 then begin
        d[v.x, v.y] := d[u.x, u.y] + 1;
        if a[v.x, v.y] = 2 then begin
          Result := Min(Result, d[v.x, v.y]);
          Continue;
        end else if a[v.x, v.y] = -2 then begin
          Result := min(Result, -d[v.x, v.y]);
          Continue;
        end;
        if d[v.x, v.y] < 2 then
          push(v, qt);
      end;
    end;
  end;
  Result := 3 - Result;
end;

procedure dfs(u: Point);
var
  i, q: Integer;
  v: Point;
begin
  used[u.x, u.y] := true;
  for i := 1 to 8 do begin
    v.x := u.x + dx[i];
    v.y := u.y + dy[i];
    if not (v.x in [0..9]) or not (v.y in [0..9]) then
      continue;
    if (a[v.x, v.y] = 0) and (a[u.x, u.y] = 1) and (not used[v.x, v.y]) then begin
      q := check(v);
      if q > 0 then begin
        inc(dms);
        dml[dms] := v;
        dmlvl[dms] := q;
        used[v.x, v.y] := True;
      end else begin
        inc(ms);
        ml[ms] := v;
        used[v.x, v.y] := true;
      end;
    end;
    if (a[v.x, v.y] = 2) and (not used[v.x, v.y]) then begin
      q := dangerous(v);
      if q > 0 then begin
        inc(des);
        del[des] := v;
        delvl[des] := q;
        used[v.x, v.y] := True;
      end else begin
        inc(es);
        el[es] := v;
        used[v.x, v.y] := true;
      end;
    end;
    if ((a[v.x, v.y] = 1) or (a[v.x, v.y] = -2)) and (not used[v.x, v.y]) then
      dfs(v);
  end;
end;

procedure makeMove(c: integer);
var
  i: integer;
begin
  for i := 1 to size do
    if not used[list[i].x, list[i].y] then
      dfs(list[i]);
  shuffle(dms, dml, dmlvl);
  sort(dms, dml, dmlvl);
  if (es = 0) and (ms = 0) and (des = 0) and (dms = 0) then
    halt(0)
  else if (es = 0) and (des = 0) then begin
    if (dms > 0) and (dmlvl[1] + c >= 5) then
      dangermove()
    else if (ms > 0) then
      move()
    else
      dangermove()
  end else if (ms = 0) and (dms = 0) then begin
    if des > 0 then
      dangereat()
    else
      eat();
  end else begin
    i := random(100)+1;
    if des > 0 then
      dangereat()
    else if i > random(50)+1 then
      if des > 0 then
        dangereat()
      else
        eat()
    else
    if (dms > 0) and (dmlvl[1] + c >= 5) then
      dangermove()
    else if (ms > 0) then
      move()
    else
      dangermove();
  end;
end;

procedure init();
var
  n: Integer;
begin
  readln(n);
  size := 0;
  fillchar(a, sizeof(a), 0);
  if n = 1 then begin
    ml[1].x := 0;
    ml[1].y := 0;
    ms := 1;
    makeMove(3);
    makeMove(2);
    makeMove(1);
    writeln;
    flush(output);
    ms := 0;
    es := 0;
    des := 0;
    dms := 0;
  end else begin
    ml[1].x := 9;
    ml[1].y := 9;
    ms := 1;
  end;
end;

procedure game();
var
  x, y, q: Integer;
  s: string;
begin
  while True do begin
    fillchar(used, sizeof(used), false);
    readln(s);
    q := 0;
    while s <> '' do begin
      y := StrToInt(s[2]);
      x := StrToInt(s[1]);
      Delete(s, 1, 2);
      inc(q);
      if a[x, y] = 0 then
        a[x, y] := 2
      else if a[x, y] = 1 then
        eaten(x, y);
    end;
    if q < 3 then
      Halt(0);
    makeMove(3);
    makeMove(2);
    makeMove(1);
    writeln;
    flush(output);
    ms := 0;
    es := 0;
    des := 0;
    dms := 0;
  end;
end;

begin
  randomize;
  init();
  game();
end.
