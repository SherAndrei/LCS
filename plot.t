reset
unset border
unset ytics
unset xtics
unset raxis
unset rtics

set offset 1, 1, 1, 1

plot "seq1.txt" w lines, \
     "seq2.txt" w lines, \
     "mapping.txt" w lines
     "seq1.txt" using 1:2:(sprintf("%0.2f", $2 - 5)) w labels point pt 7 offset 0,-1 notitle, \
     "seq2.txt" using 1:2:(sprintf("%0.2f", $2)) w labels point pt 7 offset 0, 1 notitle