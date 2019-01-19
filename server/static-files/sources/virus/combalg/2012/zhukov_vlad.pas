program strateg;

uses
  SysUtils;

const
  MaxN = 100;
  Cold = -1000;
  enemyAttack = 25;

var
  map, temp: array [0..MaxN, 0..MaxN] of Longint;
  canStep, connected: array[0..MaxN, 0..MaxN] of Boolean;
  turn: longint;
  s: String;
  gameOver: Boolean;

  function min(a, b: LongInt): LongInt;
  begin
    if a < b then
      min := a
    else
      min := b;
  end;

  function max(a, b: LongInt): LongInt;
  begin
    max := a + b - min(a, b);
  end;

  procedure printMap();
  var
    i, j: longint;
  begin
    Writeln;
    for i := 0 to 9 do
      write(i, ' ');
    Writeln;
    Writeln;
    for i := 0 to 9 do
    begin
      for j := 0 to 9 do
        write(map[i, j], ' ');
      write(' ', i);
      Writeln;
    end;
  end;

  procedure printTemp();
  var
    i, j: LongInt;
  begin
    Writeln;
    for i := 0 to 9 do
    begin
      for j := 0 to 9 do
        write(temp[i, j], ' ');
      writeln;
    end;
  end;

  procedure fillConnectCell(i, j: LongInt);
  var
    t, k: LongInt;
  begin
    connected[i, j] := true;
    for t := max(0, i - 1) to min(9, i + 1) do
      for k := max(0, j - 1) to min(9, j + 1) do
      begin
        if (t = i) and (k = j) then
          continue;
        if not(connected[t, k]) and (map[t, k] - 2 = 3 - turn) then
          fillConnectCell(t, k);
      end;
  end;

  procedure fillStep(s: string; playerId: Longint);
  var
    i1, j1, i2, j2, i3, j3: LongInt;
  begin
    i1 := ord(s[1]) - 48;
    j1 := ord(s[2]) - 48;
    i2 := ord(s[3]) - 48;
    j2 := ord(s[4]) - 48;
    i3 := ord(s[5]) - 48;
    j3 := ord(s[6]) - 48;
    if (map[i1, j1] = 0) then
        map[i1, j1] := playerId
      else
        if (map[i1, j1] = 3 - playerId) then
          map[i1, j1] := 2 + 3 - playerId;

    if (map[i2, j2] = 0) then
        map[i2, j2] := playerId
      else
        if (map[i2, j2] = 3 - playerId) then
          map[i2, j2] := 2 + 3 - playerId;

    if (map[i3, j3] = 0) then
        map[i3, j3] := playerId
      else
        if (map[i3, j3] = 3 - playerId) then
          map[i3, j3] := 2 + 3 - playerId;
  end;

  procedure searchY(x: LongInt; var y1, y2: LongInt; id: LongInt);
  var
    i, nowY1, nowY2, maxLen: LongInt;
    f: Boolean;
  begin
    f := false;
    nowY1 := -1;
    nowY2 := -1;
    y1 := 0;
    y2 := 0;
    maxLen := -1;
    for i := 0 to 9 do
      if map[i, x] = id then
        if f then
        begin
          nowY2 := i
        end
        else
        begin
          maxLen := Abs(nowY1 - nowY2);
          nowY1 := i;
          nowY2 := i;
          f := true;
        end
        else
        begin
          f := false;
          if Abs(nowY1 - nowY2) > maxLen then
          begin
            y1 := nowY1;
            y2 := nowY2;
            maxLen := Abs(nowY1 - nowY2);
          end;
        end;
    if (Abs(nowY1 - nowY2) > maxLen) and (nowY1 <> -1) and (nowY2 <> -1) then
    begin
      y1 := nowY1;
      y2 := nowY2;
    end;
  end;

  procedure searchX(y: LongInt; var x1, x2: LongInt; id: LongInt);
  var
    i, nowX1, nowX2, maxLen: LongInt;
    f: Boolean;
  begin
    f := false;
    nowX1 := -1;
    nowX2 := -1;
    x1 := -1;
    x2 := -1;
    maxLen := -1;
    for i := 0 to 9 do
      if map[y, i] = id then
        if f then
        begin
          nowX2 := i;
        end
        else
        begin
            nowX1 := i;
            nowX2 := i;
            maxLen := abs(nowX1 - nowX2);
            f := True;
        end
        else
        begin
          f := False;
          if Abs(nowX1 - nowX2) >  maxLen then
          begin
            x1 := nowX1;
            x2 := nowX2;
            maxLen := Abs(nowX1 - nowX2);
          end;
        end;
      if (Abs(nowX1 - nowX2) >  maxLen) and (nowX1 <> -1) and (nowX2 <> -1) then
      begin
        x1 := nowX1;
        x2 := nowX2;
      end;
  end;

  function is_in(a, b, c: LongInt): Boolean;
  begin
    if (a >= b) and (a <= c) then
      is_in := True
    else
      is_in := False;
  end;

  function is_intersect(a, b, c, d: LongInt): Boolean;
  begin
    if (is_in(b, c, d)) or (is_in(a, c, d)) then
      is_intersect := True
    else
      is_intersect := False;
  end;

  function cellCanStep(i, j: longint; player: longint): Boolean;
  var  
    t, k: longint;
  begin
    for t := max(i - 1, 0) to min(i + 1, 9) do
      for k := max(j - 1, 0) to min(j + 1, 9) do
      begin
        if (t = i) and (k = j) then
          Continue;
        if (map[t, k] = 0) or (map[t, k] = 3 - player) or (map[t, k] = 2 + 3 - player) then
        begin
          cellCanStep := True;
          exit;
        end;
      end;
      cellCanStep := False;
  end;

  procedure fillTemp();
  var
    i, j, dist, t, k, x1Our, x2Our, n, m: longInt;
    x1Enemy, x2Enemy: LongInt;
    flag: Boolean;
  begin
    FillChar(connected, SizeOf(connected), 0);
    FillChar(canStep, SizeOf(canStep), 0);
    FillChar(temp, SizeOf(temp), 0);
    for i := 0 to 9 do
      for j := 0 to 9 do
        if (map[i, j] = turn) then
          for t := max(0, i - 1) to min(9, i + 1) do
            for k := max(0, j - 1) to min(9, j + 1) do
            begin
              if (t = i) and (k = j) then
                continue;
              if (map[t, k] = 2 + 3 - turn) then
                if not(connected[t, k]) then
                  fillConnectCell(t, k);
            end;
    //fill warm border
    for i := 0 to 9 do
    begin
      temp[0, i] := 8;
      temp[9, i] := 8;
      temp[i, 0] := 8;
      temp[i, 9] := 8;
    end;
    for i := 0 to 8 do
    begin
      temp[1, i] := 4;  //it's not a bug it's feach
      temp[8, i] := 4;
      temp[i, 1] := 4;
      temp[i, 8] := 4;
    end;
    for i := 2 to 7 do
    begin
      temp[2, i] := 2;
      temp[7, i] := 2;
      temp[i, 2] := 2;
      temp[i, 7] := 2;
    end;
    {temp[0, 0] := 50;
    temp[0, 9] := 50;
    temp[9, 9] := 50;
    temp[9, 0] := 50;}
    temp[1, 0] := 4;//bazinga!
    //temp[1, 1] := 0;//double bazinga!

    //calculate temperature

    //front's attack
    for i := 0 to 9 do
    begin
      //enemy is down
      searchX(i, x1Our, x2Our, turn);
      if (i + 3 <= 9) then
        searchX(i + 3, x1Enemy, x2Enemy, 3 - turn)
      else
      begin
        x1Our := -1;
        x1Enemy := -1;
      end;
      //Writeln(x1Our, ' ', x2Our);
      //Writeln(x1Enemy, ' ', x2Enemy);
      if is_in(x2Our, x1Enemy, x2Enemy) {and (x1Our <= x1Enemy)} then
      begin
        temp[i + 1, x2Our] := temp[i + 1, x2Our] + enemyAttack;
        temp[i + 1, x2Enemy] := temp[i + 1, x2Enemy] + 2 * enemyAttack;
        //Writeln('[', x2Our, ', ', i + 1, ']');
      end
      else
        if is_in(x1Our, x1Enemy, x2Enemy) {and (x2Our >= x2Enemy))} then
        begin
          temp[i + 1, x1Enemy] := temp[i + 1, x1Enemy] + 2 * enemyAttack;
          temp[i + 1, x1Our] := temp[i + 1, x1Our] + enemyAttack;
          //Writeln('[', x1Our, ', ', i + 1, ']');
        end
        else
        begin
          temp[i + 1, x2Enemy] := temp[i + 1, x2Enemy] + 2 * enemyAttack;
          temp[i + 1, x1Enemy] := temp[i + 1, x1Enemy] + 2 * enemyAttack;
        end;
      //enemy is up
      searchX(i, x1Our, x2Our, 3 - turn);
      if (i - 3 >= 0) then
         searchX(i - 3 , x1Enemy, x2Enemy, turn)
      else
      begin
        x1Our := -1;
        x2Our := -1;
      end;
      if ((x1Our <> -1) or (x1Enemy <> -1)) and is_intersect(x1Our, x2Our, x1Enemy, x2Enemy) then
      begin
        if (is_in(x2Our, x1Enemy, x2Enemy) and (x1Our < x1Enemy)) then
        begin
          //Writeln('[', x2Our, ', ', i - 1, ']');
          temp[i - 1, x2Our] := temp[i - 1, x2Our] + enemyAttack;
          temp[i - 1, x2Enemy] := temp[i - 1, x2Enemy] + enemyAttack;
        end
        else
          if (is_in(x1Our, x1Enemy, x2Enemy) and (x2Our > x2Enemy)) then
          begin
            //Writeln('[', x1Our, ', ', i - 1, ']');
            temp[i - 1, x1Our] := temp[i - 1, x1Our] + enemyAttack;
            temp[i - 1, x1Enemy] := temp[i - 1, x1Enemy] + enemyAttack;
          end
          else
          begin
            temp[i - 1, x2Enemy] := temp[i - 1, x2Enemy] + enemyAttack;
            temp[i - 1, x1Enemy] := temp[i - 1, x1Enemy] + enemyAttack;
          end;
      end;  
    end;

    //printTemp;

    for i := 0 to 9 do
    begin
      for j := 0 to 9 do
      begin
        //dead cell
        if map[i, j] - 2 = turn then
          for t := max(0, i - 1) to min(9, i + 1) do
            for k := max(0, j - 1) to min(9, j + 1) do
            begin
              if (t = i) and (k = j) then
                Continue;
              if map[t, k] = 0 then
                temp[t, k] := temp[t, k] - 1;
              if map[t, k] = 3 - turn then
                temp[t, k] := temp[t, k] + 1;
            end;
        if map[i, j] - 2 = 3 - turn then
          for t := max(0, i - 1) to min(9, i + 1) do
            for k := max(0, j - 1) to min(9, j + 1) do
            begin
              if (t = i) and (k = j) then
                continue;
              if map[t, k] = 0 then
                temp[t, k] := temp[t, k] + 1;
              if map[t, k] = 3 - turn then
              begin
                temp[t, k] := temp[t, k] + 17;
              end;
            end;
        //lives cell
        //enemy cell
        {if map[i, j] = turn then
          for t := max(i - 3, 0) to min(i + 3, 9) do
            for k := max(j - 3, 0) to min(j + 3, 9) do
            begin
              if dist = 2 then
                temp[t, k] := temp[t, k] + 35;
            end;}
        if map[i, j] = 3 - turn then
          for t := max(i - 3, 0) to min(i + 3, 9) do
            for k := max(j - 3, 0) to min(j + 3, 9) do
            begin
              dist := max(Abs(i - t), Abs(j - k));
              if dist = 0 then
                temp[t, k] := temp[t, k] + enemyAttack;
              if dist = 1 then
                temp[t, k] := temp[t, k] + enemyAttack div 3;
              if dist = 2 then
                temp[t, k] := temp[t, k] - 2;
              if dist = 3 then
                temp[t, k] := temp[t, k] - 4;
            end;
        end;
    end;
    for i := 0 to 9 do
      for j := 0 to 9 do
        if (map[i, j] = turn) or (map[i, j] = 2 + 3 - turn) then//or dead cell :)
          for t := max(i - 2, 0) to min(i + 2, 9) do
            for k := max(j - 2, 0) to min(j + 2, 9) do
            begin
              //if i can step near live cell
              dist := max(Abs(i - t), Abs(j - k));
              if (map[t, k] <> turn) and (map[t, k] <> 3) and (map[t, k] <> 4) and (map[i, j] = turn) then
              begin
                if (map[t, k] = 3 - turn) and (dist = 1) then   
                  temp[t, k] := -cold + 10 - min(Abs(0 - t) , min(Abs(9 - t) , min(Abs(0 - k), Abs(9 - k))));
                if (dist = 1) then
                  canStep[t, k] := true;
                if (dist = 2) and (map[t, k] = 0) then //[t, k] is free and near is enemy
                begin
                  flag := false;
                  for n := max(t - 1, 0) to min(t + 1, 9) do
                    for m := max(k - 1, 0) to min(k + 1, 9) do
                      if (map[n, m] = 2 + 3 - turn) and (connected[n, m]) then
                        flag := True;
                  if not(flag) then
                    for n := max(t - 1, 0) to min(t + 1, 9) do
                      for m := max(k - 1, 0) to min(k + 1, 9) do
                        if map[n, m] = 3 - turn then
                          temp[t, k] := -cold div 2 + + 10 - min(Abs(0 - t) , min(Abs(9 - t) , min(Abs(0 - k), Abs(9 - k))));
                end;
              end;
              //if [i][j] is connected dead enemy and [t][k] is enemy
              if (dist = 1) and (map[i, j] = 2 + 3 - turn) and (connected[i, j]) and (map[t, k] = 3 - turn) then
              begin
                canStep[t, k] := True;
                Continue;
              end;
            end;
    //it is magick!!!!!!!!!!!!! :-)
    for i := 0 to 9 do
      for j := 0 to 9 do
      begin
        //if (i = 0) and (j = 0) then
        //  Writeln(not(cellcanStep(i, j, 3 - turn)));
        if map[i, j] = 3 - turn then
          if not(cellCanStep(i, j, 3 - turn)) then
            temp[i, j] := temp[i, j] - 3 * enemyAttack;
      end;

    for i := 0 to 9 do
      for j := 0 to 9 do
        if not(canStep[i, j]) then
          temp[i, j] := cold;
  end;

  function doStep({var lastVal}): String;
  var
    max, idi, idj, i, j, enemyCount: LongInt;
  begin
    max := cold - 1;
    idi := 0;
    idj := 0;
    enemyCount := 0;
    for i := 0 to 9 do
      for j := 0 to 9 do
        if map[i, j] = 3 - turn then
          Inc(enemyCount);
    for i := 0 to 9 do
      for j := 0 to 9 do
      begin
        if temp[i, j] > max then
        begin
          max := temp[i, j];
          idi := i;
          idj := j;
        end;
      end;
    if max = cold then
      gameOver := true;
    if map[idi, idj] = 0 then
      map[idi, idj] := turn
      else
        if map[idi, idj] = 3 - turn then
          map[idi, idj] := 2 + 3 - turn;
    if enemyCount = 0 then
      gameOver := true;
     if not(gameOver) then
       doStep := IntToStr(idi) + IntToStr(idj)
     else
       doStep := '';
  end;

begin
  readln(turn);
  if (turn = 1) then
  begin
    fillStep('000102', 1);
    Writeln('000102');
    Flush(output);
  end
  else
  begin
    readln(s);
    fillStep(s, 1);
    fillStep('998979', 2);
    Writeln('998979');
    Flush(output);
  end;

  while not(gameOver) do
  begin
    Readln(s);
    fillStep(s, 3 - turn);
    fillTemp();
    //printTemp();
    s := doStep;
    fillTemp();
    //printTemp();
    s := s + doStep;
    fillTemp();
    //printTemp();
    s := s + doStep;
    fillTemp();
    //printTemp();
    if not(gameOver) then
      Writeln(s);
    Flush(output);
  end;
  
  readln;
end.

