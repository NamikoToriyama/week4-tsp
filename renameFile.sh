for var in "0" "1" "2" "3" "4" "5" "6"
do
infile="solution_yours_$var.csv"
outfile="greedy_2opt_plus_$var.csv"
mv $infile $outfile
done
