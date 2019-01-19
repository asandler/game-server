var f2,kolb,kolw,f,g,t,k,i,j,i1,j1,t1,f1,t2,r,q,max,maxi:longint;
z:string;
a,b: ARRAY [-5..15,-5..15] of char;
c:array [1..60,1..2] of integer;
v: array [1..60] of integer;
{переверунуть фишки}

procedure perev(x,y:integer);
   begin
      if (a[x,y]='B') then begin
                                    if a[x-1,y]='W' then begin
                                                         i1:=x-1;
                                                         j1:=y;
                                                         while (a[i1,j1]='W') and (i1>=1) do i1:=i1-1;
                                                         if a[i1,j1]='B' then begin i1:=i1+1; while (a[i1,j1]='W') do
                                                                                              begin
                                                                                              a[i1,j1]:='B';
                                                                                              i1:=i1+1;
                                                                                              end;
                                                                               end;

                                                         end;

                                    if a[x+1,y]='W' then begin
                                                         i1:=x+1;
                                                         j1:=y;
                                                         while (a[i1,j1]='W') and (i1<=8) do i1:=i1+1;
                                                         if a[i1,j1]='B' then begin i1:=i1-1; while (a[i1,j1]='W') do
                                                                                              begin
                                                                                              a[i1,j1]:='B';
                                                                                              i1:=i1-1;
                                                                                              end;
                                                                               end;

                                                         end;


                                    if a[x,y-1]='W' then begin
                                                         i1:=x;
                                                         j1:=y-1;
                                                         while (a[i1,j1]='W') and (j1>=1) do j1:=j1-1;
                                                         if a[i1,j1]='B' then begin j1:=j1+1; while (a[i1,j1]='W') do
                                                                                              begin
                                                                                              a[i1,j1]:='B';
                                                                                              j1:=j1+1;
                                                                                              end;
                                                                               end;

                                                         end;
                                    if a[x,y+1]='W' then begin
                                                         i1:=x;
                                                         j1:=y+1;
                                                         while (a[i1,j1]='W') and (j1<=8) do j1:=j1+1;
                                                         if a[i1,j1]='B' then begin j1:=j1-1; while (a[i1,j1]='W') do
                                                                                              begin
                                                                                              a[i1,j1]:='B';
                                                                                              j1:=j1-1;
                                                                                              end;
                                                                               end;

                                                         end;



                                    if a[x+1,y+1]='W' then begin
                                                         i1:=x+1;
                                                         j1:=y+1;
                                                         while (a[i1,j1]='W') and (j1<=8) and (i1<=8) do begin j1:=j1+1; i1:=i1+1; end;
                                                         if a[i1,j1]='B' then begin j1:=j1-1; i1:=i1-1; while (a[i1,j1]='W') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='B';
                                                                                                        j1:=j1-1;
                                                                                                        i1:=i1-1;
                                                                                                        end;
                                                                               end;

                                                         end;

                                    if a[x-1,y-1]='W' then begin
                                                         i1:=x-1;
                                                         j1:=y-1;
                                                         while (a[i1,j1]='W') and (j1>=1) and (i1>=1) do begin j1:=j1-1; i1:=i1-1; end;
                                                         if a[i1,j1]='B' then begin j1:=j1+1; i1:=i1+1; while (a[i1,j1]='W') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='B';
                                                                                                        j1:=j1+1;
                                                                                                        i1:=i1+1;
                                                                                                        end;
                                                                               end;

                                                         end;


                                    if a[x+1,y-1]='W' then begin
                                                         i1:=x+1;
                                                         j1:=y-1;
                                                         while (a[i1,j1]='W') and (j1>=1) and (i1<=8) do begin j1:=j1-1; i1:=i1+1; end;
                                                         if a[i1,j1]='B' then begin j1:=j1+1; i1:=i1-1; while (a[i1,j1]='W') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='B';
                                                                                                        j1:=j1+1;
                                                                                                        i1:=i1-1;
                                                                                                        end;
                                                                               end;

                                                         end;

                                    if a[x-1,y+1]='W' then begin
                                                         i1:=x-1;
                                                         j1:=y+1;
                                                         while (a[i1,j1]='W') and (j1<=8) and (i1>=1) do begin j1:=j1+1; i1:=i1-1; end;
                                                         if a[i1,j1]='B' then begin j1:=j1-1; i1:=i1+1; while (a[i1,j1]='W') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='B';
                                                                                                        j1:=j1-1;
                                                                                                        i1:=i1+1;
                                                                                                        end;
                                                                               end;

                                                         end;

                                    end;


            if (a[x,y]='W') then begin
                                    if a[x-1,y]='B' then begin
                                                         i1:=x-1;
                                                         j1:=y;
                                                         while (a[i1,j1]='B') and (i1>=1) do i1:=i1-1;
                                                         if a[i1,j1]='W' then begin i1:=i1+1; while (a[i1,j1]='B') do
                                                                                              begin
                                                                                              a[i1,j1]:='W';
                                                                                              i1:=i1+1;
                                                                                              end;
                                                                               end;

                                                         end;


                                    if a[x+1,y]='B' then begin
                                                         i1:=x+1;
                                                         j1:=y;
                                                         while (a[i1,j1]='B') and (i1<=8) do i1:=i1+1;
                                                         if a[i1,j1]='W' then begin i1:=i1-1; while (a[i1,j1]='B') do
                                                                                              begin
                                                                                              a[i1,j1]:='W';
                                                                                              i1:=i1-1;
                                                                                              end;
                                                                               end;

                                                         end;


                                    if a[x,y+1]='B' then begin
                                                         i1:=x;
                                                         j1:=y+1;
                                                         while (a[i1,j1]='B') and (j1<=8) do j1:=j1+1;
                                                         if a[i1,j1]='W' then begin j1:=j1-1; while (a[i1,j1]='B') do
                                                                                              begin
                                                                                              a[i1,j1]:='W';
                                                                                              j1:=j1-1;
                                                                                              end;
                                                                               end;

                                                         end;


                                    if a[x,y-1]='B' then begin
                                                         i1:=x;
                                                         j1:=y-1;
                                                         while (a[i1,j1]='B') and (j1>=1) do j1:=j1-1;
                                                         if a[i1,j1]='W' then begin j1:=j1+1; while (a[i1,j1]='B') do
                                                                                              begin
                                                                                              a[i1,j1]:='W';
                                                                                              j1:=j1+1;
                                                                                              end;
                                                                               end;
                                                         end;



                                    if a[x+1,y+1]='B' then begin
                                                         i1:=x+1;
                                                         j1:=y+1;
                                                         while (a[i1,j1]='B') and (j1<=8) and (i1<=8) do begin j1:=j1+1; i1:=i1+1; end;
                                                         if a[i1,j1]='W' then begin j1:=j1-1; i1:=i1-1; while (a[i1,j1]='B') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='W';
                                                                                                        j1:=j1-1;
                                                                                                        i1:=i1-1;
                                                                                                        end;
                                                                               end;

                                                         end;


                                    if a[x-1,y-1]='B' then begin
                                                         i1:=x-1;
                                                         j1:=y-1;
                                                         while (a[i1,j1]='B') and (j1>=1) and (i1>=1) do begin j1:=j1-1; i1:=i1-1; end;
                                                         if a[i1,j1]='W' then begin j1:=j1+1; i1:=i1+1; while (a[i1,j1]='B') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='W';
                                                                                                        j1:=j1+1;
                                                                                                        i1:=i1+1;
                                                                                                        end;
                                                                               end;

                                                         end;


                                    if a[x+1,y-1]='B' then begin
                                                         i1:=x+1;
                                                         j1:=y-1;
                                                         while (a[i1,j1]='B') and (j1>=1) and (i1<=8) do begin j1:=j1-1; i1:=i1+1; end;
                                                         if a[i1,j1]='W' then begin j1:=j1+1; i1:=i1-1; while (a[i1,j1]='B') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='W';
                                                                                                        j1:=j1+1;
                                                                                                        i1:=i1-1;
                                                                                                        end;
                                                                               end;

                                                         end;

                                    if a[x-1,y+1]='B' then begin
                                                         i1:=x-1;
                                                         j1:=y+1;
                                                         while (a[i1,j1]='B') and (j1<=8) and (i1>=1) do begin j1:=j1+1; i1:=i1-1; end;
                                                         if a[i1,j1]='W' then begin j1:=j1-1; i1:=i1+1; while (a[i1,j1]='B') do
                                                                                                        begin
                                                                                                        a[i1,j1]:='W';
                                                                                                        j1:=j1-1;
                                                                                                        i1:=i1+1;
                                                                                                        end;
                                                                               end;

                                                         end;

                                    end;

   end;

{нахождение хода для черных фишек}
procedure findb;
begin
t:=1;
     for j:=1 to 8 do
         for i:=1 to 8 do
         begin
          if a[i,j]='*' then if (a[i-1,j]='W') then begin
                                                    i1:=i-1;
                                                    j1:=j;
                                                    while (i1>=1) and (a[i1,j1]='W') do
                                                    i1:=i1-1;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                    end;
          if a[i,j]='*' then if (a[i+1,j]='W') then begin
                                                    i1:=i+1;
                                                    j1:=j;
                                                    while (i1<=8) and (a[i1,j1]='W') do
                                                    i1:=i1+1;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                    end;
         if a[i,j]='*' then if (a[i,j+1]='W') then begin
                                                    i1:=i;
                                                    j1:=j+1;
                                                    while (j1<=8) and (a[i1,j1]='W') do
                                                    j1:=j1+1;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                    end;

         if a[i,j]='*' then if (a[i,j-1]='W') then begin
                                                    i1:=i;
                                                    j1:=j-1;
                                                    while (j1>=1) and (a[i1,j1]='W') do
                                                    j1:=j1-1;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i+1,j+1]='W') then begin
                                                    i1:=i+1;
                                                    j1:=j+1;
                                                    while (j1<=8) and (i1<=8) and (a[i1,j1]='W') do
                                                    begin
                                                    j1:=j1+1;
                                                    i1:=i1+1;
                                                    end;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i-1,j-1]='W') then begin
                                                    i1:=i-1;
                                                    j1:=j-1;
                                                    while (j1>=8) and (i1>=8) and (a[i1,j1]='W') do
                                                    begin
                                                    j1:=j1-1;
                                                    i1:=i1-1;
                                                    end;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i+1,j-1]='W') then begin
                                                    i1:=i+1;
                                                    j1:=j-1;
                                                    while (j1>=1) and (i1<=8) and (a[i1,j1]='W') do
                                                    begin
                                                    j1:=j1-1;
                                                    i1:=i1+1;
                                                    end;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i-1,j+1]='W') then begin
                                                    i1:=i-1;
                                                    j1:=j+1;
                                                    while (j1<=8) and (i1>=1) and (a[i1,j1]='W') do
                                                    begin
                                                    j1:=j1+1;
                                                    i1:=i1-1;
                                                    end;
                                                    if (a[i1,j1]='B') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;


         end;

end;

{нахождение хода для белых фишек}
procedure findw;
begin
t:=1;
     for j:=1 to 8 do
         for i:=1 to 8 do
         begin
          if a[i,j]='*' then if (a[i-1,j]='B') then begin
                                                    i1:=i-1;
                                                    j1:=j;
                                                    while (i1>=1) and (a[i1,j1]='B') do
                                                    i1:=i1-1;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                    end;
          if a[i,j]='*' then if (a[i+1,j]='B') then begin
                                                    i1:=i+1;
                                                    j1:=j;
                                                    while (i1<=8) and (a[i1,j1]='B') do
                                                    i1:=i1+1;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                    end;
         if a[i,j]='*' then if (a[i,j+1]='B') then begin
                                                    i1:=i;
                                                    j1:=j+1;
                                                    while (j1<=8) and (a[i1,j1]='B') do
                                                    j1:=j1+1;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                    end;

         if a[i,j]='*' then if (a[i,j-1]='B') then begin
                                                    i1:=i;
                                                    j1:=j-1;
                                                    while (j1>=1) and (a[i1,j1]='B') do
                                                    j1:=j1-1;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i+1,j+1]='B') then begin
                                                    i1:=i+1;
                                                    j1:=j+1;
                                                    while (j1<=8) and (i1<=8) and (a[i1,j1]='B') do
                                                    begin
                                                    j1:=j1+1;
                                                    i1:=i1+1;
                                                    end;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i-1,j-1]='B') then begin
                                                    i1:=i-1;
                                                    j1:=j-1;
                                                    while (j1<=8) and (i1<=8) and (a[i1,j1]='B') do
                                                    begin
                                                    j1:=j1-1;
                                                    i1:=i1-1;
                                                    end;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i+1,j-1]='B') then begin
                                                    i1:=i+1;
                                                    j1:=j-1;
                                                    while (j1>=1) and (i1<=8) and (a[i1,j1]='B') do
                                                    begin
                                                    j1:=j1-1;
                                                    i1:=i1+1;
                                                    end;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;

         if a[i,j]='*' then if (a[i-1,j+1]='B') then begin
                                                    i1:=i-1;
                                                    j1:=j+1;
                                                    while (j1<=8) and (i1>=1) and (a[i1,j1]='B') do
                                                    begin
                                                    j1:=j1+1;
                                                    i1:=i1-1;
                                                    end;
                                                    if (a[i1,j1]='W') then begin
                                                                          for k:=1 to t do if (c[k,1]=i) and (c[k,2]=j) then g:=1;
                                                                          if (g<>1) then begin
                                                                                         c[t,1]:=i;
                                                                                         c[t,2]:=j;
                                                                                         t:=t+1;
                                                                                         end;
                                                                          g:=0;
                                                                          end;
                                                     end;


         end;

end;

procedure ob;
begin
   for i:=1 to 60 do
   c[i,1]:=0;
   c[i,2]:=0;
end;

procedure obv;
begin
   for j:=1 to 60 do
   v[j]:=0;
end;

procedure win;
   begin
   kolb:=0;
   kolw:=0;
   for j:=1 to 8 do
   for i:=1 to 8 do
   begin
   if (a[i,j]='B') then kolb:=kolb+1;
   if (a[i,j]='W') then kolw:=kolw+1;
   end;
   end;

procedure copyab;
   begin
   for j:=1 to 8 do
   for i:=1 to 8 do
   b[i,j]:=a[i,j];
   end;

procedure copyba;
   begin
   for j:=1 to 8 do
   for i:=1 to 8 do
   a[i,j]:=b[i,j];
   end;



begin

for j:=1 to 8 do
    for i:=1 to 8 do
    if ((i=4) and (j=4)) or ((i=5) and (j=5)) then a[i,j]:='W' else
    if ((i=4) and (j=5)) or ((i=5) and (j=4)) then a[i,j]:='B' else
    a[i,j]:='*';

Randomize;
readln (f2);
findb;
f:=1;
if (f2=1) then while (c[1,1]<>0) do
   begin

   if (f=1) then begin
                 obv;
                 copyab;
                 ob;

                 findb;
                 f1:=1;
                 t2:=t-1;
                   for r:=1 to t2 do
                       begin
                          for q:=1 to 150 do
                             begin
                             f1:=-1;
                             copyba;
                             findb;
                             a[c[r,1],c[r,2]]:='B';
                             while (c[1,1]<>0) do
                             begin


                             if (f1=1) then begin
                                            ob;
                                            findb;
                                            t1:=random(t-1)+1;
                                            a[c[t1,1],c[t1,2]]:='B';
                                            perev(c[t1,1],c[t1,2]);
                                            end;
                             if (f1=-1) then begin
                                            ob;
                                            findw;
                                            t1:=random(t-1)+1;
                                            a[c[t1,1],c[t1,2]]:='W';
                                            perev(c[t1,1],c[t1,2]);
                                            end;
                             f1:=f1*(-1);
                             if (f1=1) then findb else findw;
                             end;
                             win;
                             if kolb>kolw then v[r]:=v[r]+1;
                             end;
                       copyba;
                       findb;

                       end;
                 max:=0;
                       findb;
                       for q:=1 to t do
                       if v[q]>max then begin max:=v[q]; maxi:=q; end;
                       if (c[1,1]<>0) then a[c[maxi,1],c[maxi,2]]:='B';
                       perev(c[maxi,1],c[maxi,2]);
                       if (c[1,1]=0) then writeln ('Skip') else writeln (chr(96+c[maxi,1]),c[maxi,2]);
                       flush(output);
                 end;
    f:=f*(-1);

    if (f=-1) then begin
                 readln(z);
                 if (z<>'Skip') then a[ord(z[1])-96,ord(z[2])-48]:='W';
                 perev(ord(z[1])-96,ord(z[2])-48);
                 end;
   if (f=1) then findb else findw;

   end;












if (f2=2) then while (c[1,1]<>0) do
   begin

   if (f=1) then begin
                 readln(z);
                 if (z<>'Skip') then a[ord(z[1])-96,ord(z[2])-48]:='B';
                 perev(ord(z[1])-96,ord(z[2])-48);
                 end;
    f:=f*(-1);

    if (f=-1) then begin
                 obv;
                 copyab;
                 ob;
                 findw;
                 f1:=1;
                 t2:=t-1;
                   for r:=1 to t2 do
                       begin
                          for q:=1 to 150 do
                             begin
                             f1:=1;
                             copyba;
                             findw;
                             a[c[r,1],c[r,2]]:='W';
                             while (c[1,1]<>0) do
                             begin


                             if (f1=1) then begin
                                            ob;
                                            findb;
                                            t1:=random(t-1)+1;
                                            a[c[t1,1],c[t1,2]]:='B';
                                            perev(c[t1,1],c[t1,2]);
                                            end;
                             if (f1=-1) then begin
                                            ob;
                                            findw;
                                            t1:=random(t-1)+1;
                                            a[c[t1,1],c[t1,2]]:='W';
                                            perev(c[t1,1],c[t1,2]);
                                            end;
                             f1:=f1*(-1);
                             if (f1=1) then findb else findw;
                             end;
                             win;
                             if kolw>kolb then v[r]:=v[r]+1;
                             end;
                       copyba;
                       findw;

                       end;
                 max:=0;
                       findw;
                       for q:=1 to t do
                       if v[q]>max then begin max:=v[q]; maxi:=q; end;
                       if (c[1,1]<>0) then a[c[maxi,1],c[maxi,2]]:='W';
                       perev(c[maxi,1],c[maxi,2]);
                       if (c[1,1]=0) then writeln ('Skip') else writeln (chr(96+c[maxi,1]),c[maxi,2]);
                       flush(output);
                 end;
   if (f=1) then findb else findw;

   end;


end.
