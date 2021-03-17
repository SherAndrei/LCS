reset

set key left top
set view equal xy

set macros
long = "0"
before_solving = "0"
show_map_func = "0"
shift = "2"


if (@long) {
    if (@before_solving) {
        set terminal png size 1000,1000; set output 'xyz.png';
        if (@show_map_func) {
            set multiplot layout 2,2
        } else {
            set multiplot layout 2, 1
        }
        set label 1
            set grid
            p "to_plot/seq1.txt" w lines title 'sequence 1' lw 3
            unset grid
        unset label 1
        set label 2
            set grid
            p "to_plot/seq2.txt" w lines title 'sequence 2' lw 3
            unset grid
        unset label 2
    } else {
        if (@show_map_func) {
            set terminal png size 1000,500; set output 'xyz.png';
            set multiplot layout 1,2
        } else {
            set terminal png size 500,500; set output 'xyz.png';
            set multiplot layout 1,1
        }
    }
    if (@show_map_func) {
        set label 3
            set grid
            p "to_plot/map_function.txt" w lines title 'map function' lw 3
            unset grid
        unset label 3
    }
    set label 4
        set border 0
        unset ytics
        unset xtics

        p "to_plot/shifted_seq1.txt" w lines title 'sequence 1' lw 3, \
        "to_plot/seq2.txt" w lines title 'sequence 2' lw 3, \
        "to_plot/mapping.txt" w lines title 'mapping' lw 1

        unset border
    unset label 4
    unset multiplot
    unset macros
    unset terminal
} else {
    set margins 5, 5, 3, 3
    if (@show_map_func) {
        set terminal png size 1000,500; set output 'xyz.png';
        set multiplot layout 1,2
        set label 1
            set grid
            p "to_plot/map_function.txt" w linespoints title 'map function' lw 3
            unset grid
        unset label 1
    } else {
        set terminal png size 500,500; set output 'xyz.png';
        set multiplot layout 1,1
    }

    set label 2
        set border 0
        unset ytics
        unset xtics

        p "to_plot/shifted_seq1.txt" w linespoints title 'sequence 1' lw 3, \
        "to_plot/seq2.txt" w linespoints title 'sequence 2' lw 3, \
        "to_plot/mapping.txt" w lines title 'mapping' lw 2 ,  \
        "to_plot/shifted_seq1.txt" using 1:2:(sprintf("%0.2f", $2 - @shift)) w labels point pt 7 offset 0,1 notitle, \
        "to_plot/seq2.txt" using 1:2:(sprintf("%0.2f", $2)) w labels point pt 7 offset 0,-1 notitle

        unset border
    unset label 2
    unset multiplot

    unset terminal
}
