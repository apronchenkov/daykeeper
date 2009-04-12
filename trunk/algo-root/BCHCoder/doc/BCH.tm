<TeXmacs|1.0.6>

<style|article>

<\body>
  <section|���� ��� (����� - �������� - ����������)>

  <\theorem>
    (������� ���)

    ����� <with|mode|math|g(x)> ��������� ��� <with|mode|math|C> � ����� ���
    ������ ����

    <\equation*>
      \<alpha\><rsup|m>, \<alpha\><rsup|m + j>, \<alpha\><rsup|m + 2j>,
      \<ldots\> ,\<alpha\><rsup|m + (d-2)j>,
    </equation*>

    ��� <with|mode|math|\<alpha\><rsup|j>> -- ����������� ������� ����
    ���������� <with|mode|math|g(x)>. ����� ����������� ���������� ����
    <with|mode|math|C> �� ����� d.
  </theorem>

  <\proof>
    (�� ����������) ����������� <with|mode|math|d(C) \<less\> d>, �� ���� �
    <with|mode|math|C> ���� ������� ����� ���������� �������� ������ d.
    ������� ��� � ���� ����������

    <\equation*>
      f(x) = \<beta\><rsub|1>x<rsup|i<rsub|1>>+\<beta\><rsub|2>x<rsup|i<rsub|2>>
      + \<ldots\>+\<beta\><rsub|d-1>x<rsup|i<rsub|d-1>>,
    </equation*>

    ���, ��������, ��������� <with|mode|math|\<beta\><rsub|i>> �������, ��
    ��� <with|mode|math|f(x)> ������� �� ����.

    �� ����������� ����������� ����� <with|mode|math|g(x)\|f(x)>, ������ ���
    ����� <with|mode|math|g(x)> �������� ������ <with|mode|math|f(x)>.

    <\equation*>
      <left|{><tabular|<tformat|<cwith|3|3|1|1|cell-halign|c>|<table|<row|<cell|\<beta\><rsub|1>\<alpha\><rsup|m*i<rsub|1>>
      + \<beta\><rsub|2>\<alpha\><rsup|m*i<rsub|2>>+\<ldots\>+\<beta\><rsub|d-1>\<alpha\><rsup|m*i<rsub|d-1>>>|<cell|=>|<cell|0>>|<row|<cell|\<beta\><rsub|1>\<alpha\><rsup|(m*+j)i<rsub|1>>
      + \<beta\><rsub|2>\<alpha\><rsup|(m+j)i<rsub|2>>+\<ldots\>+\<beta\><rsub|d-1>\<alpha\><rsup|(m+j)i<rsub|d-1>>>|<cell|=>|<cell|0>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<beta\><rsub|1>\<alpha\><rsup|(m*+(d-2)j)i<rsub|1>>
      + \<beta\><rsub|2>\<alpha\><rsup|(m+(d-2)j)i<rsub|2>>+\<ldots\>+\<beta\><rsub|d-1>\<alpha\><rsup|(m+(d-2)j)i<rsub|d-1>>>|<cell|=>|<cell|0>>>>>
    </equation*>

    ��� ������� �� <with|mode|math|d-1> ��������� � <with|mode|math|d-1>
    ����������� <with|mode|math|\<beta\><rsub|1>, \<beta\><rsub|2 >,
    \<ldots\>, \<beta\><rsub|d-1>>. ������������ �������:

    <\equation*>
      \<Delta\>=\<alpha\><rsup|m*i<rsub|1>>\<alpha\><rsup|m*i<rsub|2>>*\<ldots\>*\<alpha\><rsup|m*i<rsub|d-1>><det|<tformat|<table|<row|<cell|1>|<cell|1>|<cell|\<ldots\>>|<cell|1>>|<row|<cell|\<alpha\><rsup|j*i<rsub|1>>>|<cell|\<alpha\><rsup|j*i<rsub|2>>>|<cell|\<ldots\>>|<cell|\<alpha\><rsup|j*i<rsub|d-1>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|\<alpha\><rsup|(d-2)j*i<rsub|1>>>|<cell|\<alpha\><rsup|(d-2)j*i<rsub|2>>>|<cell|\<ldots\>>|<cell|\<alpha\><rsup|(d-2)j*i<rsub|d-1>>>>>>><neg|
      = >0,
    </equation*>

    -- ������� �� ����, ������������� ������� ������������ ������� �������,
    <with|mode|math|\<beta\><rsub|i>=0>, <with|mode|math|\<forall\>i>.
    ������������ � �������� <with|mode|math|f(x) <neg|=>0.>
  </proof>

  <\definition>
    ����� ���� <with|mode|math|d, m<rsub|0>\<in\>\<bbb-Z\>> �
    <with|mode|math|\<alpha\>\<in\>GF(2<rsup|m>)>, <with|mode|math|\<alpha\>>
    ����������� ������. ��� ��� -- ��� � ����������� �����������
    <with|mode|math|g(x)\<in\>GF(2)[x]>, ���������� ������� ����� ������
    �������� ���� <with|mode|math|\<alpha\><rsup|m<rsub|0>>,
    \<alpha\><rsup|m<rsub|0>+1>, \<ldots\>, \<alpha\><rsup|m<rsub|0> + d-2>>.
    ���� <with|mode|math|m<rsub|0>=1>, ����� ��� ����������� ���-���.
  </definition>

  <\remark>
    <with|mode|math|g(x)\|x<rsup|2<rsup|m>-1>-1>.

    ��� �����, ��� ��� ������ ������� <with|mode|math|GF(2<rsup|m>)> ��������
    ������ <with|mode|math|x<rsup|2<rsup|m>>-x>, �
    <with|mode|math|GF(2<rsup|m>)> �������� ���� ����������
    <with|mode|math|g(x)>.
  </remark>

  <\remark>
    ������ ���-���� <with|mode|math|n=2<rsup|m>-1>.

    ��� ������� �� �������, ��� <with|mode|math|g(x) \| x<rsup|n>-1> �
    <with|mode|math|n> -- ����������� � ����� ���������.
  </remark>

  <\theorem>
    ����������� ���-��� �������� ���������
    <with|mode|math|t\<leqslant\><frac|d-1|2>> ������.
  </theorem>

  <\proof>
    �����������, ��� ��� �������� ��������� ���������
    <with|mode|math|\<nu\>\<leqslant\>t> ������:

    <\equation*>
      i(x)g(x) \<longrightarrow\><rsub|+e(x)> f(x)=i(x)g(x) + e(x),\ 
    </equation*>

    ��� <with|mode|math|e(x)> ��������� ������:

    <\equation*>
      e(x)=x<rsup|i<rsub|1>>+x<rsup|i<rsub|2>>+\<ldots\>+x<rsup|i<rsub|\<nu\>>>
      <with|mode|text|, ��� >i<rsub|j>\<less\>2<rsup|m>-1.
    </equation*>

    ������� ���������� <with|mode|math|g(x)> ��������
    <with|mode|math|\<alpha\>, \ \<alpha\><rsup|2>, \<ldots\>,
    \<alpha\><rsup|d-1>>, ����� ������� <with|mode|math|f(\<alpha\><rsup|j>)=i(\<alpha\><rsup|j>)*g(\<alpha\><rsup|j>)
    + e(\<alpha\><rsup|j>)=e(\<alpha\><rsup|j>), \ <with|mode|text|���
    >j\<in\>{1, 2, \<ldots\>, d-1}.> ���������� ��������� �������:

    <\equation*>
      <with|mode|text|<with|mode|math|S<rsub|j>=f(\<alpha\><rsup|j>)=e(\<alpha\><rsup|j>)>>,
    </equation*>

    \ -- ��������.

    ������ ����������� ������ �������� ��������� �������: �������
    <with|mode|math|d-1> ��������� <with|mode|math|S<rsub|j>>, ����������
    ����������� ��������� ������ <with|mode|math|e(x)>.

    ��������� <with|mode|math|e(x)> ������������ �������������
    <with|mode|math|\<nu\>> � <with|mode|math|i<rsub|1>, i<rsub|2>,
    \<ldots\>, i<rsub|\<nu\>>>.

    ����� ������������� ��������������� �������:
    <with|mode|math|X<rsub|1>=\<alpha\><rsup|i<rsub|1>>,
    X<rsub|2>=\<alpha\><rsup|i<rsub|2>>, \<ldots\>,
    X<rsub|\<nu\>>=\<alpha\><rsup|i<rsub|\<nu\>>>> -- �������� ������.

    � ����� ������������:

    <\equation*>
      <tabular|<tformat|<cwith|1|4|3|3|cell-halign|c>|<table|<row|<cell|e(\<alpha\>)>|<cell|=>|<cell|X<rsub|1>+X<rsub|2>+\<ldots\>+X<rsub|\<nu\>>>|<cell|=>|<cell|S<rsub|1>>>|<row|<cell|e(\<alpha\><rsup|2>)>|<cell|=>|<cell|X<rsub|1><rsup|2>+X<rsub|2><rsup|2>+\<ldots\>+X<rsub|\<nu\>><rsup|2>>|<cell|=>|<cell|S<rsub|2>>>|<row|<cell|>|<cell|>|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|e(\<alpha\><rsup|2t>)>|<cell|=>|<cell|X<rsub|1><rsup|2t>+X<rsub|2><rsup|2t>+\<ldots\>+X<rsub|\<nu\>><rsup|2t>>|<cell|=>|<cell|S<rsub|2t>>>>>>
    </equation*>

    ��� �������� ��������, ��� ��� <with|mode|math|X<rsup|k>=X<rsup|l>
    \<Leftrightarrow\> \<alpha\><rsup|i<rsub|k>>=\<alpha\><rsup|i<rsub|l>>
    \<Leftrightarrow\> i<rsub|k>\<equiv\>i<rsub|l>(mod 2<rsup|m>-1)>, ��
    <with|mode|math|i<rsub|k>,i<rsub|l>\<less\>2<rsup|m>-1>; � ������� ��
    ����.

    ���������� �������:

    <\equation*>
      <tabular|<tformat|<table|<row|<cell|M=<matrix|<tformat|<table|<row|<cell|S<rsub|1>>|<cell|S<rsub|2>>|<cell|\<ldots\>>|<cell|S<rsub|t>>>|<row|<cell|S<rsub|2>>|<cell|S<rsub|3>>|<cell|\<ldots\>>|<cell|S<rsub|t+1>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|S<rsub|t>>|<cell|S<rsub|t+1>>|<cell|\<ldots\>>|<cell|S<rsub|2t-1>>>>>>=<matrix|<tformat|<table|<row|<cell|1>|<cell|1>|<cell|\<ldots\>>|<cell|1>>|<row|<cell|X<rsub|1>>|<cell|X<rsub|2>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|X<rsub|1><rsup|t-1>>|<cell|X<rsub|2><rsup|t-1>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t-1>>>>>>*<matrix|<tformat|<table|<row|<cell|X<rsub|1>>|<cell|X<rsub|1><rsup|2>>|<cell|\<ldots\>>|<cell|X<rsub|1><rsup|t>>>|<row|<cell|X<rsub|2>>|<cell|X<rsub|2><rsup|2>>|<cell|\<ldots\>>|<cell|X<rsub|2><rsup|t>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|X<rsub|\<nu\>>>|<cell|X<rsub|\<nu\>><rsup|2>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t>>>>>>=>>|<row|<cell|=<matrix|<tformat|<table|<row|<cell|1>|<cell|1>|<cell|\<ldots\>>|<cell|1>>|<row|<cell|X<rsub|1>>|<cell|X<rsub|2>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|X<rsub|1><rsup|t-1>>|<cell|X<rsub|2><rsup|t-1>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t-1>>>>>><matrix|<tformat|<table|<row|<cell|X<rsub|1>>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|X<rsub|2>>|<cell|>|<cell|>>|<row|<cell|>|<cell|>|<cell|\<ldots\>>|<cell|>>|<row|<cell|>|<cell|>|<cell|>|<cell|X<rsub|\<nu\>>>>>>><matrix|<tformat|<table|<row|<cell|1>|<cell|X<rsub|1>>|<cell|\<ldots\>>|<cell|X<rsub|1><rsup|t-1>>>|<row|<cell|1>|<cell|X<rsub|2>>|<cell|\<ldots\>>|<cell|X<rsub|2><rsup|t-1>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|1>|<cell|X<rsub|\<nu\>>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t-1>>>>>>=W*I*W<rsup|T>.>>>>>
    </equation*>

    ���� ������ <with|mode|math|W> � <with|mode|math|I> �����
    <with|mode|math|\<nu\>>, ������������� ���� ������� <with|mode|math|M>
    ��� �� ����� <with|mode|math|\<nu\>>.

    ���������� ���������� ���������:

    <\equation*>
      \<Lambda\>(x) = (1-x*X<rsub|1>)(1-x*X<rsub|2>)\<ldots\>(1-x*X<rsub|\<nu\>>)=\<Lambda\><rsub|\<nu\>>x<rsup|\<nu\>>+\<Lambda\><rsub|\<nu\>-1>x<rsup|\<nu\>-1>+\<ldots\>+\<Lambda\><rsub|1>x+1.
    </equation*>

    ��� ����� ������� ���������. ��������� ������ <with|mode|math|x> �������
    <with|mode|math|X<rsub|i><rsup|-1>>, �������:

    <\equation*>
      \<Lambda\><rsub|\<nu\>>X<rsub|i><rsup|-\<nu\>>+\<Lambda\><rsub|\<nu\>-1>X<rsub|i><rsup|1-\<nu\>>+\<ldots\>+\<Lambda\><rsub|1>X<rsub|i><rsup|-1>+1=0,<with|mode|text|
      ������� �� <with|mode|math|X<rsub|i><rsup|j+\<nu\>>>,>
    </equation*>

    <\equation*>
      \<Lambda\><rsub|\<nu\>>X<rsub|i><rsup|j>+\<Lambda\><rsub|\<nu\>-1>X<rsub|i><rsup|j+1>+\<ldots\>+\<Lambda\><rsub|1>X<rsub|i><rsup|j+\<nu\>-1>+X<rsub|i><rsup|j+\<nu\>>=0,<with|mode|text|
      ������ �� ���� <with|mode|math|i\<in\>{1\<ldots\>\<nu\>}>.>
    </equation*>

    <\equation*>
      \<Lambda\><rsub|\<nu\>>S<rsub|j>+\<Lambda\><rsub|\<nu\>-1>S<rsub|j+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|j+\<nu\>-1>+S<rsub|j+\<nu\>>=0.
    </equation*>

    �������� ������� �������� ��������� � �������������
    <with|mode|math|\<Lambda\><rsub|i>>:

    <\equation*>
      \<star\><left|{><tabular|<tformat|<cwith|1|4|1|1|cell-halign|c>|<table|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|1>+\<Lambda\><rsub|\<nu\>-1>S<rsub|2>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>><rsub|>>|<cell|=>|<cell|-S<rsub|\<nu\>+1>>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|2>+\<Lambda\><rsub|\<nu\>-1>S<rsub|3>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>+1>>|<cell|=>|<cell|-S<rsub|\<nu\>+2>>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|\<nu\>>+\<Lambda\><rsub|\<nu\>-1>S<rsub|\<nu\>+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|2\<nu\>-1>>|<cell|=>|<cell|-S<rsub|2\<nu\>>>>>>>
    </equation*>

    ������� ���� �������:

    <\equation*>
      <matrix|<tformat|<table|<row|<cell|S<rsub|1>>|<cell|S<rsub|2>>|<cell|\<ldots\>>|<cell|S<rsub|\<nu\>>>>|<row|<cell|S<rsub|2>>|<cell|S<rsub|3>>|<cell|\<ldots\>>|<cell|S<rsub|\<nu\>+1>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|S<rsub|\<nu\>>>|<cell|S<rsub|\<nu\>+1>>|<cell|\<ldots\>>|<cell|S<rsub|2\<nu\>-1>>>>>>
    </equation*>

    -- �������� ������������ ������� <with|mode|math|M> � ����� ������
    <with|mode|math|\<nu\>\<times\>\<nu\>>, ������������� �������� �������
    �����������. C������ ������������ � <with|mode|math|\<Lambda\><rsub|1>,
    \<Lambda\><rsub|2>, \<ldots\>, \<Lambda\><rsub|\<nu\>>> ���������.
  </proof>

  <\remark>
    ������� <with|mode|math|\<star\>> � ����� ���� �������� �����:

    <\equation*>
      <left|{><tabular|<tformat|<cwith|1|4|1|1|cell-halign|c>|<table|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|1>+\<Lambda\><rsub|\<nu\>-1>S<rsub|2>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>><rsub|>>|<cell|=>|<cell|-S<rsub|\<nu\>+1>>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|2>+\<Lambda\><rsub|\<nu\>-1>S<rsub|3>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>+1>>|<cell|=>|<cell|-S<rsub|\<nu\>+2>>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|t>+\<Lambda\><rsub|\<nu\>-1>S<rsub|t+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|2t-1>>|<cell|=>|<cell|-S<rsub|2t>>>>>>
    </equation*>

    ��, �������� � ������ ������� �������, ������� ����������� �������
    �������� �������� �����.

    � ����� ������, ������ ������ ��� �� �����.
  </remark>

  <\proposition>
    �������� ����������� ������:

    <\enumerate-numeric>
      <item>(��������������� ����������) ��������� <with|mode|math|S<rsub|1>,
      S<rsub|2>, \<ldots\>, S<rsub|2t>>.

      <item>(�������������) <with|mode|math|\<nu\>=t>.

      <item>(��� �����) ������� <with|mode|math|\<Delta\>=<det|<tformat|<table|<row|<cell|S<rsub|1>>|<cell|\<ldots\>>|<cell|S<rsub|\<nu\>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|S<rsub|\<nu\>>>|<cell|\<ldots\>>|<cell|S<rsub|2\<nu\>-1>>>>>>>.

      <item>(������� �����) ���� <with|mode|math|\<Delta\> =0>, ��
      <with|mode|math|\<nu\>=\<nu\>-1> � ��������� ��� 3.

      <item>(��������� ����������� <with|mode|math|\<Lambda\>(x)>) ������
      �������, ������� <with|mode|math|><with|mode|math|\<Lambda\><rsub|1>,
      \<Lambda\><rsub|2>, \<ldots\>, \<Lambda\><rsub|\<nu\>>.>

      <item>(����� ���������) ��������� ����� <with|mode|math|\<Lambda\>(x)>,
      ������� <with|mode|math|X<rsub|1>, X<rsub|2>, \<ldots\>,
      X<rsub|\<nu\>>>.

      <item>(�������������� <with|mode|math|e(x)>) �������
      <with|mode|math|i<rsub|1>, i<rsub|2>, \<ldots\>, i<rsub|\<nu\>>>.
    </enumerate-numeric>
  </proposition>

  <section|�������� ����������>

  ���������� ������. ���� <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>,
  S<rsub|2t>\<in\>GF(2<rsup|m>)> � �������

  <\equation*>
    <left|{><tabular|<tformat|<cwith|1|4|1|1|cell-halign|c>|<table|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|1>+\<Lambda\><rsub|\<nu\>-1>S<rsub|2>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>><rsub|>>|<cell|=>|<cell|-S<rsub|\<nu\>+1>>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|2>+\<Lambda\><rsub|\<nu\>-1>S<rsub|3>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>+1>>|<cell|=>|<cell|-S<rsub|\<nu\>+2>>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|t>+\<Lambda\><rsub|\<nu\>-1>S<rsub|t+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|2t-1>>|<cell|=>|<cell|-S<rsub|2t>>>>>>,
    \<nu\> \<leqslant\>t.
  </equation*>

  ���������� ����� �������, ��������� ��: ����������� <with|mode|math|\<nu\>>
  � <with|mode|math|\<Lambda\><rsub|1>, \<Lambda\><rsub|2>, \<ldots\>,
  \<Lambda\><rsub|\<nu\>>\<in\>GF(2<rsup|m>)>.

  ����� <with|mode|math|\<Lambda\>(x)> -- ���������� �������
  <with|mode|math|L> �� ��������� ����� <with|mode|math|1>
  (<with|mode|math|\<Lambda\><rsub|0>=1>).

  <\definition>
    <with|mode|math|\<Lambda\>(x)> ��������� <with|mode|math|S<rsub|1>,
    S<rsub|2>, \<ldots\>, S<rsub|r>>, ����
    <with|mode|math|\<forall\>\<rho\>\<in\>{L+1, \<ldots\>, r}> �����������

    <\equation*>
      \<Sigma\><rsub|j=0><rsup|L>\<Lambda\><rsub|j>S<rsub|p-j>=0.
    </equation*>
  </definition>

  ������ ������ ����� �������������� �����: ���������� ����� ���������
  ���������� ������� ����������� ��� <with|mode|math|S<rsub|1>, S<rsub|2>,
  \<ldots\>, S<rsub|2t>>.

  ����� ����������� ������������, �������� <with|mode|math|\<Lambda\>(x)> ���
  ��������.

  <\proposition>
    �������� ����������:

    <\enumerate-numeric>
      <item>(�������������) <with|mode|math|\<Lambda\><rsup|(0)>(x)=1<with|mode|text|
      -- ��������� ������� �� ������>>, <with|mode|math|B<rsup|(0)>(x) = 1,
      L(0) = 0, r=1<with|mode|text| -- ��������������� ��������>>.

      <item>(������ ��������) <with|mode|math|\<Delta\><rsub|r>=\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|r-j>>.

      <item>(<with|mode|math|\<Delta\><rsub|r>=0>) ����
      <with|mode|math|\<Delta\><rsub|r>=0>, ��
      <with|mode|math|\<Lambda\><rsup|(r)>(x)=\<Lambda\><rsup|(r-1)>(x)>,
      <with|mode|math|L(r)=L(r-1)>.

      <item>(<with|mode|math|\<Delta\><rsub|r><neg|=>0>) ����
      <with|mode|math|\<Delta\><rsub|r><neg|=>0>, ��
      <with|mode|math|\<Lambda\><rsup|(r)>(x)=\<Lambda\><rsup|(r-1)>(x)-\<Delta\><rsub|r>x*B<rsup|(r-1)>(x)>,
      <with|mode|math|L(r) = max{L(r-1), \ r - L(r-1)}>.

      <item>(������������� <with|mode|math|B<rsup|(r)>>)

      <\equation*>
        B<rsup|(r)>(x)=<left|{><tabular|<tformat|<table|<row|<cell|B<rsup|(r-1)>(x)x<with|mode|text|,
        ���� <with|mode|math|L(r)=L(r-1)>>>>|<row|<cell|\<Delta\><rsub|r><rsup|-1>\<Lambda\><rsup|(r-1)>(x)<with|mode|text|,
        ���� <with|mode|math|L(r)\<gtr\>L(r-1)>>.>>>>>
      </equation*>

      <item>(���) ���� <with|mode|math|r\<less\>2t>, �� ���������� ����������
      � ���� 2 ��� <with|mode|math|r=r+1>.
    </enumerate-numeric>
  </proposition>

  ������� ������������.

  <\lemma>
    <label|�����1>����� <with|mode|math|\<Lambda\><rsup|(r-1)>(x)> �
    <with|mode|math|\<Lambda\><rsup|(r)>(x)> ���������� ���������� ��������
    ����������� �������� <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>,
    S<rsub|r-1>> � <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>,
    S<rsub|r>> ��������������; <with|mode|math|L(r-1) =
    deg\<Lambda\><rsup|(r-1)>(x)>, <with|mode|math|L(r) =
    deg\<Lambda\><rsup|(r)>(x)> � <with|mode|math|\<Lambda\><rsup|(r-1)>(x)
    <neg|=>\<Lambda\><rsup|(r)>(x)>, �����
    <with|mode|math|L(r)\<geqslant\>max{L(r-1), r-L(r-1)}.>
  </lemma>

  <\proof>
    �� ������������� <with|mode|math|\<Lambda\><rsup|(r-1)>> �������, ���
    <with|mode|math|L<rsup|(r-1)>\<leqslant\>L<rsup|(r)>>, ������� ��������,
    ��� <with|mode|math|r-L(r-1)\<leqslant\>L(r)>. ����������� ��������:

    <\equation*>
      r-L(r-1)\<gtr\>L(r)<with|mode|text| ���, ��� ���� �����,
      >r-L(r)\<gtr\>L(r-1).
    </equation*>

    �� ������� �������, �� �����:

    <\equation*>
      \<forall\>\<rho\>\<in\>{L(r-1)+1, r-1}<with|mode|text| �����
      >\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|\<rho\>-j>=0\<Rightarrow\>S<rsub|\<rho\>>=-\<Sigma\><rsub|j=1><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|\<rho\>-j><with|mode|text|;>
    </equation*>

    <\equation*>
      \<forall\>\<rho\>\<in\>{L(r)+1, r}<with|mode|text| �����
      >\<Sigma\><rsub|j=0><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|\<rho\>-j>=0\<Rightarrow\>S<rsub|\<rho\>>=-\<Sigma\><rsub|j=1><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|\<rho\>-j><with|mode|text|.>
    </equation*>

    ��� ���� ��������:

    <\equation*>
      S<rsub|r><neg|=>-\<Sigma\><rsub|j=1><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|r-j><with|mode|text|
      , ��� <with|mode|math|r-j> ��������� �� <with|mode|math|r-1> ��
      <with|mode|math|r-L(r-1)>, ��� ������ <with|mode|math|L(r)>; >
    </equation*>

    <\equation*>
      S<rsub|r>=-\<Sigma\><rsub|j=1><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|r-j><with|mode|text|
      , ��� <with|mode|math|r-j> ��������� �� <with|mode|math|r-1> ��
      <with|mode|math|r-L(r)>, ��� ������ <with|mode|math|L(r-1)>.>
    </equation*>

    ����� ������� ���������� ������ ��������� �� ������ (��� ��������� ��
    ������ �� ��������� �� ������� ��������� ���������) ��������:

    <\equation*>
      S<rsub|r><neg|=>-\<Sigma\><rsub|j=1><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>(-\<Sigma\><rsub|i=1><rsup|L(r)>\<Lambda\><rsub|i><rsup|(r)>S<rsub|r-j-i>)<with|mode|text|,>
    </equation*>

    <\equation*>
      S<rsub|r>=-\<Sigma\><rsub|j=1><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>(-\<Sigma\><rsub|i=1><rsup|L(r-1)>\<Lambda\><rsub|i><rsup|(r-1)>S<rsub|r-j-i>)
    </equation*>

    -- ������������, ��� ��� ���� ����� ���������� �� ������ ���������������
    ����������.
  </proof>

  <\lemma>
    ����� <with|mode|math|\<forall\>i\<in\>{1,\<ldots\>,r-1}>
    <with|mode|math|\<Lambda\><rsup|(i)>(x)> ���������� ��������� �����������
    <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>, S<rsub|i>>;
    <with|mode|math|L(i)=deg\<Lambda\><rsup|(i)>(x)>; �
    <with|mode|math|\<Lambda\><rsup|(r-1)>(x)> �� ���������
    <with|mode|math|S<rsub|r>>. ����� <with|mode|math|\<star\>><with|mode|math|\<Lambda\><rsup|(r)>(x)
    = \<Lambda\><rsup|(r-1)>(x) - \<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>x<rsup|r-m>\<Lambda\><rsup|(m-1)>(x)>,
    ��� <with|mode|math|m> -- ��������� �����, ��� ��������� ����������
    (<with|mode|math|L(m-1)\<less\>L(m)>), ���������
    <with|mode|math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|r>>, �����
    ���������� �������, � <with|mode|math|deg\<Lambda\><rsup|(r)>(x)=max{L(r-1),
    r-L(r-1)}.>
  </lemma>

  <\proof>
    �� �� ������� �� <with|mode|math|m> �� �����
    <with|mode|math|L(r-1)=L(m)\<gtr\>L(m-1)>;

    <\equation*>
      <with|mode|text|<with|mode|math|\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|p-j>=<left|{><tabular|<tformat|<table|<row|<cell|0<with|mode|text|
      , ���� <with|mode|math|p\<in\>{L(r-1)+1,\<ldots\>,r-1}>>>>|<row|<cell|\<Delta\><rsub|r><with|mode|text|
      , ���� <with|mode|math|p=r>.>>>>>>>>
    </equation*>

    ����� �������, ���

    <\equation*>
      <with|mode|text|<with|mode|math|\<Sigma\><rsub|j=0><rsup|L(m-1)>\<Lambda\><rsub|j><rsup|(m-1)>S<rsub|p-j>=<left|{><tabular|<tformat|<table|<row|<cell|0<with|mode|text|
      , ���� <with|mode|math|p\<in\>{L(m-1)+1,\<ldots\>,m-1}>>>>|<row|<cell|\<Delta\><rsub|r><neg|=>0<with|mode|text|
      , ���� <with|mode|math|p=m>,>>>>>>>>
    </equation*>

    ��� ���� <with|mode|math|L(m)=m-L(m-1).>

    ����� ������� <with|mode|math|\<Lambda\><rsup|(r)>(x)> �� �������
    <with|mode|math|\<star\>>:

    <\equation*>
      <tabular|<tformat|<table|<row|<cell|deg\<Lambda\><rsup|(r)>(x)>|<cell|\<leqslant\>>|<cell|max{L(r-1),L(m-1)+r-m}>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|max{L(r-1),
      r-L(m)}>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|max{L(r-1),
      r-L(r-1)}.>|<cell|>>>>>
    </equation*>

    �������, ��� <with|mode|math|\<Lambda\><rsup|(r)>(x)> ���������
    <with|mode|math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|r>>, ����� �� �����
    <reference|�����1>: <with|mode|math|deg\<Lambda\><rsup|(r)>(x)\<geqslant\>max{L(r-1),r-L(r-1)}>,
    ����� �������, �������� ����������, �������
    <with|mode|math|\<Lambda\><rsup|(r)>(x)> ����� �����

    <\equation*>
      deg\<Lambda\><rsup|(r)>(x)\<geqslant\>max{L(r-1),r-L(r-1)},
    </equation*>

    �� ���� <with|mode|math|\<Lambda\><rsup|(r)>(x)> -- ��������� ����������
    ������� ����������� <with|mode|math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|r>>.

    ������� <with|mode|math|j>-� ���������� �
    <with|mode|math|\<Lambda\><rsup|(r)>(x)>:

    <\equation*>
      \<Lambda\><rsub|j><rsup|(r)>=\<Lambda\><rsub|j><rsup|(r-1)>-\<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>\<Lambda\><rsub|j-r+m><rsup|(m-1)><with|mode|text|
      , ��� <with|mode|math|\<Lambda\><rsub|i><rsup|(m-1)>=0>, ���
      <with|mode|math|i\<less\>0>.>
    </equation*>

    <\equation*>
      <tabular|<tformat|<table|<row|<cell|\<Sigma\><rsub|j=0><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|p-j>>|<cell|=>|<cell|\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|p-j>-\<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>\<Sigma\><rsub|j=0><rsup|L(m-1)>\<Lambda\><rsub|j><rsup|(m-1)>S<rsub|p-r+m-j>>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|[p\<in\>{L(r)+1,\<ldots\>,r}\<Rightarrow\>p-r+m\<leqslant\>m]>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|[\<Sigma\><rsub|j=0><rsup|L(m-1)>\<Lambda\><rsub|j><rsup|(m-1)>S<rsub|t>=<left|{><tabular|<tformat|<table|<row|<cell|0<with|mode|text|
      , ��� <with|mode|math|t \<less\> m>>>>|<row|<cell|\<Delta\><rsub|m><with|mode|text|
      , ��� <with|mode|math|t = m>>>>>>>]>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|<left|{><tabular|<tformat|<table|<row|<cell|0+0=0<with|mode|text|
      , ��� <with|mode|math|p \<leqslant\>
      r-1>>>>|<row|<cell|\<Delta\><rsub|r>-\<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>\<Delta\><rsub|m>=0<with|mode|text|
      , ��� <with|mode|math|p= m>>.>>>>>>|<cell|>>>>>
    </equation*>

    \;
  </proof>

  <tabular|<tformat|<cwith|6|6|6|6|cell-hyphen|b>|<table|<row|<cell|<with|mode|math|\|GF(2<rsup|m>)\|>>|<cell|<with|mode|math|g(x)>>|<cell|<with|mode|math|n>>|<cell|<with|mode|math|k>>|<cell|<with|mode|math|d<rsub|<with|mode|text|���>>>>|<cell|�����>>|<row|<cell|<with|mode|math|2<rsup|3>>>|<cell|<with|mode|math|x<rsup|3>+x+1>>|<cell|7>|<cell|4>|<cell|3>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>>>>|<row|<cell|<with|mode|math|2<rsup|4>>>|<cell|<with|mode|math|x<rsup|4>+x+1>>|<cell|15>|<cell|11>|<cell|3>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|4>+x+1)(x<rsup|4>+x<rsup|3>+x<rsup|2>+x+1)>>|<cell|15>|<cell|7>|<cell|5>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|4>+x+1)(x<rsup|4>+x<rsup|3>+x<rsup|2>+x+1)(x<rsup|2>+x+1)>>|<cell|15>|<cell|5>|<cell|7>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<alpha\><rsup|6>>>>|<row|<cell|<with|mode|math|2<rsup|5>>>|<cell|<with|mode|math|x<rsup|5>+x<rsup|2>+1>>|<cell|31>|<cell|26>|<cell|3>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|21>|<cell|5>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|4>+x<rsup|2>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|16>|<cell|7>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<alpha\><rsup|6>>>>|<row|<cell|>|<cell|<with|mode|math|><with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|3>+x<rsup|2>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|2>+x+1)>>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|11>|<cell|11>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<ldots\>>>>|<row|<cell|>|<cell|<with|mode|math|><with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|3>+x<rsup|2>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|2>+x+1)>>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|4>+x<rsup|3>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|6>|<cell|15>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<ldots\>>>>|<row|<cell|<with|mode|math|2<rsup|6>>>|<cell|>|<cell|63>|<cell|57>|<cell|3>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|51>|<cell|5>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|45>|<cell|7>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|39>|<cell|9>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|36>|<cell|11>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|30>|<cell|13>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|24>|<cell|15>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|18>|<cell|21>|<cell|>>>>>
</body>

<\initial>
  <\collection>
    <associate|font|cyrillic>
    <associate|language|russian>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|�����1|<tuple|10|4>>
    <associate|auto-1|<tuple|1|1>>
    <associate|auto-2|<tuple|2|3>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|1<space|2spc>����
      ��� (����� - �������� - ����������)>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-1><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|2<space|2spc>��������
      ����������> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>