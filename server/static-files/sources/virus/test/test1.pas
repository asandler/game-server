var
        a: array [-2..11, -2..11] of integer;
        s, answer: string;
        i, j, k, turn: integer;

procedure init;
var i, j: integer;
begin
        for i := -2 to 11 do
                for j := -2 to 11 do
                        if (i < 0) or (i > 9) or (j < 0) or (j > 9) then
                                a[i, j] := -3
                        else
                                a[i, j] := 0;
end;

function bfs(t_i, t_j:integer; kill:boolean):boolean;
var
        visited: array [-2..11, -2..11] of boolean;
        queue: array [1..100] of integer;
        start, finish, i, j, i1, j1: integer;
begin
        for i := -2 to 11 do
                for j := -2 to 11 do
                        visited[i, j] := false;
        visited[t_i, t_j] := true;
        queue[1] := t_i * 10 + t_j;
        start := 0;
        finish := 1;
        while (start < finish) do begin
                inc(start);
                i := queue[start] div 10;
                j := queue[start] mod 10;
                for i1 := i - 1 to i + 1 do
                        for j1 := j - 1 to j + 1 do
                                if (not visited[i1, j1]) and ((a[i1, j1] = turn) or ((a[i1, j1] = -(3 - turn)) and kill)) then begin
                                        if (a[i1, j1] = turn) then begin
                                                bfs := true;
                                                exit;
                                        end;
                                        inc(finish);
                                        queue[finish] := i1 * 10 + j1;
                                        visited[i1, j1] := true;
                                end;
        end;
        bfs := false;
end;

function valid(i, j: integer):boolean;
begin
        if (a[i, j] = 0) then begin
                valid := bfs(i, j, false)
        end else begin
                if (a[i, j] = 3 - turn) then
                        valid := bfs(i, j, true)
                else
                        valid := false;
        end;
end;

function random_move():string;
var i, j, k: integer;
begin
        k := 0;
        while (k < 1000) and (length(answer) < 6) do begin
                i := random(10);
                j := random(10);
                if (valid(i, j)) then begin
                        if (a[i, j] = 3 - turn) then
                                a[i, j] := -a[i, j]
                        else
                                a[i, j] := turn;
                        answer := answer + chr(i + 48) + chr(j + 48);
                end;
                inc(k);
        end;
        random_move := answer;
        answer := '';
end;

begin
        init();
        readln(turn);
        if (turn = 1) then begin
                answer := '00';
                a[0, 0] := 1;
                writeln(random_move());
                flush(output);
        end else begin
                answer := '99';
                a[9, 9] := 2;
        end;
        while (true) do begin
                readln(s);
                for k := 0 to length(s) div 2 - 1 do begin
                        i := ord(s[k * 2 + 1]) - 48;
                        j := ord(s[k * 2 + 2]) - 48;
                        if (a[i, j] = turn) then
                                a[i, j] := -turn
                        else
                                a[i, j] := 3 - turn;
                end;
                writeln(random_move());
                flush(output);
        end;
end.
