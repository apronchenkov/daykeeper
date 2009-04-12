<TeXmacs|1.0.6>

<style|article>

<\body>
  <section|Коды БЧХ (Боуза - Чоудхури - Хоквингема)>

  <\theorem>
    (Граница БЧХ)

    Пусть <with|mode|math|g(x)> порождает код <with|mode|math|C> и среди его
    корней есть

    <\equation*>
      \<alpha\><rsup|m>, \<alpha\><rsup|m + j>, \<alpha\><rsup|m + 2j>,
      \<ldots\> ,\<alpha\><rsup|m + (d-2)j>,
    </equation*>

    где <with|mode|math|\<alpha\><rsup|j>> -- примитивный элемент поля
    разложения <with|mode|math|g(x)>. Тогда минимальное расстояние кода
    <with|mode|math|C> не менее d.
  </theorem>

  <\proof>
    (от противного) Предположим <with|mode|math|d(C) \<less\> d>, то есть в
    <with|mode|math|C> есть кодовое слово расстояние которого меньше d.
    Выпишем его в виде многочлена

    <\equation*>
      f(x) = \<beta\><rsub|1>x<rsup|i<rsub|1>>+\<beta\><rsub|2>x<rsup|i<rsub|2>>
      + \<ldots\>+\<beta\><rsub|d-1>x<rsup|i<rsub|d-1>>,
    </equation*>

    где, возможно, некоторые <with|mode|math|\<beta\><rsub|i>> нулевые, но
    сам <with|mode|math|f(x)> отличен от нуля.

    По определению циклических кодов <with|mode|math|g(x)\|f(x)>, значит все
    корни <with|mode|math|g(x)> являются конями <with|mode|math|f(x)>.

    <\equation*>
      <left|{><tabular|<tformat|<cwith|3|3|1|1|cell-halign|c>|<table|<row|<cell|\<beta\><rsub|1>\<alpha\><rsup|m*i<rsub|1>>
      + \<beta\><rsub|2>\<alpha\><rsup|m*i<rsub|2>>+\<ldots\>+\<beta\><rsub|d-1>\<alpha\><rsup|m*i<rsub|d-1>>>|<cell|=>|<cell|0>>|<row|<cell|\<beta\><rsub|1>\<alpha\><rsup|(m*+j)i<rsub|1>>
      + \<beta\><rsub|2>\<alpha\><rsup|(m+j)i<rsub|2>>+\<ldots\>+\<beta\><rsub|d-1>\<alpha\><rsup|(m+j)i<rsub|d-1>>>|<cell|=>|<cell|0>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<beta\><rsub|1>\<alpha\><rsup|(m*+(d-2)j)i<rsub|1>>
      + \<beta\><rsub|2>\<alpha\><rsup|(m+(d-2)j)i<rsub|2>>+\<ldots\>+\<beta\><rsub|d-1>\<alpha\><rsup|(m+(d-2)j)i<rsub|d-1>>>|<cell|=>|<cell|0>>>>>
    </equation*>

    Это система из <with|mode|math|d-1> уравнения с <with|mode|math|d-1>
    неизвестным <with|mode|math|\<beta\><rsub|1>, \<beta\><rsub|2 >,
    \<ldots\>, \<beta\><rsub|d-1>>. Определитель системы:

    <\equation*>
      \<Delta\>=\<alpha\><rsup|m*i<rsub|1>>\<alpha\><rsup|m*i<rsub|2>>*\<ldots\>*\<alpha\><rsup|m*i<rsub|d-1>><det|<tformat|<table|<row|<cell|1>|<cell|1>|<cell|\<ldots\>>|<cell|1>>|<row|<cell|\<alpha\><rsup|j*i<rsub|1>>>|<cell|\<alpha\><rsup|j*i<rsub|2>>>|<cell|\<ldots\>>|<cell|\<alpha\><rsup|j*i<rsub|d-1>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|\<alpha\><rsup|(d-2)j*i<rsub|1>>>|<cell|\<alpha\><rsup|(d-2)j*i<rsub|2>>>|<cell|\<ldots\>>|<cell|\<alpha\><rsup|(d-2)j*i<rsub|d-1>>>>>>><neg|
      = >0,
    </equation*>

    -- отличен от нуля, следовательно имеется единственное решение системы,
    <with|mode|math|\<beta\><rsub|i>=0>, <with|mode|math|\<forall\>i>.
    Противоречие с условием <with|mode|math|f(x) <neg|=>0.>
  </proof>

  <\definition>
    Пусть даны <with|mode|math|d, m<rsub|0>\<in\>\<bbb-Z\>> и
    <with|mode|math|\<alpha\>\<in\>GF(2<rsup|m>)>, <with|mode|math|\<alpha\>>
    примитивный корень. Код БЧХ -- код с порождающим многочленом
    <with|mode|math|g(x)\<in\>GF(2)[x]>, наименьшей степени среди корней
    которого есть <with|mode|math|\<alpha\><rsup|m<rsub|0>>,
    \<alpha\><rsup|m<rsub|0>+1>, \<ldots\>, \<alpha\><rsup|m<rsub|0> + d-2>>.
    Если <with|mode|math|m<rsub|0>=1>, тогда это примитивный БЧХ-код.
  </definition>

  <\remark>
    <with|mode|math|g(x)\|x<rsup|2<rsup|m>-1>-1>.

    Это верно, так как каждый элемент <with|mode|math|GF(2<rsup|m>)> является
    корнем <with|mode|math|x<rsup|2<rsup|m>>-x>, и
    <with|mode|math|GF(2<rsup|m>)> является поле разложения
    <with|mode|math|g(x)>.
  </remark>

  <\remark>
    Длинна БЧХ-кода <with|mode|math|n=2<rsup|m>-1>.

    Это следует из условия, что <with|mode|math|g(x) \| x<rsup|n>-1> и
    <with|mode|math|n> -- минимальное с таким своиством.
  </remark>

  <\theorem>
    Приминивный БЧХ-код способен исправить
    <with|mode|math|t\<leqslant\><frac|d-1|2>> ошибок.
  </theorem>

  <\proof>
    Предположим, что при передаче сообщения произошло
    <with|mode|math|\<nu\>\<leqslant\>t> ошибок:

    <\equation*>
      i(x)g(x) \<longrightarrow\><rsub|+e(x)> f(x)=i(x)g(x) + e(x),\ 
    </equation*>

    где <with|mode|math|e(x)> многочлен ошибок:

    <\equation*>
      e(x)=x<rsup|i<rsub|1>>+x<rsup|i<rsub|2>>+\<ldots\>+x<rsup|i<rsub|\<nu\>>>
      <with|mode|text|, где >i<rsub|j>\<less\>2<rsup|m>-1.
    </equation*>

    Корнями многочлена <with|mode|math|g(x)> являются
    <with|mode|math|\<alpha\>, \ \<alpha\><rsup|2>, \<ldots\>,
    \<alpha\><rsup|d-1>>, таким образом <with|mode|math|f(\<alpha\><rsup|j>)=i(\<alpha\><rsup|j>)*g(\<alpha\><rsup|j>)
    + e(\<alpha\><rsup|j>)=e(\<alpha\><rsup|j>), \ <with|mode|text|при
    >j\<in\>{1, 2, \<ldots\>, d-1}.> Рассмотрим следующие объекты:

    <\equation*>
      <with|mode|text|<with|mode|math|S<rsub|j>=f(\<alpha\><rsup|j>)=e(\<alpha\><rsup|j>)>>,
    </equation*>

    \ -- синдромы.

    Задача исправления ошибок ставится следующим образом: имеются
    <with|mode|math|d-1> синдромов <with|mode|math|S<rsub|j>>, необходимо
    востановить многочлен ошибок <with|mode|math|e(x)>.

    Многочлен <with|mode|math|e(x)> определяется коэффицентами
    <with|mode|math|\<nu\>> и <with|mode|math|i<rsub|1>, i<rsub|2>,
    \<ldots\>, i<rsub|\<nu\>>>.

    Введјм дополнительно вспомогательные объекты:
    <with|mode|math|X<rsub|1>=\<alpha\><rsup|i<rsub|1>>,
    X<rsub|2>=\<alpha\><rsup|i<rsub|2>>, \<ldots\>,
    X<rsub|\<nu\>>=\<alpha\><rsup|i<rsub|\<nu\>>>> -- локаторы ошибок.

    В таких обозначениях:

    <\equation*>
      <tabular|<tformat|<cwith|1|4|3|3|cell-halign|c>|<table|<row|<cell|e(\<alpha\>)>|<cell|=>|<cell|X<rsub|1>+X<rsub|2>+\<ldots\>+X<rsub|\<nu\>>>|<cell|=>|<cell|S<rsub|1>>>|<row|<cell|e(\<alpha\><rsup|2>)>|<cell|=>|<cell|X<rsub|1><rsup|2>+X<rsub|2><rsup|2>+\<ldots\>+X<rsub|\<nu\>><rsup|2>>|<cell|=>|<cell|S<rsub|2>>>|<row|<cell|>|<cell|>|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|e(\<alpha\><rsup|2t>)>|<cell|=>|<cell|X<rsub|1><rsup|2t>+X<rsub|2><rsup|2t>+\<ldots\>+X<rsub|\<nu\>><rsup|2t>>|<cell|=>|<cell|S<rsub|2t>>>>>>
    </equation*>

    Все локаторы различны, так как <with|mode|math|X<rsup|k>=X<rsup|l>
    \<Leftrightarrow\> \<alpha\><rsup|i<rsub|k>>=\<alpha\><rsup|i<rsub|l>>
    \<Leftrightarrow\> i<rsub|k>\<equiv\>i<rsub|l>(mod 2<rsup|m>-1)>, но
    <with|mode|math|i<rsub|k>,i<rsub|l>\<less\>2<rsup|m>-1>; и отличны от
    нуля.

    Рассмотрим матрицу:

    <\equation*>
      <tabular|<tformat|<table|<row|<cell|M=<matrix|<tformat|<table|<row|<cell|S<rsub|1>>|<cell|S<rsub|2>>|<cell|\<ldots\>>|<cell|S<rsub|t>>>|<row|<cell|S<rsub|2>>|<cell|S<rsub|3>>|<cell|\<ldots\>>|<cell|S<rsub|t+1>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|S<rsub|t>>|<cell|S<rsub|t+1>>|<cell|\<ldots\>>|<cell|S<rsub|2t-1>>>>>>=<matrix|<tformat|<table|<row|<cell|1>|<cell|1>|<cell|\<ldots\>>|<cell|1>>|<row|<cell|X<rsub|1>>|<cell|X<rsub|2>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|X<rsub|1><rsup|t-1>>|<cell|X<rsub|2><rsup|t-1>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t-1>>>>>>*<matrix|<tformat|<table|<row|<cell|X<rsub|1>>|<cell|X<rsub|1><rsup|2>>|<cell|\<ldots\>>|<cell|X<rsub|1><rsup|t>>>|<row|<cell|X<rsub|2>>|<cell|X<rsub|2><rsup|2>>|<cell|\<ldots\>>|<cell|X<rsub|2><rsup|t>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|X<rsub|\<nu\>>>|<cell|X<rsub|\<nu\>><rsup|2>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t>>>>>>=>>|<row|<cell|=<matrix|<tformat|<table|<row|<cell|1>|<cell|1>|<cell|\<ldots\>>|<cell|1>>|<row|<cell|X<rsub|1>>|<cell|X<rsub|2>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|X<rsub|1><rsup|t-1>>|<cell|X<rsub|2><rsup|t-1>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t-1>>>>>><matrix|<tformat|<table|<row|<cell|X<rsub|1>>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|X<rsub|2>>|<cell|>|<cell|>>|<row|<cell|>|<cell|>|<cell|\<ldots\>>|<cell|>>|<row|<cell|>|<cell|>|<cell|>|<cell|X<rsub|\<nu\>>>>>>><matrix|<tformat|<table|<row|<cell|1>|<cell|X<rsub|1>>|<cell|\<ldots\>>|<cell|X<rsub|1><rsup|t-1>>>|<row|<cell|1>|<cell|X<rsub|2>>|<cell|\<ldots\>>|<cell|X<rsub|2><rsup|t-1>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|1>|<cell|X<rsub|\<nu\>>>|<cell|\<ldots\>>|<cell|X<rsub|\<nu\>><rsup|t-1>>>>>>=W*I*W<rsup|T>.>>>>>
    </equation*>

    Ранг матриц <with|mode|math|W> и <with|mode|math|I> равен
    <with|mode|math|\<nu\>>, следовательно ранг матрицы <with|mode|math|M>
    так же равен <with|mode|math|\<nu\>>.

    Рассмотрим локаторный многочлен:

    <\equation*>
      \<Lambda\>(x) = (1-x*X<rsub|1>)(1-x*X<rsub|2>)\<ldots\>(1-x*X<rsub|\<nu\>>)=\<Lambda\><rsub|\<nu\>>x<rsup|\<nu\>>+\<Lambda\><rsub|\<nu\>-1>x<rsup|\<nu\>-1>+\<ldots\>+\<Lambda\><rsub|1>x+1.
    </equation*>

    Его корни обратны локаторам. Подставим вместо <with|mode|math|x> локатор
    <with|mode|math|X<rsub|i><rsup|-1>>, получим:

    <\equation*>
      \<Lambda\><rsub|\<nu\>>X<rsub|i><rsup|-\<nu\>>+\<Lambda\><rsub|\<nu\>-1>X<rsub|i><rsup|1-\<nu\>>+\<ldots\>+\<Lambda\><rsub|1>X<rsub|i><rsup|-1>+1=0,<with|mode|text|
      умножим на <with|mode|math|X<rsub|i><rsup|j+\<nu\>>>,>
    </equation*>

    <\equation*>
      \<Lambda\><rsub|\<nu\>>X<rsub|i><rsup|j>+\<Lambda\><rsub|\<nu\>-1>X<rsub|i><rsup|j+1>+\<ldots\>+\<Lambda\><rsub|1>X<rsub|i><rsup|j+\<nu\>-1>+X<rsub|i><rsup|j+\<nu\>>=0,<with|mode|text|
      сложим по всем <with|mode|math|i\<in\>{1\<ldots\>\<nu\>}>.>
    </equation*>

    <\equation*>
      \<Lambda\><rsub|\<nu\>>S<rsub|j>+\<Lambda\><rsub|\<nu\>-1>S<rsub|j+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|j+\<nu\>-1>+S<rsub|j+\<nu\>>=0.
    </equation*>

    Получаем систему линейных уравнений с неизвествными
    <with|mode|math|\<Lambda\><rsub|i>>:

    <\equation*>
      \<star\><left|{><tabular|<tformat|<cwith|1|4|1|1|cell-halign|c>|<table|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|1>+\<Lambda\><rsub|\<nu\>-1>S<rsub|2>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>><rsub|>>|<cell|=>|<cell|-S<rsub|\<nu\>+1>>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|2>+\<Lambda\><rsub|\<nu\>-1>S<rsub|3>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>+1>>|<cell|=>|<cell|-S<rsub|\<nu\>+2>>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|\<nu\>>+\<Lambda\><rsub|\<nu\>-1>S<rsub|\<nu\>+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|2\<nu\>-1>>|<cell|=>|<cell|-S<rsub|2\<nu\>>>>>>>
    </equation*>

    Матрица этой системы:

    <\equation*>
      <matrix|<tformat|<table|<row|<cell|S<rsub|1>>|<cell|S<rsub|2>>|<cell|\<ldots\>>|<cell|S<rsub|\<nu\>>>>|<row|<cell|S<rsub|2>>|<cell|S<rsub|3>>|<cell|\<ldots\>>|<cell|S<rsub|\<nu\>+1>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|S<rsub|\<nu\>>>|<cell|S<rsub|\<nu\>+1>>|<cell|\<ldots\>>|<cell|S<rsub|2\<nu\>-1>>>>>>
    </equation*>

    -- является подматрицией матрицы <with|mode|math|M> и имеет размер
    <with|mode|math|\<nu\>\<times\>\<nu\>>, следовательно является линейно
    независимой. Cистема Крамеровская и <with|mode|math|\<Lambda\><rsub|1>,
    \<Lambda\><rsub|2>, \<ldots\>, \<Lambda\><rsub|\<nu\>>> находятся.
  </proof>

  <\remark>
    Система <with|mode|math|\<star\>> в общем виде выглядит иначе:

    <\equation*>
      <left|{><tabular|<tformat|<cwith|1|4|1|1|cell-halign|c>|<table|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|1>+\<Lambda\><rsub|\<nu\>-1>S<rsub|2>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>><rsub|>>|<cell|=>|<cell|-S<rsub|\<nu\>+1>>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|2>+\<Lambda\><rsub|\<nu\>-1>S<rsub|3>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>+1>>|<cell|=>|<cell|-S<rsub|\<nu\>+2>>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|t>+\<Lambda\><rsub|\<nu\>-1>S<rsub|t+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|2t-1>>|<cell|=>|<cell|-S<rsub|2t>>>>>>
    </equation*>

    но, находясь в рамках условия теоремы, решение сокращјнной системы
    является решением общей.

    В общем случае, вообще говоря это не верно.
  </remark>

  <\proposition>
    Алгоритм исправления ошибок:

    <\enumerate-numeric>
      <item>(предварительные вычисления) Вычисляем <with|mode|math|S<rsub|1>,
      S<rsub|2>, \<ldots\>, S<rsub|2t>>.

      <item>(инициализация) <with|mode|math|\<nu\>=t>.

      <item>(шаг цикла) Считаем <with|mode|math|\<Delta\>=<det|<tformat|<table|<row|<cell|S<rsub|1>>|<cell|\<ldots\>>|<cell|S<rsub|\<nu\>>>>|<row|<cell|\<ldots\>>|<cell|\<ldots\>>|<cell|\<ldots\>>>|<row|<cell|S<rsub|\<nu\>>>|<cell|\<ldots\>>|<cell|S<rsub|2\<nu\>-1>>>>>>>.

      <item>(условие цикла) Если <with|mode|math|\<Delta\> =0>, то
      <with|mode|math|\<nu\>=\<nu\>-1> и повторяем шаг 3.

      <item>(вычисляем коэффиценты <with|mode|math|\<Lambda\>(x)>) Решаем
      систему, находим <with|mode|math|><with|mode|math|\<Lambda\><rsub|1>,
      \<Lambda\><rsub|2>, \<ldots\>, \<Lambda\><rsub|\<nu\>>.>

      <item>(поиск локаторов) Подбираем корни <with|mode|math|\<Lambda\>(x)>,
      находим <with|mode|math|X<rsub|1>, X<rsub|2>, \<ldots\>,
      X<rsub|\<nu\>>>.

      <item>(востанавливаем <with|mode|math|e(x)>) Находим
      <with|mode|math|i<rsub|1>, i<rsub|2>, \<ldots\>, i<rsub|\<nu\>>>.
    </enumerate-numeric>
  </proposition>

  <section|Алгоритм Берлекэмпа>

  Постановка задачи. Даны <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>,
  S<rsub|2t>\<in\>GF(2<rsup|m>)> и система

  <\equation*>
    <left|{><tabular|<tformat|<cwith|1|4|1|1|cell-halign|c>|<table|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|1>+\<Lambda\><rsub|\<nu\>-1>S<rsub|2>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>><rsub|>>|<cell|=>|<cell|-S<rsub|\<nu\>+1>>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|2>+\<Lambda\><rsub|\<nu\>-1>S<rsub|3>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|\<nu\>+1>>|<cell|=>|<cell|-S<rsub|\<nu\>+2>>>|<row|<cell|\<ldots\>>|<cell|>|<cell|>>|<row|<cell|\<Lambda\><rsub|\<nu\>>S<rsub|t>+\<Lambda\><rsub|\<nu\>-1>S<rsub|t+1>+\<ldots\>+\<Lambda\><rsub|1>S<rsub|2t-1>>|<cell|=>|<cell|-S<rsub|2t>>>>>>,
    \<nu\> \<leqslant\>t.
  </equation*>

  Необходимо найти решение, состоящее из: наименьшего <with|mode|math|\<nu\>>
  и <with|mode|math|\<Lambda\><rsub|1>, \<Lambda\><rsub|2>, \<ldots\>,
  \<Lambda\><rsub|\<nu\>>\<in\>GF(2<rsup|m>)>.

  Пусть <with|mode|math|\<Lambda\>(x)> -- многочлено степени
  <with|mode|math|L> со свободным члено <with|mode|math|1>
  (<with|mode|math|\<Lambda\><rsub|0>=1>).

  <\definition>
    <with|mode|math|\<Lambda\>(x)> порождает <with|mode|math|S<rsub|1>,
    S<rsub|2>, \<ldots\>, S<rsub|r>>, если
    <with|mode|math|\<forall\>\<rho\>\<in\>{L+1, \<ldots\>, r}> выполняется

    <\equation*>
      \<Sigma\><rsub|j=0><rsup|L>\<Lambda\><rsub|j>S<rsub|p-j>=0.
    </equation*>
  </definition>

  Теперь задачу можно сформулировать иначе: необходимо найти многочлен
  наименьшей степени порождающий все <with|mode|math|S<rsub|1>, S<rsub|2>,
  \<ldots\>, S<rsub|2t>>.

  Будем действовать интерационно, подгоняя <with|mode|math|\<Lambda\>(x)> под
  синдромы.

  <\proposition>
    Алгоритм Берлекэмпа:

    <\enumerate-numeric>
      <item>(инициализация) <with|mode|math|\<Lambda\><rsup|(0)>(x)=1<with|mode|text|
      -- многочлен который мы строим>>, <with|mode|math|B<rsup|(0)>(x) = 1,
      L(0) = 0, r=1<with|mode|text| -- вспомогательные элементы>>.

      <item>(строим неувязку) <with|mode|math|\<Delta\><rsub|r>=\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|r-j>>.

      <item>(<with|mode|math|\<Delta\><rsub|r>=0>) Если
      <with|mode|math|\<Delta\><rsub|r>=0>, то
      <with|mode|math|\<Lambda\><rsup|(r)>(x)=\<Lambda\><rsup|(r-1)>(x)>,
      <with|mode|math|L(r)=L(r-1)>.

      <item>(<with|mode|math|\<Delta\><rsub|r><neg|=>0>) Если
      <with|mode|math|\<Delta\><rsub|r><neg|=>0>, то
      <with|mode|math|\<Lambda\><rsup|(r)>(x)=\<Lambda\><rsup|(r-1)>(x)-\<Delta\><rsub|r>x*B<rsup|(r-1)>(x)>,
      <with|mode|math|L(r) = max{L(r-1), \ r - L(r-1)}>.

      <item>(пересчитываем <with|mode|math|B<rsup|(r)>>)

      <\equation*>
        B<rsup|(r)>(x)=<left|{><tabular|<tformat|<table|<row|<cell|B<rsup|(r-1)>(x)x<with|mode|text|,
        если <with|mode|math|L(r)=L(r-1)>>>>|<row|<cell|\<Delta\><rsub|r><rsup|-1>\<Lambda\><rsup|(r-1)>(x)<with|mode|text|,
        если <with|mode|math|L(r)\<gtr\>L(r-1)>>.>>>>>
      </equation*>

      <item>(шаг) Если <with|mode|math|r\<less\>2t>, то продолжить выполнение
      с шага 2 при <with|mode|math|r=r+1>.
    </enumerate-numeric>
  </proposition>

  Докажем корректность.

  <\lemma>
    <label|лемма1>Пусть <with|mode|math|\<Lambda\><rsup|(r-1)>(x)> и
    <with|mode|math|\<Lambda\><rsup|(r)>(x)> многочлены наименьших степеней
    порождающие синдромы <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>,
    S<rsub|r-1>> и <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>,
    S<rsub|r>> соответственно; <with|mode|math|L(r-1) =
    deg\<Lambda\><rsup|(r-1)>(x)>, <with|mode|math|L(r) =
    deg\<Lambda\><rsup|(r)>(x)> и <with|mode|math|\<Lambda\><rsup|(r-1)>(x)
    <neg|=>\<Lambda\><rsup|(r)>(x)>, тогда
    <with|mode|math|L(r)\<geqslant\>max{L(r-1), r-L(r-1)}.>
  </lemma>

  <\proof>
    Из минимальности <with|mode|math|\<Lambda\><rsup|(r-1)>> следует, что
    <with|mode|math|L<rsup|(r-1)>\<leqslant\>L<rsup|(r)>>, остајтся доказать,
    что <with|mode|math|r-L(r-1)\<leqslant\>L(r)>. Предположим обратное:

    <\equation*>
      r-L(r-1)\<gtr\>L(r)<with|mode|text| или, что тоже самое,
      >r-L(r)\<gtr\>L(r-1).
    </equation*>

    Из условия теоремы, мы имеем:

    <\equation*>
      \<forall\>\<rho\>\<in\>{L(r-1)+1, r-1}<with|mode|text| имеем
      >\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|\<rho\>-j>=0\<Rightarrow\>S<rsub|\<rho\>>=-\<Sigma\><rsub|j=1><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|\<rho\>-j><with|mode|text|;>
    </equation*>

    <\equation*>
      \<forall\>\<rho\>\<in\>{L(r)+1, r}<with|mode|text| имеем
      >\<Sigma\><rsub|j=0><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|\<rho\>-j>=0\<Rightarrow\>S<rsub|\<rho\>>=-\<Sigma\><rsub|j=1><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|\<rho\>-j><with|mode|text|.>
    </equation*>

    При этом получаем:

    <\equation*>
      S<rsub|r><neg|=>-\<Sigma\><rsub|j=1><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|r-j><with|mode|text|
      , где <with|mode|math|r-j> пробегает от <with|mode|math|r-1> до
      <with|mode|math|r-L(r-1)>, что больше <with|mode|math|L(r)>; >
    </equation*>

    <\equation*>
      S<rsub|r>=-\<Sigma\><rsub|j=1><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|r-j><with|mode|text|
      , где <with|mode|math|r-j> пробегает от <with|mode|math|r-1> до
      <with|mode|math|r-L(r)>, что больше <with|mode|math|L(r-1)>.>
    </equation*>

    Таким образом подставляя первые уравнения во вторые (что допустимо из
    оценок на интервалы по которым пробегают параметры) получаем:

    <\equation*>
      S<rsub|r><neg|=>-\<Sigma\><rsub|j=1><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>(-\<Sigma\><rsub|i=1><rsup|L(r)>\<Lambda\><rsub|i><rsup|(r)>S<rsub|r-j-i>)<with|mode|text|,>
    </equation*>

    <\equation*>
      S<rsub|r>=-\<Sigma\><rsub|j=1><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>(-\<Sigma\><rsub|i=1><rsup|L(r-1)>\<Lambda\><rsub|i><rsup|(r-1)>S<rsub|r-j-i>)
    </equation*>

    -- противоречие, так как одна сумма получается из другой переименованием
    переменных.
  </proof>

  <\lemma>
    Пусть <with|mode|math|\<forall\>i\<in\>{1,\<ldots\>,r-1}>
    <with|mode|math|\<Lambda\><rsup|(i)>(x)> наименьший многочлен порождающий
    <with|mode|math|S<rsub|1>, S<rsub|2>, \<ldots\>, S<rsub|i>>;
    <with|mode|math|L(i)=deg\<Lambda\><rsup|(i)>(x)>; и
    <with|mode|math|\<Lambda\><rsup|(r-1)>(x)> не порождает
    <with|mode|math|S<rsub|r>>. Тогда <with|mode|math|\<star\>><with|mode|math|\<Lambda\><rsup|(r)>(x)
    = \<Lambda\><rsup|(r-1)>(x) - \<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>x<rsup|r-m>\<Lambda\><rsup|(m-1)>(x)>,
    где <with|mode|math|m> -- последний номер, где произошло увеличение
    (<with|mode|math|L(m-1)\<less\>L(m)>), порождает
    <with|mode|math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|r>>, имеет
    наименьшую степень, и <with|mode|math|deg\<Lambda\><rsup|(r)>(x)=max{L(r-1),
    r-L(r-1)}.>
  </lemma>

  <\proof>
    По из условия на <with|mode|math|m> мы имеем
    <with|mode|math|L(r-1)=L(m)\<gtr\>L(m-1)>;

    <\equation*>
      <with|mode|text|<with|mode|math|\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|p-j>=<left|{><tabular|<tformat|<table|<row|<cell|0<with|mode|text|
      , если <with|mode|math|p\<in\>{L(r-1)+1,\<ldots\>,r-1}>>>>|<row|<cell|\<Delta\><rsub|r><with|mode|text|
      , если <with|mode|math|p=r>.>>>>>>>>
    </equation*>

    Можно считать, что

    <\equation*>
      <with|mode|text|<with|mode|math|\<Sigma\><rsub|j=0><rsup|L(m-1)>\<Lambda\><rsub|j><rsup|(m-1)>S<rsub|p-j>=<left|{><tabular|<tformat|<table|<row|<cell|0<with|mode|text|
      , если <with|mode|math|p\<in\>{L(m-1)+1,\<ldots\>,m-1}>>>>|<row|<cell|\<Delta\><rsub|r><neg|=>0<with|mode|text|
      , если <with|mode|math|p=m>,>>>>>>>>
    </equation*>

    при этом <with|mode|math|L(m)=m-L(m-1).>

    Найдјм степень <with|mode|math|\<Lambda\><rsup|(r)>(x)> из формулы
    <with|mode|math|\<star\>>:

    <\equation*>
      <tabular|<tformat|<table|<row|<cell|deg\<Lambda\><rsup|(r)>(x)>|<cell|\<leqslant\>>|<cell|max{L(r-1),L(m-1)+r-m}>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|max{L(r-1),
      r-L(m)}>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|max{L(r-1),
      r-L(r-1)}.>|<cell|>>>>>
    </equation*>

    Покажем, что <with|mode|math|\<Lambda\><rsup|(r)>(x)> порождает
    <with|mode|math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|r>>, тогда по Лемме
    <reference|лемма1>: <with|mode|math|deg\<Lambda\><rsup|(r)>(x)\<geqslant\>max{L(r-1),r-L(r-1)}>,
    таким образом, учитывая предыдущее, степень
    <with|mode|math|\<Lambda\><rsup|(r)>(x)> будет точно

    <\equation*>
      deg\<Lambda\><rsup|(r)>(x)\<geqslant\>max{L(r-1),r-L(r-1)},
    </equation*>

    то есть <with|mode|math|\<Lambda\><rsup|(r)>(x)> -- многочлен наименьшей
    степени порождающий <with|mode|math|S<rsub|1>,S<rsub|2>,\<ldots\>,S<rsub|r>>.

    Считаем <with|mode|math|j>-й коэффицент в
    <with|mode|math|\<Lambda\><rsup|(r)>(x)>:

    <\equation*>
      \<Lambda\><rsub|j><rsup|(r)>=\<Lambda\><rsub|j><rsup|(r-1)>-\<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>\<Lambda\><rsub|j-r+m><rsup|(m-1)><with|mode|text|
      , где <with|mode|math|\<Lambda\><rsub|i><rsup|(m-1)>=0>, для
      <with|mode|math|i\<less\>0>.>
    </equation*>

    <\equation*>
      <tabular|<tformat|<table|<row|<cell|\<Sigma\><rsub|j=0><rsup|L(r)>\<Lambda\><rsub|j><rsup|(r)>S<rsub|p-j>>|<cell|=>|<cell|\<Sigma\><rsub|j=0><rsup|L(r-1)>\<Lambda\><rsub|j><rsup|(r-1)>S<rsub|p-j>-\<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>\<Sigma\><rsub|j=0><rsup|L(m-1)>\<Lambda\><rsub|j><rsup|(m-1)>S<rsub|p-r+m-j>>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|[p\<in\>{L(r)+1,\<ldots\>,r}\<Rightarrow\>p-r+m\<leqslant\>m]>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|[\<Sigma\><rsub|j=0><rsup|L(m-1)>\<Lambda\><rsub|j><rsup|(m-1)>S<rsub|t>=<left|{><tabular|<tformat|<table|<row|<cell|0<with|mode|text|
      , при <with|mode|math|t \<less\> m>>>>|<row|<cell|\<Delta\><rsub|m><with|mode|text|
      , при <with|mode|math|t = m>>>>>>>]>|<cell|=>>|<row|<cell|>|<cell|=>|<cell|<left|{><tabular|<tformat|<table|<row|<cell|0+0=0<with|mode|text|
      , при <with|mode|math|p \<leqslant\>
      r-1>>>>|<row|<cell|\<Delta\><rsub|r>-\<Delta\><rsub|r>\<Delta\><rsub|m><rsup|-1>\<Delta\><rsub|m>=0<with|mode|text|
      , при <with|mode|math|p= m>>.>>>>>>|<cell|>>>>>
    </equation*>

    \;
  </proof>

  <tabular|<tformat|<cwith|6|6|6|6|cell-hyphen|b>|<table|<row|<cell|<with|mode|math|\|GF(2<rsup|m>)\|>>|<cell|<with|mode|math|g(x)>>|<cell|<with|mode|math|n>>|<cell|<with|mode|math|k>>|<cell|<with|mode|math|d<rsub|<with|mode|text|БЧХ>>>>|<cell|корни>>|<row|<cell|<with|mode|math|2<rsup|3>>>|<cell|<with|mode|math|x<rsup|3>+x+1>>|<cell|7>|<cell|4>|<cell|3>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>>>>|<row|<cell|<with|mode|math|2<rsup|4>>>|<cell|<with|mode|math|x<rsup|4>+x+1>>|<cell|15>|<cell|11>|<cell|3>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|4>+x+1)(x<rsup|4>+x<rsup|3>+x<rsup|2>+x+1)>>|<cell|15>|<cell|7>|<cell|5>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|4>+x+1)(x<rsup|4>+x<rsup|3>+x<rsup|2>+x+1)(x<rsup|2>+x+1)>>|<cell|15>|<cell|5>|<cell|7>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<alpha\><rsup|6>>>>|<row|<cell|<with|mode|math|2<rsup|5>>>|<cell|<with|mode|math|x<rsup|5>+x<rsup|2>+1>>|<cell|31>|<cell|26>|<cell|3>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|21>|<cell|5>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>>>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|4>+x<rsup|2>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|16>|<cell|7>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<alpha\><rsup|6>>>>|<row|<cell|>|<cell|<with|mode|math|><with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|3>+x<rsup|2>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|2>+x+1)>>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|11>|<cell|11>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<ldots\>>>>|<row|<cell|>|<cell|<with|mode|math|><with|mode|math|(x<rsup|5>+x<rsup|2>+1)(x<rsup|5>+x<rsup|3>+x<rsup|2>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|2>+x+1)>>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|<with|mode|math|(x<rsup|5>+x<rsup|4>+x<rsup|3>+x+1)(x<rsup|5>+x<rsup|4>+x<rsup|3>+x<rsup|2>+1)>>|<cell|31>|<cell|6>|<cell|15>|<cell|<with|mode|math|\<alpha\>,\<alpha\><rsup|2>,\<alpha\><rsup|3>,\<alpha\><rsup|4>,\<alpha\><rsup|5>,\<ldots\>>>>|<row|<cell|<with|mode|math|2<rsup|6>>>|<cell|>|<cell|63>|<cell|57>|<cell|3>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|51>|<cell|5>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|45>|<cell|7>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|39>|<cell|9>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|36>|<cell|11>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|30>|<cell|13>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|24>|<cell|15>|<cell|>>|<row|<cell|>|<cell|>|<cell|63>|<cell|18>|<cell|21>|<cell|>>>>>
</body>

<\initial>
  <\collection>
    <associate|font|cyrillic>
    <associate|language|russian>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|лемма1|<tuple|10|4>>
    <associate|auto-1|<tuple|1|1>>
    <associate|auto-2|<tuple|2|3>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|1<space|2spc>Коды
      БЧХ (Боуза - Чоудхури - Хоквингема)>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-1><vspace|0.5fn>

      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|2<space|2spc>Алгоритм
      Берлекэмпа> <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>