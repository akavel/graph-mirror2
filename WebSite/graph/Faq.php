<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <title>Graph - Frequently asked questions</title>


  <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">

  <link rel="stylesheet" href="Graph.css" type="text/css">

</head>


<body>

<div id="main">
<h1 align="center"><font color="#0000ff" size="+1"><b> <font size="+7">FAQ</font></b></font></h1>

<p><font size="+1"><b>Q:</b> <b>What
are the system requirements of the program?</b></font><br>

<font size="+1"><b>A:</b> The program
requires 32 bit Windows. It has been tested under Windows 95, Windows
98, Windows ME, Windows 2000 and Windows XP. If the system is able to
run Windows, it should be able to run the program.</font></p>

<p><strong><font size="+1">Q: Will Graph
run under Linux?</font></strong><font size="+1"><br>

<strong>A:</strong>&nbsp; Graph is a native
Windows
application and not tested under Linux, but several users have informed
me that Graph runs without problems under Linux with Wine.</font></p>

<p><font size="+1"><b>Q: Will
Graph run on a
Macintosh?<br>

A:</b> As with the above,
you cannot run Graph directly on a Mac. But it should be possible with
some
kind of Windows emulator.</font></p>

<p><font size="+1"><b>Q: When will the
next version be released?</b></font><font size="+1"><br>

<b>A:</b> When it is ready.<br>

</font><font size="+1">I try to release a new
version every 3 months, but sometimes it takes more time to release a
version, and sometimes a new version can be made faster. It depends on
how difficult it is to implement the new features and how much time I
have available to do the implementation.</font></p>

<p><strong><font size="+1">Q:</font></strong>
<font size="+1"><strong>How can I move the
coordinate system?</strong><br>

<strong>A:</strong> When you hold down the Ctrl key
you can use the arrow keys to move the coordinate system. You can also
use <em>Zoom|Move system</em> and drag the coordinate
system around with the mouse.</font></p>

<p><strong><font size="+1">Q:</font></strong>
<font size="+1"><strong>How can I easily zoom
in and out?</strong><br>

<strong>A:</strong>&nbsp; When you hold down the
Ctrl key you
can use the + and - keys to zoom in and out. The scroll wheel on the
mouse can be used for zooming at the cursor position. When you move the
scroll wheel up the program will zoom into the coordinate system and
center the window at the position of the mouse cursor. When you move
the scroll wheel down the program zooms out.</font></p>

<p><strong><font size="+1">Q:</font></strong>
<font size="+1"><strong>How do I save default
settings?</strong><br>

<strong>A:</strong>&nbsp; Set the desired default
settings in the <em>Edit axes</em> dialog, and put a mark
in <em>Save as default</em> before pressing the <em>OK</em>
button. Next time you create a new coordinate system, the saved default
settings will be used.</font></p>

<p><font size="+1"><strong>Q:</strong>
<strong>Why does the program not accept a comma as decimals
separator?</strong><br>

<strong>A:</strong>&nbsp; I know a lot of
countries uses
comma to separate the decimal part from the integer part, but Graph
uses comma for separating function arguments. The program always uses a
period to separate decimals from the integer value, no matter your
local settings.</font></p>

<p><font size="+1"><strong>Q:</strong>
<strong>How do I plot a vertical line?</strong><br>

<strong>A:</strong> &nbsp;A standard function
cannot be
vertical, but it can have a slope so large that it looks vertical. The
function f(x)=1000(x-5) looks like a vertical line at x=5. A real
vertical line can be drawn as a parametric function. Select <em>Parametric
function</em> as <em>function type</em>
when adding the function. You can when add the vertical line x=5 as
x(t)=5, y(t)=t. Alternatively you can add x=5 as a relation.</font></p>

<p><font size="+1"><strong>Q:</strong>
<strong>How do I plot a function x=f(y)?</strong><br>

<strong>A:</strong>&nbsp; To draw a function with
y as the independent variable, you need to use a parametric function.
Select <em>Parametric function</em> as <em>function
type</em>
when adding the function. If you want to draw the function x=sin(y),
you can now enter the function as x(t)=sin(t), y(t)=t. Alternatively
you can draw it as a relation where you can enter x=sin(y) directly. </font></p>

<p><font size="+1"><strong>Q:</strong>
<strong>How do I plot a circle?</strong><br>

<strong>A:</strong>&nbsp; You need to use a
parametric function to draw a circle. When inserting the function,
select <em>Parametric function</em> as <em>Function
type</em>. You can now add a circle with radius 5 and center in
(2,3) as x(t)=5cos(t)+2, y(t)=5sin(t)+3. You may need to use <em>Zoom|Square</em>
to make the axes equally scaled. Else the circle may look like an
ellipse. A circle can also be added as a polar function, but only with
center in (0,0). A circle with radius 5 may be added as the polar
function r(t)=5. Alternatively you can use a relation and add the
circle as (x-2)^2+(y-3)^2=5^2.</font></p>

<p><font size="+1"><b>Q: How do I
calculate the area between two functions?</b></font><br>

<font size="+1"><b>A:</b> If you want to
find the area between two functions f1(x)=3x and f2(x)= x^2, the
easiest way is to create a new function that is the difference between
thetwo functions: f(x)= f1(x)-f2(x)=3x-x^2. You can then use Calc|Area
to calculate the area for a given interval.</font></p>

<p><strong><font size="+1">Q:</font></strong>
<font size="+1"><strong>Can I copy a function,
shade or point series from one instance of the program to another?</strong><br>

<strong>A:</strong>&nbsp; Yes, you can select a
function or point series and use the <em>Edit</em> menu to
place a copy of the selected item in the clipboard. You can then paste
the item into another coordinate system.</font></p>

<p><strong><font size="+1">Q: How can I
draw the negative part of f(x)=sqrt(x+2) ?</font></strong><font size="+1"><br>

<strong>A:</strong> For each value x, f(x) will
evaluate to
at most one value. f(x)=sqrt(x+2) will therefore only have positive
values of f(x). To plot it for negative f(x) too, you will have to
create two separate functions: f(x)=sqrt(x+2) and f(x)=-sqrt(x+2).
Alternatively you can plot it as the relation: y^2=x+2.</font></p>

<p><font size="+1"><strong>Q: How do I
plot a complex function like f(t)=e^(i*t) ?</strong><br>

<strong>A:</strong> You probably want to show the
real part on
the x-axis and the imaginary part on the y-axis. In that case you can
draw the function as the parametric function x(t)=re(e^(i*t)),
y(t)=im(e^(i*t))</font></p>

<p><font size="+1"><strong>Q: How can I
make Graph plot functions with vertical asymptotes correctly?</strong><br>

<strong>A:</strong> Functions like f(x)=tan(x) with
vertical
asymptotes may not always be shown correctly. As default Graph will
evaluate the function for each pixel on the x-axis. But if the graph
has a steep slope that goes against infinite and back between two
pixels, Graph will not notice it. To plot the function correctly you
can tell Graph how many evaluations to perform. This may be entered in
the <em>Steps</em> field in the <em>Function|Insert
function</em> dialog. A number around 100000 will usually show
the function correctly.</font></p>

<p><font size="+1"><strong>Q: How to
create a PDF file from Graph?</strong><br>

<strong>A:</strong>&nbsp; You can choose to save
as PDF in the <em>File|Save as image</em> dialog.</font></p>

<p><b><font size="+1">Q: Should I make a
donation? <br>

A:</font></b> <font size="+1">Sure, donate
early and often. <br>

Seriously, it is completely up to you. The program is free of charge,
but if you want to support the development and the expenses involved, I
appreciate any kind of donation. </font></p>

<p><font size="+1"><strong>Q: Has the
previous question really been asked frequently?<br>

A:</strong> Would I joke about something like that?</font> </p>

<p><font size="+1"><b>Q:</b> <b>Why
will the program not start under Windows 95?</b><br>

<b>A:</b> Some of the system files under Windows 95
need to be updated to the newest version. The easiest way to do this is
to upgrade MS Internet Explorer to the newest version supporting
Windows 95.<br>

</font> </p>

<p>&nbsp;</p>

</div>

<?php include './Menu.inc'; ?>
</body>
</html>
