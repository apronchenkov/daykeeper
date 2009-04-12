%!
TeXDict begin
/!BP{ 72 72.27 div dup scale } def /!psvect { /y exch def /x exch def
/L2 exch def /L1 exch def /ang exch def /dist exch def /nb exch def
newpath 0 0 moveto x y lineto stroke x y translate x 0 eq y 0 eq and
{/x 1 def /y 0 def} if y x atan 180 add rotate 1 1 nb { newpath 0 0
moveto L1 ang cos mul L1 ang sin mul 2 copy lineto L2 0 lineto neg
lineto closepath L2 0 eq { stroke } { fill } ifelse dist 0 translate
} for } def /!psrectpath { /y1 exch def /x1 exch def /y0 exch def /x0
exch def newpath x0 y0 moveto x1 y0 lineto x1 y1 lineto x0 y1 lineto
closepath } def /!pscirclepath { newpath 0 360 arc closepath } def
/!psovalpath { /rmax exch dup 0.1 le { pop 0.1 } if def /H exch 2 div
dup 0.1 le { pop 0.1 } if def /W exch 2 div dup 0.1 le { pop 0.1 }
if def /y exch def /x exch def H W le { /R H def }{ /R W def } ifelse
rmax R le { /R rmax def } if newpath x y H add moveto x W add y H add
x W add y R arct x W add y H sub x y H sub R arct x W sub y H sub x
W sub y R arct x W sub y H add x y H add R arct closepath } def /!ps_cbezier
{ /y3 exch def /x3 exch def /y2 exch def /x2 exch def /y1 exch def
/x1 exch def /y0 exch def /x0 exch def /L2 exch def /L1 exch def /ang
exch def /dist exch def /nb exch def newpath x0 y0 moveto x1 y1 x2
y2 x3 y3 curveto stroke /xa x1 x2 sub 3 mul x3 x0 sub add def /xb x0
x1 sub x1 sub x2 add 3 mul def /xc x1 x0 sub 3 mul def /ya y1 y2 sub
3 mul y3 y0 sub add def /yb y0 y1 sub y1 sub y2 add 3 mul def /yc y1
y0 sub 3 mul def /t4 1 def 1 1 nb { /x4 t4 !calculx def /y4 t4 !calculy
def L2 0 le { /x2 x4 def /y2 y4 def /dx t4 !calculdx def /dy t4 !calculdy
def dx dup mul dy dup mul add sqrt dup 0 eq { /dx 1 def /dy 0 def pop
1 } if L1 exch div dup dx neg mul /xx exch def dy neg mul /yy exch
def } { /t1 0 def /t2 t4 def 0 1 !max { pop /t t1 t2 add 2 div def
t !calculx x4 sub dup mul t !calculy y4 sub dup mul add sqrt L2 ge
{ /t1 t def } { /t2 t def } ifelse } for /x2 t !calculx def /y2 t !calculy
def L1 L2 div dup x2 x4 sub mul /xx exch def y2 y4 sub mul /yy exch
def } ifelse /x1 x4 xx ang cos mul add yy ang sin mul sub def /y1 y4
xx ang sin mul add yy ang cos mul add def /x3 x4 xx ang cos mul add
yy ang sin mul add def /y3 y4 xx ang sin mul sub yy ang cos mul add
def newpath x4 y4 moveto x1 y1 lineto x2 y2 lineto x3 y3 lineto closepath
L2 0 le { stroke } { fill } ifelse /t1 0 def /t2 t4 def 0 1 !max {
pop /t t1 t2 add 2 div def t !calculx x4 sub dup mul t !calculy y4
sub dup mul add sqrt dist ge { /t1 t def } { /t2 t def } ifelse } for
/t4 t def } for } def /!ps_qbezier { /y3 exch def /x3 exch def /yy
exch def /xx exch def /y0 exch def /x0 exch def /x1 xx 2 mul x0 add
3 div def /y1 yy 2 mul y0 add 3 div def /x2 xx 2 mul x3 add 3 div def
/y2 yy 2 mul y3 add 3 div def x0 y0 x1 y1 x2 y2 x3 y3 !ps_cbezier }
def /!max 10 def /!ps_r_cbezier { /y3 exch def /x3 exch def /y2 exch
def /x2 exch def /y1 exch def /x1 exch def /y0 exch def /x0 exch def
/xa x1 x2 sub 3 mul x3 x0 sub add def /xb x0 x1 sub x1 sub x2 add 3
mul def /xc x1 x0 sub 3 mul def /ya y1 y2 sub 3 mul y3 y0 sub add def
/yb y0 y1 sub y1 sub y2 add 3 mul def /yc y1 y0 sub 3 mul def /t1 0
def /t2 1 def 0 1 !max { pop /t t1 t2 add 2 div def t !calculx t !calculy
path!a inufill { /t1 t def } { /t2 t def } ifelse } for /ta t def /t1
0 def /t2 1 def 0 1 !max { pop /t t1 t2 add 2 div def t !calculx t
!calculy path!b inufill { /t2 t def } { /t1 t def } ifelse } for /tb
t def ta !calculx ta !calculy ta !calculdx 3 div tb ta sub mul ta !calculx
add ta !calculdy 3 div tb ta sub mul ta !calculy add tb !calculdx 3
div ta tb sub mul tb !calculx add tb !calculdy 3 div ta tb sub mul
tb !calculy add tb !calculx tb !calculy !ps_cbezier } def /!calculx
{ dup dup xa mul xb add mul xc add mul x0 add } def /!calculy { dup
dup ya mul yb add mul yc add mul y0 add } def /!calculdx { dup 3 xa
mul mul 2 xb mul add mul xc add } def /!calculdy { dup 3 ya mul mul
2 yb mul add mul yc add } def /!sign {dup 0 lt {pop -1} {0 eq {0} {1}
ifelse} ifelse} def /!pslatexline { /L exch def /b exch def /a exch
def a 0 eq {0 L b !sign mul} {L a !sign mul dup b mul a div} ifelse
newpath 0 0 moveto lineto stroke } def /!pslatexvector { /L exch def
/b exch def /a exch def a 0 eq {0 L b !sign mul} {L a !sign mul dup
b mul a div} ifelse !psvect } def /!pscircle { 2 div newpath 0 0 3
2 roll 0 360 arc stroke } def /!psdisk { 2 div newpath 0 0 3 2 roll
0 360 arc fill } def
end