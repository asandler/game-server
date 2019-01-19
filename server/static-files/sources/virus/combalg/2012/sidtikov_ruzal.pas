{$APPTYPE CONSOLE}
{$R+}
{$O-}

uses
  SysUtils;

var
  a, was, d, used, kr, ff: array[-1..10, -1..10] of integer;
  sv, q: array[1..2, 1..1000] of integer;
  dy, dx, hod: array[1..8] of integer;
  s: string;
  r, rq, l, vkl, ans, r_2, vy, vx, sy, sx, ok, kto_hodit, i, j, sp: integer;

procedure init;
var
  i: integer;
begin
  fillchar(a, sizeof(a), 0);
  for i := -1 to 10 do begin
    a[-1, i] := -1;
    a[i, -1] := -1;
    a[10, i] := -1;
    a[i, 10] := -1;
  end;
  dy[1] := -1; dx[1] := 0;
  dy[2] := -1; dx[2] := 1;
  dy[3] := 0; dx[3] := 1;
  dy[4] := 1; dx[4] := 1;
  dy[5] := 1; dx[5] := 0;
  dy[6] := 1; dx[6] := -1;
  dy[7] := 0; dx[7] := -1;
  dy[8] := -1; dx[8] := -1;
end;

function f(x: char): integer;
begin
  f := ord(x) - 48;
end;

procedure make(s: string);
var
  y1, x1, y2, x2, y3, x3: integer;
begin
  if length(s) > 0 then begin
    y1 := f(s[1]);
    x1 := f(s[2]);
    if a[y1, x1] = 0 then
      a[y1, x1] := 2
    else
      a[y1, x1] := 4;
  end;
  if length(s) > 2 then begin
    y2 := f(s[3]);
    x2 := f(s[4]);
    if a[y2, x2] = 0 then
      a[y2, x2] := 2
    else
      a[y2, x2] := 4;
  end;
  if length(s) > 4 then begin
    y3 := f(s[5]);
    x3 := f(s[6]);
    if a[y3, x3] = 0 then
      a[y3, x3] := 2
    else
      a[y3, x3] := 4;
  end;
end;

procedure add(y, x: integer);
begin
  inc(r);
  sv[1, r] := x;
  sv[2, r] := y;
  was[y, x] := 1;
end;

procedure push(y, x: integer);
begin
  inc(rq);
  q[1, rq] := x;
  q[2, rq] := y;
end;

function is_empty: boolean;
begin
  is_empty := (l > rq);
end;

function popx: integer;
begin
  popx := q[1, l];
end;

function popy: integer;
begin
  popy := q[2, l];
  if not(is_empty) then
    inc(l);
end;

procedure initbfs(y, x: integer);
begin
  fillchar(used, sizeof(used), 0);
  fillchar(d, sizeof(d), 0);
  rq := 0;
  l := 1;
  push(y, x);
  used[y, x] := 1;
end;

procedure bfs_rast(y, x: integer);
var
  cur1, cur2, k: integer;
begin
  initbfs(y, x);
  while not(is_empty) do begin
    cur1 := popx;
    cur2 := popy;
    for k := 1 to 8 do
      if (a[cur2 + dy[k], cur1 + dx[k]] = 0) and (d[cur2, cur1] < 3) and (used[cur2 + dy[k], cur1 + dx[k]] = 0) then begin
        push(cur2 + dy[k], cur1 + dx[k]);
        used[cur2 + dy[k], cur1 + dx[k]] := 1;
        d[cur2 + dy[k], cur1 + dx[k]] := d[cur2, cur1] + 1;
      end;
  end;
end;

function hm(w: integer): integer;
begin
  if w = 1 then
    hm := 3
  else if w = 3 then
    hm := 1
  else
    hm := 2;
end;

procedure dfs(y, x, kto, fort: integer);
var
  heh, neheh: integer;
begin
  used[y, x] := 1;
  for heh := 1 to 8 do
    if a[y + dy[heh], x + dx[heh]] = kto then begin
        vkl := 1;
        exit;
    end;
  for neheh := 1 to 8 do
    if (a[y + dy[neheh], x + dx[neheh]] = fort) and (used[y + dy[neheh], x + dx[neheh]] = 0) then
      dfs(y + dy[neheh], x + dx[neheh], kto, fort);
end;

procedure search_vragoff;
var
  i, j, k: integer;
begin
  fillchar(used, sizeof(used), 0);
  for i := 0 to 9 do
    for j := 0 to 9 do
      if a[i, j] = 2 then begin
        for k := 1 to 8 do
          if (d[i + dy[k], j + dx[k]] > 0) then
            kr[i + dy[k], j + dx[k]] := 3;
      end else if (a[i, j] = 4) and (used[i, j] = 0) then begin
        vkl := 0;
        dfs(i, j, 2, 4);
        if vkl = 1 then
          for k := 1 to 8 do
            if (d[i + dy[k], j + dx[k]] > 0) then
              kr[i + dy[k], j + dx[k]] := 3;
      end;
end;

procedure analyzing;
var
  i, j, k: integer;
begin
  for i := 0 to 9 do
    for j := 0 to 9 do
      if a[i, j] = 1 then
        for k := 1 to 8 do
          if (a[i + dy[k], j + dx[k]] = 0) then begin
            bfs_rast(i, j);
            search_vragoff;
          end;
end;

procedure forming;
var
  i, j, k: integer;
begin
  fillchar(was, sizeof(was), 0); // 0
  fillchar(kr, sizeof(kr), 0);
  r := 0;
  for i := 0 to 9 do
    for j := 0 to 9 do
      if a[i, j] = 1 then
        for k := 1 to 8 do begin
          if (a[i + dy[k], j + dx[k]] = 0) and (was[i + dy[k], j + dx[k]] = 0) then
            add(i + dy[k], j + dx[k]);
          if a[i + dy[k], j + dx[k]] = 2 then
            kr[i + dy[k], j + dx[k]] := 4;
        end;
end;

procedure osobo_opasni;
var
  i, j, ok: integer;
begin
  while ans < 6 do begin
    ok := 0;
    for i := 0 to 9 do
      for j := 0 to 9 do begin
        if ans = 6 then
          exit;
        if (kr[i, j] = 4) then begin
          a[i, j] := 3;
          inc(ans, 2);
          hod[ans - 1] := i;
          hod[ans] := j;
          ok := 1;
          forming;
          analyzing;
        end;
      end;
    if ok = 0 then
      break;
  end;
end;

procedure check_2(y, x, kto: integer);
var
  k: integer;
begin
  r_2 := 0;
  for k := 1 to 8 do
    if a[y + dy[k], x + dx[k]] = kto then begin
      r_2 := 1;
      vy := y + dy[k];
      vx := x + dx[k];
      exit;
    end;
end;

procedure find_2(y, x: integer);
var
  k, nek: integer;
begin
  ff[y, x] := 1;
  for k := 1 to 8 do
    if (a[y + dy[k], x + dx[k]] = 2) and (was[y + dy[k], x + dx[k]] = 0) then
      add(y + dy[k], x + dx[k]);
  for nek := 1 to 8 do
    if (a[y + dy[nek], x + dx[nek]] = 3) and (ff[y + dy[nek], x + dx[nek]] = 0) then
      find_2(y + dy[nek], x + dx[nek]);
end;

function count(y, x: integer): extended;                  // kr = 0 -> forti -> kr = 1 -> kr = 2
begin
  count := sqrt( (x - sx) * (x - sx) + (y - sy) * (y - sy) );
end;

procedure chose(c: integer);
var
  mn: extended;
  i: integer;
begin
  mn := 1000000;
  for i := 1 to r do
    if count(sv[2, i], sv[1, i]) < mn then begin
      mn := count(sv[2, i], sv[1, i]);
      vy := sv[2, i];
      vx := sv[1, i];
      ok := 1;
    end;
end;

procedure atack_po_fortam;
var
  i, j, da: integer;
begin
  fillchar(was, sizeof(was), 0); // 2
  fillchar(ff, sizeof(ff), 0);
  while ans < 6 do begin
    da := 0;
    for i := 0 to 9 do
      for j := 0 to 9 do
        if (a[i, j] = 3) and (ff[i, j] = 0) then begin
          if ans = 6 then
            exit;
          vkl := 0;
          fillchar(used, sizeof(used), 0);
          dfs(i, j, 1, 3);
          if vkl = 1 then begin
            r := 0; // spisok
            find_2(i, j);
            ok := 0;
            chose(2); // ok
            if (ok = 1) and (ff[vy, vx] = 0) then begin
              a[vy, vx] := 3;
              inc(ans, 2);
              hod[ans - 1] := vy;
              hod[ans] := vx;
              ff[vy, vx] := 1;
              da := 1;
            end;
          end;
        end;
    if da = 0 then
      exit;
  end;
end;

procedure otkl_forta;
var
  i, k, ok: integer;
begin
  forming;
  analyzing;
  while ans < 6 do begin
    if ans >= 4 then
      exit;
    ok := 0;
    for i := 1 to r do begin
      for k := 1 to 8 do
        if a[sv[2, i] + dy[k], sv[1, i] + dx[k]] = 2 then begin
          check_2(sv[2, i] + dy[k], sv[1, i] + dx[k], 4);
          if r_2 = 1 then begin
            a[sv[2, i], sv[1, i]] := 1;
            a[sv[2, i] + dy[k], sv[1, i] + dx[k]] := 3;
            inc(ans, 4);
            hod[ans - 3] := sv[2, i];
            hod[ans - 2] := sv[1, i];
            hod[ans - 1] := sv[2, i] + dy[k];
            hod[ans] := sv[1, i] + dx[k];
            ok := 1;
            break;
          end;
        end;
      if ok = 1 then
        break;
    end;
    if ok = 0 then
      exit
    else begin
      forming;
      analyzing;
      if ans = 6 then
        exit;
      osobo_opasni;
    end;
  end;
end;

procedure ataaack;
var
  i, ok: integer;
begin
  forming;
  analyzing;
  while ans < 6 do begin
    ok := 0;
    if ans > 0 then
      break;
    for i := 1 to r do
      if kr[sv[2, i], sv[1, i]] = 3 then begin
        check_2(sv[2, i], sv[1, i], 2);
        if r_2 = 1 then begin
          a[sv[2, i], sv[1, i]] := 1;
          a[vy, vx] := 3;
          inc(ans, 4);
          hod[ans - 3] := sv[2, i];
          hod[ans - 2] := sv[1, i];
          hod[ans - 1] := vy;
          hod[ans] := vx;
          ok := 1;
          break;
        end;
      end;
    if ok = 0 then
      break
    else begin
      forming;
      analyzing;
      osobo_opasni;
      if ans = 6 then
        exit;
      atack_po_fortam;
      break;
    end;
  end;
end;

procedure spokoistvie;
var
  i, ok: integer;
begin
  forming;
  analyzing;
  fillchar(used, sizeof(used), 0);
  while ans < 6 do begin
    ok := 0;
    for i := 1 to r do begin
      if ans = 6 then
        exit;
      if ((sv[2, i] = sy) or (sv[1, i] = sx)) and (kr[sv[2, i], sv[1, i]] = 0) and (used[sv[2, i], sv[1, i]] = 0) then begin
        a[sv[2, i], sv[1, i]] := 1;
        inc(ans, 2);
        hod[ans - 1] := sv[2, i];
        hod[ans] := sv[1, i];
        used[sv[2, i], sv[1, i]] := 1;
        ok := 1;
      end;
    end;
    if ok = 0 then
      break;
    forming;
    analyzing;
    osobo_opasni;
  end;
end;

procedure chosempty;
var
  i: integer;
  mn: extended;
begin
  mn := 1000000;
  for i := 1 to r do
    if ((a[sv[2, i], sv[1, i]] = 0) and (kr[sv[2, i], sv[1, i]] < 3)) and (count(sv[2, i], sv[1, i]) < mn) then begin
      mn := count(sv[2, i], sv[1, i]);
      vy := sv[2, i];
      vx := sv[1, i];
      sp := 1;
    end;
end;

procedure usuallyhod;
var
  ok: integer;
begin
  while ans < 6 do begin
    ok := 0;
    forming;
    analyzing;
    osobo_opasni;
    if ans = 6 then
      exit;
    otkl_forta;
    if ans = 6 then
      exit;
    atack_po_fortam;
    if ans = 6 then
      exit;
    sp := 0;
    chosempty;
    if sp = 1 then begin
      a[vy, vx] := 1;
      inc(ans, 2);
      hod[ans - 1] := vy;
      hod[ans] := vx;
      ok := 1;
    end;
    if ok = 0 then
      break;
  end;
end;

procedure na_vsyakiy;
begin
  while ans < 6 do begin
    forming;
    analyzing;
    osobo_opasni;
    if ans = 6 then
      exit;
    otkl_forta;
    if ans = 6 then
      exit;
    atack_po_fortam;
    if ans = 6 then
      exit;
    ok := 0;
    chose(1);
    if ok = 1 then begin
      a[vy, vx] := 1;
      inc(ans, 2);
      hod[ans - 1] := vy;
      hod[ans] := vx;
      ok := 1;
    end;
    if ok = 0 then
      exit;
  end;
end;

procedure go;
var
  i: integer;
begin
  forming;      // spisok svobodnih hodov, poisk superopasnih
  analyzing;    // pomechaem opasnix
  for i := 1 to 6 do
    hod[i] := -1;
  ans := 0;
  osobo_opasni;
  if ans = 6 then
    exit;
  otkl_forta;
  if ans  = 6 then
    exit;
  atack_po_fortam;
  if ans = 6 then
    exit;
  spokoistvie;
  if ans = 6 then
    exit;
  usuallyhod;
  if ans = 6 then
    exit;
  ataaack;
  if ans = 6 then
    exit;
  na_vsyakiy;
  if ans = 6 then
    exit;
  halt;
end;

begin
  init;
  readln(kto_hodit);
  if kto_hodit = 1 then begin
    sy := 0;
    sx := 0;
    a[0, 0] := 1;
    a[0, 1] := 1;
    a[1, 0] := 1;
    writeln('000110');
    flush(output);
  end else begin
    readln(s);
    make(s);
    sy := 9;
    sx := 9;
    a[9, 9] := 1;
    a[9, 8] := 1;
    a[8, 9] := 1;
    writeln('999889');
    flush(output);
  end;
  while true do begin
    readln(s);
    make(s);
    go;
    for i := 1 to 6 do
      if hod[i] <> -1 then
        write(hod[i]);
    writeln;
    {for i := 0 to 9 do begin
      for j := 0 to 9 do
        write(a[i, j]);
      writeln;
    end;   }
    flush(output);
  end;
end.
