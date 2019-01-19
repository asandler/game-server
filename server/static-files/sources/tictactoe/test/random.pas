var     a: array [0..8] of boolean;
        turn, m, n: integer;

function random_move():integer;
var i: integer;
begin
        i := random(9);
        while (a[i]) do
                i := random(9);
        a[i] := true;
        random_move := i;
end;

begin
        randomize;
        read(turn);
        if (turn = 1) then begin
                writeln(random_move());
                flush(output);
                n := 1;
        end;
        while (n < 9) do begin
                read(m);
                a[m] := true;
                inc(n);
                if (n < 9) then begin
                        writeln(random_move());
                        flush(output);
                        inc(n);
                end;
        end;
end.
