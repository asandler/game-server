{ Тестовый рандом-игрок в "Морской бой".
  Областной открытый турнир по программированию 2010, старшая группа }
program SeaBattle_RandomPlayer;

const	
	RND_STEP  = 15;
	SIZE	  = 10;
	SHIPS_NUM = 10;
	SHOTS_NUM = 10;
	{ Состояние клеток поля }
	EMPTY   = 0;
	DECK    = 1; { Палуба корабля }
	DAMAGE  = 2; { Повреждённая палуба }
	FOUL    = 3; { Клетки окружающие корабль }
	MINE    = 4; { Мина }
	{ Состояние корабля }
	NOMOVE   = 'X';
	UNDEF    = 0; { Для вражеского корабля }
	FIGHTING = 1; { В боевой готовности }
	WOUNDED  = 2;  { "Ранен" }
	CRUSHED  = 4;  { "Убит" }
	
type
	TMoveStr   = string[7];
	TPairCoord = string[4];
	TShip	   = record
			     coord : TPairCoord;
			     state : Integer;
		     end;	   
	TBoard	   = array [Pred('0')..Succ('9'), Pred('A')..Succ('J')] of Byte;
	TPlayer	   = record	   
			     ships : array [0..SHIPS_NUM-1] of TShip;
			     { Игровое поле. Добавлены барьерные клетки }
			     board : TBoard;
			     last_move : TMoveStr;
			     shots_num : Integer;
		     end;	   
	TGameState = record
			     mv_num : Integer;
			     player : array [0..1] of TPlayer;
		     end;	    

{ !!! }
procedure showBoard (const gms : TGameState);
const
	CellChar : string	= ' DX.* ';
var
	row, col : Char;
begin
	for row := Succ('9') downto Pred('0') do begin
		if row in ['0'..'9'] then
			Write (stderr, row, ' ')
		else
			Write (stderr, '  ');
		for col := Pred('A') to Succ('J') do
			Write (stderr, CellChar[gms.player[0].board[row, col] + 1]: 2);
		Write (stderr, '     ');
		for col := Pred('A') to Succ('J') do
			Write (stderr, CellChar[gms.player[1].board[row, col] + 1]: 2);
		if row in ['0'..'9'] then
			Writeln (stderr, '  ', row)
		else
			Writeln(stderr, '');
	end;
	Writeln (stderr, '     A B C D E F G H I J          A B C D E F G H I J')
end; { showBoard }

procedure showOneBoard (const brd : TBoard);
const
	CellChar : string	= ' DX.* ';
var
	row, col : Char;
begin
	for row := Succ('9') downto Pred('0') do begin
		if row in ['0'..'9'] then
			Write (row, ' ')
		else
			Write ('  ');
		for col := Pred('A') to Succ('J') do
			Write (CellChar[brd[row, col] + 1]: 2);
		Writeln
	end;
	Writeln ('     A B C D E F G H I J')
end; { showOneBoard }

{ !!! }
procedure initGame (var gms : TGameState);
	function NormalizeCoord (const s : TPairCoord) : TPairCoord;
	{ Возвращает две пары координат в упорядоченном по возрастанию виде }
	begin
		if (s[1] > s[3]) or (s[2] > s[4]) then
			NormalizeCoord := Copy (s, 3, 2) + Copy (s, 1, 2)
		else
			NormalizeCoord := s;
	end;
var
	s	 : String;
	i	 : Byte;
	row, col : Char;
begin
	Randomize;
	with gms.player[0] do begin
		{ Проинициализировать поле, барьерные клетки занять минами }
		FillChar (board, sizeOf (board), MINE);
		FillChar (gms.player[1].board, sizeOf (board), MINE);
		for row := '0' to '9' do
			for col := 'A' to 'J' do begin
				gms.player[1].board[row,col] := EMPTY;
				board[row, col] := EMPTY;
			end;
		shots_num := 0;
		(* Проинициализировать координаты кораблей и соответствующие клетки на поле,
		 *  включая соседние. Координаты считаются корректными
		 *)
		Readln (s);
		for i := 0 to SHIPS_NUM - 1 do begin
			ships[i].state := FIGHTING;
			ships[i].coord := NormalizeCoord (Copy (s, i * 5 + 1, 4));
			{ Сначала все клетки в прямоугольнике окружающем корабль запрещаются для движения }
			for row := Pred (ships[i].coord[2]) to Succ (ships[i].coord[4]) do
				for col := Pred (ships[i].coord[1]) to Succ (ships[i].coord[3]) do
					if board[row, col] = EMPTY then	{ Барьерные клетки и мины изменять нельзя }
						board[row, col] := FOUL;
			{ Затем отмечаем клетки занятые непосредственно кораблём }
			for row := ships[i].coord[2] to ships[i].coord[4] do
				for col := ships[i].coord[1] to ships[i].coord[3] do
					board[row, col] := DECK;
		end;
	end;
	gms.player[1].last_move := 'K'
end; { initGame }

{  }
(*  Ход программы. По причине сложных правил выполнения хода -- сначала необходимо выполнить перемещение, потом просчитать
 *  результат выстрела соперника, т.е. фактически завершить выполнение хода соперника, и только затем выполнить
 *  свой ход -- MakeProgramMove не только выполняет но и вначале сама находит очередной ход программы.
 *  Таким образом makeProgramMove помимо своих задач  выполняет дополнительно задачу GetProgramMove и частично makeOppMove)
 *)
function makeProgramMove (var gms : TGameState) : TMoveStr;

	procedure changeCoord (var c : TPaircoord; dir : Char);
	begin
		case dir of
		  'N' : begin Inc(c[2]); Inc (c[4]); end;
		  'E' : begin Inc(c[1]); Inc (c[3]); end;
		  'S' : begin Dec(c[2]); Dec (c[4]); end;
		  'W' : begin Dec(c[1]); Dec (c[3]); end;
		end;
	end; { changeCoord }

	function getShipNum (row, col : Char; const p : TPlayer): Integer;
	{ Находит корабль, которому принадлежит клетка с координатами row, col }
	var
		i : Integer;
	begin
		with p do begin
			i := 0;
			while (i < SHIPS_NUM) and ((row < ships[i].coord[2]) or (row > ships[i].coord[4])
			       or (col < ships[i].coord[1]) or (col > ships[i].coord[3])) do
				       Inc (i)
		end;
		Assert (i < SIZE, 'Не найден корабль содержащий клетку ' + col + row);
		getShipNum := i;
	end; { getShipNum }

	function posCorrect (const brd : TBoard; t : TPairCoord) : Boolean;
	var
		row, col, r, c : Char;
	begin
		posCorrect := FALSE;
		for row := t[2] to t[4] do
			for col := t[1] to t[3] do begin
				{ Клетка, в которую перемещается корабль не должна содержать мину  }
				if brd[row, col] = MINE  then
					Exit;
				{ Ни одна из клеток соседних с новой позицией не должна быть палубой корабля }
				for r := Pred(row) to Succ(row) do
					for c := Pred(col) to Succ(col) do
						if (brd[r, c] = DECK) or (brd[r, c] = DAMAGE) then
							Exit;
			end;
		posCorrect := TRUE;
	end; { posCorrect }

	function getMoveDirect (brd : TBoard; coord : TPairCoord) : Char;
	(*  Находит вариант направления перемещения для корабля с координатами coord,
	 *  на поле brd. Если корабль нельзя переместить ни в одном направлении -- возвращает 'X'
	 *)
	const
		DIR : string[4]	= 'NESW';
	var
		res	 : string;
		t	 : TPairCoord;
		i	 : Integer;
		row, col : Char;
	begin 
		res := '';
		{ Снять корабль из текущей позиции }
		for row := coord[2] to coord[4] do
			for col := coord[1] to coord[3] do
				brd[row, col] := EMPTY;
{ 		showOneBoard (brd); }
		for i := 1 to 4 do begin
			t := coord;
			changeCoord (t, DIR[i]);
			if posCorrect (brd, t) then
				res := res + DIR[i];
		end;
		if res = '' then
			getMoveDirect := 'X'
		else
			getMoveDirect := res[Random(Length(res)) + 1]
	end; { getMoveDirect }

	function getShipState (const brd : TBoard; const coord : TPairCoord) : Integer;
	var
		row, col : Char;
		cnt	 : Integer;
	begin 
		cnt := 0;
		for row := coord[2] to coord[4] do
			for col := coord[1] to coord[3] do
				if brd[row, col] = DECK then
					Inc (cnt);
		if cnt = 0 then
			getShipState := CRUSHED
		else
			getShipState := WOUNDED;
	end; { getShipState }

	function getCellState (const brd : TBoard; row, col : Char) : Integer;
	var
		r, c : Char;
	begin
		if (brd[row, col] = MINE) or (brd[row, col] = DECK) or (brd[row, col] = DAMAGE) then begin
			getCellState := brd[row, col];
			Exit
		end;
			
		for r := Pred (row) to Succ (row) do
			for c := Pred (col) to Succ (col) do
				if (brd[r, c] = DECK) or (brd[r, c] = DAMAGE) then begin
					getCellState := FOUL;
					Exit;
				end;
		getCellState := EMPTY
	end;

	procedure moveShip (var player : TPlayer; k : Integer; dir : Char);
	var
		row, col : Char;
	begin
		{ Убрать корабль из старой позиции, установив клетки в EMPTY }
		for row := player.ships[k].coord[2] to player.ships[k].coord[4] do
			for col := player.ships[k].coord[1] to player.ships[k].coord[3] do
				player.board[row, col] := EMPTY;
		{ Пересчитать состояние клеток вокруг старой позиции корабля }
		for row := Pred(player.ships[k].coord[2]) to Succ(player.ships[k].coord[4]) do
			for col := Pred(player.ships[k].coord[1]) to Succ(player.ships[k].coord[3]) do
				player.board[row, col] := getCellState (player.board, row, col);
		{ Изменить позицию корабля }
		changeCoord (player.ships[k].coord, dir);
		
		{ Установить клетки в новой позиции корабля в DECK }
		for row := player.ships[k].coord[2] to player.ships[k].coord[4] do
			for col := player.ships[k].coord[1] to player.ships[k].coord[3] do
				player.board[row, col] := DECK;
		{ Пересчитать состояние клеток вокруг новой позиции корабля }
		for row := Pred(player.ships[k].coord[2]) to Succ(player.ships[k].coord[4]) do
			for col := Pred(player.ships[k].coord[1]) to Succ(player.ships[k].coord[3]) do
				player.board[row, col] := getCellState (player.board, row, col);
	end; { moveShip }

var
	row, col, dir : Char;
	mv	      : TMoveStr;
	k	      : Integer;
	stock	      : string;
begin
	(*  1. Выбрать корабль и направление для перемещения, запомнить перемещение как первую часть хода
	 *     и выполнить перемещение корабля
	 *     1.1 В первую очередь выбор корабля для перемещения зависит от выстрела соперника.
	 *         Если его выстрел попадает в один из кораблей, нужно попытаться его переместить --
	 *         выбираем именно его, при условии что он не повреждён.
	 *     1.2 Если не было попадания, или мы не можем переместить корабль, в который было попадание,
	 *         или последний ход соперника был не выстрел -- выбираем для перемещения любой неповреждённый корабль
	 *         (может быть тот, который делал последний выстрел).
	 *     1.3 Если на поле не осталось ни одного неповреждённого корабля -- выводим '0X'
	 *     1.4 Выполнить непосредственно перемещение выбранного корабля.
	 *  2. Завершить ход-выстрел соперника
	 *     2.1 Если было попадание:
	 *         2.1.1 Отметить клетку на своём поле как поврежденную палубу.
	 *         2.1.2 Найти в списке соответствующий корабль, определить статус повреждения ("убит" или "ранен").
	 *         2.1.3 Сформировать соответствующий ход 'K' или 'H', запомнить его в last_move и выйти из функции
	 *     2.2 Если попадания не было
	 *         2.2.1 Отметить на поле клетку как MINE (за исключением случая повторного попадания
	 *                в повреждённую  палубу DAMAGE)
	 *  3. Придумать свой выстрел
	 *     3.1 Выбрать корабль для выстрела и запомнить координаты начальной клетки выстрела
	 *         3.1.1 Если есть частично повреждённый корабль, можно выбрать его
	 *         3.1.2 Иначе можно выбрать произвольно (TOTHINCK: лучше стрелять из однопалубного или многопалубного?)
	 *     3.2 Выбрать целевую клетку на поле соперника
	 *         3.2.1 В простейшем случае -- любая пустая клетка.
	 *         3.2.2 Может быть клетка из которой соперник выполнял свой выстрел или клетка рядом с ней
	 *  4. Сохранить сформированный ход как last_move.
	 *)
	{ 1. }
	dir := NOMOVE;
	{ 1.1. }
	if Length (gms.player[1].last_move) = 4 then begin
		row := gms.player[1].last_move[4];
		col := gms.player[1].last_move[3];
		if gms.player[0].board[row, col] = DECK then begin
			k := getShipNum (row, col, gms.player[0]);
			{ TOTHINCK: Перемещение корабля, который находится под угрозой попадания не всегда удачное }
			if gms.player[0].ships[k].state = FIGHTING then
				dir := getMoveDirect (gms.player[0].board, gms.player[0].ships[k].coord);
		end;
	end;
	{ 1.2. }
	if dir <> NOMOVE then
		mv := Chr (k + 48) + dir + ' '
	else begin
		stock := '';
		for k := 0 to 9 do
			if gms.player[0].ships[k].state = FIGHTING then begin
				dir := getMoveDirect (gms.player[0].board, gms.player[0].ships[k].coord);
				{ В stock накапливаем пары символов: номер корабля + допустимое напр. перемещения }
				if dir <> 'X' then
					stock := stock + Chr (k + 48) + dir;
			end;
		if stock <> '' then
			(*  Есть корабли, которые могут переместиться -- берем любые два символа
			 *  из stock начиная с нечетной позиции
			 *)
			mv := Copy (stock, Random(Length (stock) div 2)*2 + 1, 2) + ' '
		else
			{ 1.3. Не нашли ни одного корабля, который мог выполнить перемещение }
		mv := '0X ';

	end;
	{ 1.4 Выполнить перемещение }
	if mv[2] <> 'X' then
		moveShip (gms.player[0], Ord (mv[1]) - 48, mv[2]);

	{ 2. }
	if Length(gms.player[1].last_move) = 4 then begin
		row := gms.player[1].last_move[4];
		col := gms.player[1].last_move[3];
		{2.1. }
		if gms.player[0].board[row, col] = DECK then begin
			{ 2.1.1. }
			gms.player[0].board[row, col] := DAMAGE;
			{ 2.1.2. }
			k := getShipNum (row, col, gms.player[0]); 
			gms.player[0].ships[k].state := getShipState (gms.player[0].board, gms.player[0].ships[k].coord);
			{ 2.1.3 }
			if gms.player[0].ships[k].state = CRUSHED then
				mv := mv + 'K'
			else
				mv := mv + 'H';
			gms.player[0].last_move := mv;
			makeProgramMove := mv;
			Exit;
		end
		{ 2.2. }
		else if gms.player[0].board[row, col] <> DAMAGE then
			gms.player[0].board[row, col] := MINE;
	end;
	{ 3.1. }
	{ 3.1.1. }
	k := 0;
	while (k < SHIPS_NUM) and (gms.player[0].ships[k].state <> WOUNDED) do
		Inc (k);
	if k = SHIPS_NUM then begin
		{ 3.1.2 }
		repeat
			k := Random (10);
		until gms.player[0].ships[k].state = FIGHTING;
	end;
	{ Найти палубу (первую попавшуюся) }
	row := gms.player[0].ships[k].coord[2];
	col := gms.player[0].ships[k].coord[1];
	if col = gms.player[0].ships[k].coord[3] then begin
		while gms.player[0].board[row, col] <> DECK do
			Inc (row);
	end
	else
		while gms.player[0].board[row, col] <> DECK do
			Inc (col);
	{ Добавляем полученные начальные координаты выстрела к строке хода }
	mv := mv + col + row;

	{ 3.2. }
	repeat
		row := Chr (Random (10) + Ord ('0'));
		col := Chr (Random (10) + Ord ('A'));
	until gms.player[1].board[row, col] = EMPTY;
	{ Добавляем полученные координаты целевой клетки выстрела к строке хода }
	mv := mv + col + row;

	{ 4. }
	gms.player[0].last_move := mv;
	makeProgramMove := mv;
	
end; { makeProgramMove }

{$IFDEF BADPLAYER}
procedure getBadProgramMove (var gms : TGameState; var mv : TMoveStr);
type
	ErrorCodeSet =  (RF_PLAY_MOVE_OVERFLOW,
			 RF_PLAY_MOVE_TIMELIMIT,
			 RF_PLAY_MOVE_SLEEP,
			 RF_PLAY_MOVE_LOST);
const
	DIR : string = 'NESWX';
var
	k : Integer;
	i : Longint;
begin
	k := Random (17);
	{ Нарушение техн. условий }
	case ErrorCodeSet (k) of
	  RF_PLAY_MOVE_OVERFLOW	: mv := 'My best move!'; { Неверный формат хода. Другие варианты ниже }
	  RF_PLAY_MOVE_TIMELIMIT : 
		  for i := 1 to 1000000000 do			  {}
			  k := 100;
	  RF_PLAY_MOVE_SLEEP	 : Readln (k, i, k);
	  RF_PLAY_MOVE_LOST	 : Halt;
	end; { case }

	{ Моделировать ошибку передвижения корабля }
	case k of
	  4  : mv[1] := Chr (Random (10) + 48); { произвольный корабль в допустимых пределах }
	  5  : mv[1] := Chr (Random (90) + 32); { вместо номера корабля -- произвольный символ из первой части таблицы }
	  6  : mv[2] := DIR[Random (5) + 1]; { произвольное направление передвижения }
	  7 : mv[2] := Chr (Random (128)); { произвольный символ в качестве направления перемещения }
	  8 : mv[3] := '@';		       { вместо пробела }
	end;
	{ Моделируем ошибку хода }
	if Length (mv) = 4 then
		case k of
		  9  : if mv[4] = 'K' then      { Поменять 'K' на 'H', и наоборот }
			      mv[4] := 'H'
		      else
			      mv[4] := 'K';
		  10  : mv[4] := Chr (Random (256)); { Случайный символ в качестве ответа }
		  11  : mv := Copy (mv, 1, 3);	   { Отсутствует последний символ }
		  12  : mv := mv + '1F5';	   { Добавим координаты выстрела }
		end
	{ Моделировать ошибку хода-выстрела }
	else
		case k of
		  9 : mv[4] := Chr (Random (10) + Ord('A')); { Произвольные символы координат в пределах доски }
		  10 : mv[5] := Chr (Random (10) + Ord('0'));
		  11 : mv[6] := Chr (Random (10) + Ord('A'));
		  12 : mv[7] := Chr (Random (10) + Ord('0'));
		  13 : mv[4] := Chr (Random (128));	 { Любые символы в качестве координат выстрела }
		  14 : mv[5] := Chr (Random (128));
		  15 : mv[6] := Chr (Random (128));
		  16 : mv[7] := Chr (Random (128));
		end; { case }
end;
{$ENDIF}

{ !!! }
{ Выполняет ход соперника. Согласно техн. условиям -- строка представляет корректный ход }
procedure makeOppMove (var gms : TGameState; mv : TMoveStr);

var				   
	row, col, r, c : Char;
	t	 : TPairCoord;
begin 
	(* Ход соперника может быть длиной 1 или 4.
	 *  1. Если ход 1, то
	 *	1.1. Отметить на поле соперника повреждённую палубу
	 *	1.2. Если ход 'H', то отметить диагональные клетки как FOUL
	 *      1.3. Если ход 'K' -- отметить все соседние (кроме мин) как FOUL

	 *  2. Если ход 4, то
	 *      2.1 Проверить наш предыдущий ход и, если необходимо, отметить промах - поставить мину
	 *  3. Сохранить ход в last_move
	 *)
{ 	Writeln ('My last_move: ', gms.player[0].last_move); }
	row := gms.player[0].last_move[7];
	col := gms.player[0].last_move[6];
	if (Length (mv) = 1) then begin
		{ 1.2. }
		if mv[1] = 'H' then begin
			with gms.player[1] do begin
				board[row, col] := DAMAGE;
				if board[Succ(row), Succ(col)] = EMPTY then
					board[Succ(row), Succ(col)] := FOUL;
				if board[Pred(row), Succ(col)] = EMPTY then
					board[Pred(row), Succ(col)] := FOUL;
				if board[Succ(row), Pred(col)] = EMPTY then
					board[Succ(row), Pred(col)] := FOUL;
				if board[Pred(row), Pred(col)] = EMPTY then
					board[Pred(row), Pred(col)] := FOUL;
			end
		end
		{ 1.3. ход 'K' }
		else begin
			t := col + row + col + row;
			with gms.player[1] do begin
				while board[row, Pred(t[1])] = DAMAGE do
					Dec (t[1]);
				while board[row, Succ(t[3])] = DAMAGE do
					Inc (t[3]);
				while board[Pred(t[2]), t[1]] = DAMAGE do
					Dec (t[2]);
				while board[Succ(t[4]), t[3]] = DAMAGE do
					Inc (t[4]);
				for row := t[2] to t[4] do
					for col := t[1] to t[3] do
						for r := Pred (row) to Succ (row) do
							for c := Pred (col) to Succ (col) do
								if board[r, c] = EMPTY then
									board[r, c] := FOUL;
			end;
		end;
	end;
	(* 2. Если ход соперника длиной 4, то
	 *  если предыдущий наш ход был выстрел, значит мы промахнулись, надо поставить мину
	 *  и сохранить ход в last_move (для того, чтобы его выполнить полностью,
	 *  нужно сначала выполнить перемещение своего корабля)
	 *)
	if Length (gms.player[0].last_move) = 7 then begin
		with gms.player[1] do
			if board[row, col] <> DAMAGE then
				board[row, col] := MINE;
	end;
	gms.player[1].last_move := mv;
end; { makeOppMove }

{ !!! }
function gameOver (gms : TGameState) : Boolean;
var
	cr0, cr1, i : Integer;
begin
	cr0 := 0;
	cr1 := 1;
	for i := 0 to SHIPS_NUM do begin
		if gms.player[0].ships[i].state = CRUSHED then
			Inc (cr0);
		if gms.player[1].ships[i].state = CRUSHED then
			Inc (cr1);
	end;
	gameOver := (cr0 = SHIPS_NUM) or (cr1 = SHIPS_NUM)
		or (gms.player[0].shots_num = SHOTS_NUM)
		or (gms.player[1].shots_num = SHOTS_NUM);
end; { gameOver }

{ !!! }
var
	gms : TGameState;
	mv  : TMoveStr;
	plr : Integer;
begin
	initGame (gms);
	Readln (plr);

	if plr = 1 then begin
		Readln (mv);
		makeOppMove (gms, mv);
	end;

	{showBoard (gms);}

 	while not gameOver (gms) do begin
		mv := makeProgramMove (gms);
		{$IFDEF BADPLAYER  Один раз из RAND_STEP ходов пытаемся исказить ход}
		if Random (RND_STEP) = 0 then
			getBadProgramMove (gms, mv);
		{$ENDIF}
		Writeln (mv);
                Flush(output);

		{showBoard (gms);}

		if gameOver (gms) then
			break;
		Readln (mv);
		makeOppMove (gms, mv);

		{showBoard (gms);}
	end; 
end.
