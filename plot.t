reset

set size square
set key left top

set multiplot layout 1,2
set label 1
    
    p "map_function.txt" w linespoints title 'map function'

unset label 1
set label 2
    set offset 1, 1, 1, 1

    unset border
    unset ytics
    unset xtics

    p "seq1.txt" w linespoints title 'sequence 1', \
      "seq2.txt" w linespoints title 'sequence 2', \
      "mapping.txt" w lines title 'mapping',  \
      "seq1.txt" using 1:2:(sprintf("%0.2f", $2 - 5)) w labels point pt 7 offset 0,1 notitle, \
      "seq2.txt" using 1:2:(sprintf("%0.2f", $2)) w labels point pt 7 offset 0,-1 notitle

unset label 2
unset multiplot
