program Virus_War_Random;

{$MODE DELPHI}

uses
  SysUtils;

const
  My = 0;
  Enemy = 1;
  MyConquer = 2;
  EnemyConquer = 3;
  Free = 4;
  inf = 100;

var
  r, p, x: integer;
  f: array[0..9, 0..9] of byte;
  dist: array[0..9, 0..9] of integer;
  s: string;

procedure init;
var
  i, j: integer;
begin
  for i := 0 to 9 do
    for j := 0 to 9 do
      f[i, j] := Free;
end;

procedure initDist;
var
  i, j: integer;
begin
  for i := 0 to 9 do
    for j := 0 to 9 do
      dist[i, j] := inf;
end;

procedure initField;
begin
  init;
  initDist;
end;

procedure dfs(x, y, d: integer);
var
  dx, dy, xn, yn: integer;
begin
  dist[x, y] := d;
  for dx := -1 to 1 do
    for dy := -1 to 1 do begin
      xn := x + dx;
      yn := y + dy;
      if (0 > xn) or (xn > 9) or (0 > yn) or (yn > 9) then
        continue;
      if dist[x, y] >= dist[xn, yn] then
        continue;
      case f[x, y] of
        My: begin
          case f[xn, yn] of
            EnemyConquer: continue;
            My, MyConquer: dfs(xn, yn, d);
            Free, Enemy:
              if dist[x, y] + 1 < dist[xn, yn] then
                dfs(xn, yn, d + 1);
          end;
        end;
        MyConquer: begin
          case f[xn, yn] of
            EnemyConquer, Free: continue;
            My, MyConquer: dfs(xn, yn, d);
            Enemy:
              if dist[x, y] + 1 < dist[xn, yn] then
                dfs(xn, yn, d + 1);
          end;
        end;
        Enemy: begin
          if (f[xn, yn] = Enemy) and (dist[x, y] + 1 < dist[xn, yn]) then
            dfs(xn, yn, d + 1);
        end;
        EnemyConquer: continue;
        Free: begin
          case f[xn, yn] of
            Enemy, EnemyConquer: continue;
            My, MyConquer: dfs(xn, yn, d);
            Free:
              if dist[x, y] + 1 < dist[xn, yn] then
                dfs(xn, yn, d + 1);
          end;
        end;
      end;
    end;
end;

procedure makeEnemyMove(x, y: integer);
begin
  if f[x, y] = Free then
    f[x, y] := Enemy
  else if f[x, y] = My then
    f[x, y] := EnemyConquer
  else
    writeln('Impossible move Player2 - ', x, ' ', y);
end;

function makeMyMove(c: integer): String;
var
  i, j, x, y, count: integer;
  vx, vy: array[0..99] of integer;
begin
  initDist;
  x := 0;
  y := 0;
  for i := 0 to 9 do
    for j := 0 to 9 do
      if (f[i, j] = My) and (dist[i, j] <> 0) then
        dfs(i, j, 0);
  if c = 0 then begin
    result := '';
    exit;
  end;
  if f[0, 0] <> Free then begin
    count := 0;
    for i := 0 to 9 do
      for j := 0 to 9 do
        if (dist[i, j] = 1) and ((f[i, j] = Free) or (f[i, j] = Enemy)) then begin
          vx[count] := i;
          vy[count] := j;
          inc(count);
          if f[i, j] = Enemy then begin
            x := i;
            y := j;
          end;
        end;
    if count > 0 then begin
      count := random(count);
      if (x = 0) and (y = 0) then begin
        x := vx[count];
        y := vy[count];
      end;
    end else begin
      // writeln('I lose.');
      result := '';
      exit;
    end;
  end;
  if f[x, y] = Free then
    f[x, y] := My
  else if f[x, y] = Enemy then
    f[x, y] := MyConquer
  else
    writeln('Impossible move Player1 - ', x, ' ', y);
  x := abs(r - x);
  y := abs(r - y);
  result := IntToStr(x) + IntToStr(y) + makeMyMove(c - 1);
end;

procedure print;
var
  i, j: integer;
begin
  for i := 0 to 9 do begin
    for j := 0 to 9 do
      case f[i, j] of
        My: write('X ');
        Enemy: write('O ');
        MyConquer: write('@ ');
        EnemyConquer: write('# ');
        Free: write('* ');
      end;
    writeln;
  end;
end;

procedure printDist;
var
  i, j: integer;
begin
  for i := 0 to 9 do begin
    for j := 0 to 9 do
      if dist[i, j] < 10 then
        write(' ', dist[i, j], ' ')
      else
        write(' i ');
    writeln;
  end;
end;


begin
  initField;
  readln(p);
  if p = 1 then
    r := 0
  else
    r := 9;
  s := '000000';
  while (length(s) = 6) or (p = 1) do begin
    if p = 2 then begin
      readln(s);
      x := ord('0');
      if length(s) >= 2 then
        makeEnemyMove(abs(ord(s[1]) - x - r), abs(ord(s[2]) - x - r));
      if length(s) >= 4 then
        makeEnemyMove(abs(ord(s[3]) - x - r), abs(ord(s[4]) - x - r));
      if length(s) = 6 then
        makeEnemyMove(abs(ord(s[5]) - x - r), abs(ord(s[6]) - x - r));
      p := 1;
      // writeln;
    end else begin
      p := 2;
      s := makeMyMove(3);
      writeln(s);
      // print;
    end;
    flush(output);
  end;
end.
