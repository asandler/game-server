{$A8,B-,C+,D+,E-,F-,G+,H+,I+,J-,K-,L+,M-,N+,O+,P+,Q-,R-,S-,T-,U-,V+,W-,X+,Y+,Z1}

{$MODE DELPHI}

uses
    SysUtils;

const
    n = 9;
    c = 4;
    f = 4;
    xx = 4;
    max_kol = 300000;
    mx = 200;
    con = 3.78;

type
    int = integer;
    ext = extended;
    point = record
                x, y : int;
            end;
    ar = array[0..10000] of point;
    matr = array[0..20, 0..20] of int;

// 1 - mine; 2 - enemy's; 3 - my fort; 4 - enemy's fort

var
    go : array[0..10] of point;
    a, a2, b, dist, b1, b2, dist1, dist2 : matr;
    qeue : array[0..100100] of point;
    move : array[0..max_kol, 0..3] of point;
    cur_mv : array[0..3] of point;
    move_num, kol : int;
    max_prof : ext;
    game_fin, fb : boolean;
    strans : string;
    start : ext;

procedure init();
begin
    fb := false;
  go[1].x := 1;
  go[1].y := 0;
  go[2].x := 1;
  go[2].y := 1;
  go[3].x := 0;
  go[3].y := 1;
  go[4].x := -1;
  go[4].y := 1;
  go[5].x := -1;
  go[5].y := 0;
  go[6].x := -1;
  go[6].y := -1;
  go[7].x := 0;
  go[7].y := -1;
  go[8].x := 1;
  go[8].y := -1;
end;

procedure wr();
var
    i, j : int;
begin
    for i := 0 to n do begin
        for j := 0 to n do begin
            if (a[i][j] = 0) then
                write('.');
            if (a[i][j] = 1) then
                write('o');
            if (a[i][j] = 2) then
                write('x');
            if (a[i][j] = 3) then
                write('%');
            if (a[i][j] = 4) then
                write('#');
        end;
        writeln;
    end;
end;

procedure read_move();
var
    s : string;
    i, len, x, y : int;
begin
    readln(s);
    len := length(s);
    if (len <> 6) then
        game_fin := true;
    for i := 1 to (len div 2) do begin
        x := ord(s[2 * i - 1]) - ord('0');
        y := ord(s[2 * i]) - ord('0');
        if (a[x][y] = 0) then
            a[x][y] := 2;
        if (a[x][y] = 1) then begin
            a[x][y] := 4;
            fb := true;
        end;
        a2[x][y] := a[x][y];
    end;
end;

procedure add_ans(x, y : int);
begin
    strans := strans + chr(x + ord('0')) + chr(y + ord('0'));
    if (a[x][y] = 0) then
        a[x][y] := 1;
    if (a[x][y] = 2) then begin
        a[x][y] := 3;
        fb := true;
    end;
    a2[x][y] := a[x][y];
end;



procedure dfs(x, y, x1, x2 : int; var b : matr);
var
    i : int;
    cur : point;
begin
    b[x][y] := 1;
    for i := 1 to 8 do begin
        cur.x := x + go[i].x;
        cur.y := y + go[i].y;
        if (cur.x >= 0) and (cur.x <= n) and (cur.y >= 0) and (cur.y <= n) and ((a[cur.x][cur.y] = x1) or (a[cur.x][cur.y] = x2)) and (b[cur.x][cur.y] = 0) then
            dfs(cur.x, cur.y, x1, x2, b);
    end;
end;

procedure push(x, y : int; var r : int);
begin
    r := r + 1;
    qeue[r].x := x;
    qeue[r].y := y;
end;

function pop(var l : int) : point;
begin
    l := l + 1;
    result := qeue[l];
end;

procedure bfs(x1, x2 : int; const b : matr; var dist : matr);
var
    i, j, l, r : int;
    cur, next : point;
begin
    l := 0;
    r := 0;
    for i := 0 to n do
        for j := 0 to n do
            if (b[i][j] = 1) then begin
                dist[i][j] := 0;
                push(i, j, r);
            end
            else
                dist[i][j] := mx;
    while (r > l) do begin
        cur := pop(l);
        for i := 1 to 8 do begin
            next.x := cur.x + go[i].x;
            next.y := cur.y + go[i].y;
            if (next.x >= 0) and (next.x <= n) and (next.y >= 0) and (next.y <= n) and (dist[next.x][next.y] = mx) then begin
                dist[next.x][next.y] := dist[cur.x][cur.y] + 1;
                if (a[next.x][next.y] = x1) or (a[next.x][next.y] = x2) then
                    push(next.x, next.y, r);
            end;
        end;
    end;
end;

function colon(p : point) : ext;
var
    ds : int;
    ans : ext;
begin
    ds := dist2[p.x][p.y];
    if (ds = 1) then
        ans := -15;
    if (ds = 2) then
        ans := -3;
    {if (ds = 3) and (fb) then
        result := 4;
    if (ds = 3) and (not fb) then
        result := 3; }
    if (ds >= 3) then
        ans := 4;
    if (not fb) and ((p.x = 0) or (p.x = 9) or (p.y = 0) or (p.y = 9)) then
        ans := ans + 2;
    if ((p.x = 9) and (p.y = 0)) or ((p.x = 0) and (p.y = 9)) then
        ans := ans + 0.5;
    result := ans;
end;

function eat(p : point) : ext;
var
    ds: int;
begin
    ds := dist1[p.x][p.y];
    //if (ds > 3) then
    //    ds := 3;
    result := (xx / ds) * f;
end;

function count(k : int) : ext;
var
    i, j, fr1, fr2, fren1, fren2 : int;
    ans : ext;
begin
    ans := 0;
    for i := 0 to n do
        for j := 0 to n do
            a[i][j] := a2[i][j];

    fillchar(b1, sizeof(b1), 0);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 1) and (b1[i][j] = 0) then
                dfs(i, j, 1, 3, b1);
    fr1 := 0;
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 3) and (b1[i][j] = 1) then
                fr1 := fr1 + 1;

    fillchar(b1, sizeof(b1), 0);
    fren1 := 0;
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 2) and (b1[i][j] = 0) then
                dfs(i, j, 2, 4, b1);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 4) and (b1[i][j] = 1) then
                fren1 := fren1 + 1;

    for i := 1 to 3 do begin
        if (a[cur_mv[i].x][cur_mv[i].y] = 0) then
            a[cur_mv[i].x][cur_mv[i].y] := 1;
        if (a[cur_mv[i].x][cur_mv[i].y] = 2) then
            a[cur_mv[i].x][cur_mv[i].y] := 3;
    end;

    fillchar(b1, sizeof(b1), 0);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 1) then
                b1[i][j] := 1;
    bfs(0, 0, b1, dist1);

    fillchar(b2, sizeof(b2), 0);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 2) and (b2[i][j] = 0) then
                dfs(i, j, 2, 4, b2);
    bfs(0, 1, b2, dist2);
    for i := 0 to n do
        for j := 0 to n do
            a[i][j] := a2[i][j];

    for i := 1 to 3 do begin
        if (a[cur_mv[i].x][cur_mv[i].y] = 0) then
            ans := ans + colon(cur_mv[i]);
        if (a[cur_mv[i].x][cur_mv[i].y] = 2) then
            ans := ans + eat(cur_mv[i]);
    end;


    for i := 1 to 3 do begin
        if (a[cur_mv[i].x][cur_mv[i].y] = 0) then
            a[cur_mv[i].x][cur_mv[i].y] := 1;
        if (a[cur_mv[i].x][cur_mv[i].y] = 2) then
            a[cur_mv[i].x][cur_mv[i].y] := 3;
    end;

    fillchar(b1, sizeof(b1), 0);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 1) and (b1[i][j] = 0) then
                dfs(i, j, 1, 3, b1);
    fr2 := 0;
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 3) and (b1[i][j] = 1) then
                fr2 := fr2 + 1;

    fillchar(b1, sizeof(b1), 0);
    fren2 := 0;
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 2) and (b1[i][j] = 0) then
                dfs(i, j, 2, 4, b1);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 4) and (b1[i][j] = 1) then
                fren2 := fren2 + 1;

    if (fren2 < fren1) then
        ans := ans + (fren1 - fren2) * 7;

    if (fr2 > fr1) then
        ans := ans + (fr2 - fr1) * f;

    result := ans;
end;

procedure rec(k, yy : int);
var
    i, j : int;
    mv : ar;
    kl : int;
    cur_prof : ext;
begin
    if (now - start) * 24 * 3600 > con then
        exit;
    if (k > yy) then begin
        cur_prof := count(3);
        if (cur_prof > max_prof) then begin
            kol := 1;
            for i := 1 to 3 do
                move[kol][i] := cur_mv[i];
            max_prof := cur_prof;
        end
        else
            if (cur_prof = max_prof) then begin
                kol := kol + 1;
                for i := 1 to 3 do
                    move[kol][i] := cur_mv[i];
            end;
        exit;
    end;
    fillchar(b, sizeof(b), 0);
    fillchar(b1, sizeof(b1), 0);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 1) then
                b[i][j] := 1;
    bfs(0, 0, b, dist);
    kl := 0;
    for i := 0 to n do
        for j := 0 to n do
            if ((a[i][j] = 0) or (a[i][j] = 2)) and (dist[i][j] = 1) then begin
                kl := kl + 1;
                mv[kl].x := i;
                mv[kl].y := j;
                b1[i][j] := 1;
            end;
    fillchar(b, sizeof(b), 0);
    for i := 0 to n do
        for j := 0 to n do
            if (a[i][j] = 1) and (b[i][j] = 0) then
                dfs(i, j, 1, 3, b);
    bfs(0, 0, b, dist);
    for i := 0 to n do
        for j := 0 to n do
            if ((a[i][j] = 2)) and (dist[i][j] = 1) and (b1[i][j] = 0) then begin
                kl := kl + 1;
                mv[kl].x := i;
                mv[kl].y := j;
            end;

    for i := 1 to kl do begin
        cur_mv[k] := mv[i];
        if (k = 2) and (cur_mv[1].x = 2) and (cur_mv[1].y = 1)  then
            k := k;
        if (a[mv[i].x][mv[i].y] = 0) then
            a[mv[i].x][mv[i].y] := 1;
        if (a[mv[i].x][mv[i].y] = 2) then
            a[mv[i].x][mv[i].y] := 3;
        rec(k + 1, yy);
        if (now - start) * 24 * 3600 > con then
            exit;
        a[mv[i].x][mv[i].y] := a2[mv[i].x][mv[i].y];
    end;
end;

procedure main();
var
    i, num : int;
begin
    start := now;
    strans := '';
    if (a[0][0] = 0) then begin
        add_ans(0, 0);
        add_ans(0, 1);
        add_ans(1, 0);
        writeln(strans);
        flush(output);
        exit;
    end;
    if (a[9][9] = 0) then begin
        add_ans(9, 9);
        add_ans(9, 8);
        add_ans(8, 9);
        writeln(strans);
        flush(output);
        exit;
    end;
    kol := 0;
    max_prof := -100500;
    rec(1, 3);
    if (kol = 0) then begin
        rec(1, 2);
        num := 1;
        if (kol = 0) then begin
            rec(1, 1);
            if (kol = 0) then begin
                halt(0);
            end;
            add_ans(move[num][1].x, move[num][1].y);
            writeln(strans);
            flush(output);
            halt(0);
        end;
        for i := 1 to 2 do
            add_ans(move[num][i].x, move[num][i].y);
        writeln(strans);
        flush(output);
        halt(0);
    end;
    num := random(kol) + 1;
    for i := 1 to 3 do
        add_ans(move[num][i].x, move[num][i].y);
    writeln(strans);
    flush(output);
end;

begin
    randomize;

    game_fin := false;
    init();
    readln(move_num);
    if (move_num = 2) then
        read_move();
    while (true) do begin
        main();
        if (game_fin) then
            break;
        read_move();
    end;

end.
